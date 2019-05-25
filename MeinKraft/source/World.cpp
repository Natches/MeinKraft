#include <fstream>
#include <sstream>
#include <ctime>

#include "World.h"
#include "CubeShader.h"
#include "DefaultErrorShader.h"
#include "INISettings.h"
#include "Shader2D.h"
#include "PoolThread.h"
#include "Chunk.h"
#include "GameTime.h"
#include "FileUtils.h"

int nb = 0;


namespace world
{
	const char*	World::SaveFolder = "Data/Save/";
	std::string World::m_mapName;

	World::World()
	{
		m_soundController.Init();
		m_soundController.GetBackgroundMusic().openFromFile(MUSIC_SOUND_FOLDER + "Calm1_old.ogg");
		m_soundController.GetBackgroundMusic().setVolume(MUSIC_VOLUME);
		m_soundController.GetBackgroundMusic().setLoop(true);
		m_soundController.GetBackgroundMusic().play();

		srand((unsigned int)time(NULL));
		srand(time(NULL));
		CheckIfFolderExist(SaveFolder);
		std::cout << "Which Map do you want to load ?" << std::endl;
		std::cin >> m_mapName;
		m_mapName += "/";
		CheckIfFolderExist((std::string(SaveFolder) + m_mapName).c_str());
		std::hash<std::string> hash;
		m_hashMapName = hash(m_mapName);
		m_random.seed(m_hashMapName);
		m_shaderLib.LoadShaders(3, new DefaultErrorShader(), new CubeShader(), new Shader2D);
		m_blockLibrary.InitLibrary();
		m_biomeLib.InitLibrary();
	}

	World::~World()
	{
		ThreadLib::PoolThread& pool = ThreadLib::PoolThread::getInstance();
		for (auto chunk : m_chunkList)
			pool.AddTask(new TaskThreadLib::TaskTemplated::TaskTemplate<>([chunk]() {delete chunk.second; }, false));
		for (auto chunk : m_bufferChunkList)
			pool.AddTask(new TaskThreadLib::TaskTemplated::TaskTemplate<>([chunk]() {delete chunk.second; }, false));
		for (auto reg : m_regionHeightMaps)
			delete[] reg.second.first.first;
		pool.FinishAllTaskThenContinue();
		Chunk::DeleteOpenFile();
	}

	Shader* World::GetShader(const std::string& name)
	{
		return m_shaderLib.GetShader(name);
	}

	void World::RenderChunk(const ShaderParameter& param)
	{
		vecs::Vec3 dir = vecs::Vec3(param.camDir[0], 0.f, param.camDir[2]);
		m_worldMutex.lock();
		for (auto& chunk : m_chunkList)
		{
			vecs::Vec3 CamToChunk = mathlib::Direction(vecs::Vec3(param.camPos[0], 0.f, param.camPos[2]), chunk.second->GetPos());
			
			if ((!mathlib::IsSameDirection(dir, CamToChunk) &&
				mathlib::Magnitude(CamToChunk) < DefaultWidth * MULTIPLIER_BEHIND_US_FOR_RENDERING) ||
				mathlib::IsSameDirection(dir, CamToChunk))
			{
				chunk.second->Render(m_blockLibrary, *(CubeShader*)(m_shaderLib.GetShader("CubeShader")), param);
			}
		}
		m_worldMutex.unlock();
	}

	block::Block* World::GetBlock(const vecs::Vec3& pos)
	{
		block::Block* b = nullptr;
		m_worldMutex.lock();
		for (auto& chunk : m_chunkList)
		{
			if (chunk.second->IsInBoundGlobal(pos))
			{
				b = chunk.second->GetBlock(pos);
				break;
			}
		}
		if (!b)
		{
			for (auto& chunk : m_bufferChunkList)
			{
				if (chunk.second->IsInBoundGlobal(pos))
				{
					b = chunk.second->GetBlock(pos);
					break;
				}
			}
		}
		m_worldMutex.unlock();
		return b;
	}

	std::vector<vecs::Vec3> World::GetBlocksAround(const vecs::Vec3& pos, const unsigned int range)
	{
		block::Block*			block;
		std::vector<vecs::Vec3> blocks;
		vecs::Vec3				newPos;
		for (int z = (int)pos.z - range; z <= pos.z + range; ++z)
		{
			for (int y = (int)pos.y - range; y <= pos.y + range + HEIGHT_OFFSET; ++y)
			{
				for (int x = (int)pos.x - range; x <= pos.x + range; ++x)
				{
					newPos.x = x; newPos.y = y; newPos.z = z;
					block = GetBlock(newPos);
					if (block && block->GetID() != block::AIR)
					{
						blocks.push_back(newPos);
					}
				}
			}
		}
		return blocks;
	}

	Chunk* World::GetChunk(const vecs::Vec3& Pos)
	{
		Chunk* c = nullptr;
		m_worldMutex.lock();
		for (auto chunk : m_chunkList)
		{
			if (chunk.second->IsInBoundGlobal(Pos))
			{
				c = chunk.second;
				break;
			}
		}
		if ( c == nullptr )
			for (auto chunk : m_bufferChunkList)
			{
				if (chunk.second->IsInBoundGlobal(Pos))
				{
					c = chunk.second;
					break;
				}
			}
		m_worldMutex.unlock();
		return c;
	}

	void World::ModifyBlock(const vecs::Vec3& pos, const uint16_t& ID)
	{
		block::Block* block = GetBlock(pos);
		if (block)
		{
			block::Block::E_OCCL_FLAGS old = block->GetOCCLState();
			*block = m_blockLibrary.GetBlock(ID);
			block->SetOcclState(old);

			//Occlusion checks
			GetChunk(pos)->BlockOcclusionCheck(pos, m_blockLibrary);

		}
	}

	void World::UpdateChunkLists(const vecs::Vec2& playerPos)
	{
		vecs::Vec3 playerChunk = vecs::Vec3(floor(playerPos.x / DefaultWidth) * DefaultWidth, 0.f, floor(playerPos.y / DefaultDepth)  * DefaultDepth);
		std::vector<vecs::Vec3> ChunkToGen;
		for (int i = (-INISettings::viewDistance - 1); i <= INISettings::viewDistance + 1; ++i)
		{
			for (int j = (-INISettings::viewDistance - 1); j <= INISettings::viewDistance + 1; ++j)
			{
				vecs::Vec3 chunkPos = vecs::Vec3((floor(playerPos.x / DefaultWidth) + i)* DefaultWidth, 0.f, (floor(playerPos.y / DefaultDepth) + j)  * DefaultDepth);
				int Index1D = mathlib::To1D(chunkPos, DefaultWidth, DefaultHeight);
				bool ForBuffer = true;
				if (m_bufferChunkList.size() > 0)
				{
					ForBuffer = !mathlib::IsInside(chunkPos, playerChunk - vecs::Vec3(INISettings::viewDistance * DefaultWidth, 0.f, INISettings::viewDistance * DefaultDepth), playerChunk + vecs::Vec3(INISettings::viewDistance * DefaultWidth, DefaultHeight, INISettings::viewDistance * DefaultDepth));
				}
				if (m_bufferChunkList.find(Index1D) != m_bufferChunkList.end())
				{
					if (!ForBuffer)
					{
						m_worldMutex.lock();
						m_chunkList.insert(std::make_pair(Index1D, m_bufferChunkList[Index1D]));
						m_bufferChunkList.erase(Index1D);
						m_worldMutex.unlock();
					}
				}
				else if (m_chunkList.find(Index1D) != m_chunkList.end())
				{
					if (ForBuffer)
					{
						m_worldMutex.lock();
						m_bufferChunkList.insert(std::make_pair(Index1D, m_chunkList[Index1D]));
						m_chunkList.erase(Index1D);
						m_worldMutex.unlock();
					}
				}
				else if (m_genChunkList.find(Index1D) == m_genChunkList.end())
				{
					ChunkToGen.push_back(chunkPos);
				}
			}
		}

		for (auto& c : ChunkToGen)
		{
			GenerateChunk({ c.x, c.z });
		}

		if (nb == TIME_BEFORE_GC)
		{
			ClearFarChunk(playerPos);
			ClearFarRegion(playerPos);
			nb = 0;
		}
		++nb;
	}

	void World::MoveChunkFromGenBufferToChunkBuffer(Chunk* chunk)
	{
		m_worldMutex.lock();
		m_bufferChunkList.insert(std::make_pair(mathlib::To1D(chunk->GetPos(), DefaultWidth, DefaultHeight), chunk));
		m_genChunkList.erase(mathlib::To1D(chunk->GetPos(), DefaultWidth, DefaultHeight));
		m_worldMutex.unlock();
	}
	void World::GenerateRegionHeightMap(const vecs::Vec2& pos)
	{
		unsigned char (*HeightMap)[Zmax] = new unsigned char[Xmax][Zmax];
		uint8_t biomeID = m_biomeLib.GetRandomBiome().GetID();

		int MaxHeight = m_biomeLib.GetBiomeData(biomeID).GetMaxHeight() / 2;

		memset(HeightMap, 0, sizeof(unsigned char) * Xmax * Zmax);
		std::uniform_int_distribution<int> uniDistrib(1, 24);
		std::uniform_int_distribution<int> uniDistribOffset(8, 24);

		int offset = uniDistribOffset(m_random);

		int OffsetX = (int)(Xmax / offset);
		int OffsetZ = (int)(Zmax / offset);

		for (int idx = 0; idx < 12; ++idx)
		{
			uint8_t biomeID = m_biomeLib.GetRandomBiome().GetID();
			int MaxBiomeHeight = m_biomeLib.GetBiomeData(biomeID).GetMaxHeight();
			int MinBiomeHeight = m_biomeLib.GetBiomeData(biomeID).GetMinHeightX();
			for (int X = 0; X < Xmax; X += OffsetX)
			{
				float pow1 = 1.f / (float)(2.f * std::pow(uniDistribOffset(m_random), 2));
				float pow2 = 1.f / (float)(2.f * std::pow(uniDistribOffset(m_random), 2));
				for (int Z = 0; Z < Zmax; Z += OffsetZ)
				{
					if (m_random() % 2)
					{
						int i = m_random() % 2;
						if (i == 0)
							i = -1;
						MaxHeight += i * m_random() % uniDistrib(m_random);
						if (MaxHeight < MinBiomeHeight)
							MaxHeight = MinBiomeHeight;
						else if (MaxHeight > MaxBiomeHeight)
						{
							MaxHeight = MaxBiomeHeight;
							MaxHeight -= m_random() % uniDistrib(m_random);
						}
						for (int X2 = X, maxX = X + OffsetX, demiMaxX = maxX - (int)(OffsetX * 0.5f); X2 < maxX && X2 < Xmax; ++X2)
						{
							for (int Z2 = Z, maxZ = Z + OffsetZ, demiMaxZ = maxZ - (int)(OffsetZ * 0.5f); Z2 < maxZ && Z2 < Zmax; ++Z2)
							{
								HeightMap[X2][Z2] += uint8_t(MaxHeight * std::exp(-((((X2 - demiMaxX) * (X2 - demiMaxX)) * pow1) + (((Z2 - demiMaxZ) * (Z2 - demiMaxZ)) * pow2))));
								HeightMap[X2][Z2] = (HeightMap[X2][Z2] > MaxBiomeHeight ? MaxBiomeHeight : HeightMap[X2][Z2]);
								HeightMap[X2][Z2] == MaxBiomeHeight ? Z2 = maxZ, X2 = maxX : 0;
							}
						}
					}
				}
			}
		}
		m_genMutex.lock();
		BlurHeightMap(HeightMap, pos);
		m_regionHeightMaps.insert(std::make_pair(std::to_string((int)pos.x) + std::to_string((int)pos.y), std::make_pair(std::make_pair(HeightMap, biomeID), pos)));
		m_genMutex.unlock();
	}

	void World::BlurHeightMap(unsigned char(*heightMap)[Zmax], const vecs::Vec2& regionPos)
	{
		bool heightmapX1 = m_regionHeightMaps.find(std::to_string((int)regionPos.x + 1) + std::to_string((int)regionPos.y)) != m_regionHeightMaps.end();
		bool heightmapY1 = m_regionHeightMaps.find(std::to_string((int)regionPos.x) + std::to_string((int)regionPos.y + 1)) != m_regionHeightMaps.end();
		bool heightmapXm1 = m_regionHeightMaps.find(std::to_string((int)regionPos.x - 1) + std::to_string((int)regionPos.y)) != m_regionHeightMaps.end();
		bool heightmapYm1 = m_regionHeightMaps.find(std::to_string((int)regionPos.x) + std::to_string((int)regionPos.y - 1)) != m_regionHeightMaps.end();


		unsigned char newData[Xmax][Zmax];
		for (int idx = 1; idx < INISettings::blurPasses; idx += 2)
		{
			int mdiIdx = (int)(idx * 0.5f);
			for (int X = 0; X < Xmax; ++X)
			{
				for (int Y = 0; Y < Zmax; ++Y)
				{
					int col = 0;
					for (int i = -mdiIdx; i <= mdiIdx; ++i)
					{
						for (int j = -mdiIdx; j <= mdiIdx; ++j)
						{
							if (((X + i >= 0 && X + i < Xmax) && (Y + j >= 0 && Y + j < Zmax)))
							{
								++nb;
								col += int(heightMap[(X + i)][(Y + j)]);
							}
							if (X + i < 0 && heightmapXm1 && (Y + j >= 0 && Y + j < Zmax))
							{
								++nb;
								int coltemp = int(m_regionHeightMaps[std::to_string((int)regionPos.x - 1) + std::to_string((int)regionPos.y)].first.first[(X + Xmax + i)][(Y + j)]);
								col += coltemp * (coltemp > heightMap[X][Y] ? 1.1f : 0.90f);
							}
							else if (X + i > Xmax - 1 && heightmapX1 && (Y + j >= 0 && Y + j < Zmax))
							{
								++nb;
								int coltemp = int(m_regionHeightMaps[std::to_string((int)regionPos.x + 1) + std::to_string((int)regionPos.y)].first.first[((X + i) % Xmax)][(Y + j)]);
								col += coltemp * (coltemp > heightMap[X][Y] ? 1.1f : 0.90f);
							}
							if (Y + j < 0 && heightmapYm1 && (X + i >= 0 && X + i < Xmax))
							{
								++nb;
								int coltemp = int(m_regionHeightMaps[std::to_string((int)regionPos.x) + std::to_string((int)regionPos.y - 1)].first.first[((X + i))][(Y + Zmax + j)]);
								col += coltemp * (coltemp > heightMap[X][Y] ? 1.1f : 0.90f);
							}
							else if (Y + j > Zmax - 1 && heightmapY1 && (X + i >= 0 && X + i < Xmax))
							{
								++nb;
								int coltemp = int(m_regionHeightMaps[std::to_string((int)regionPos.x) + std::to_string((int)regionPos.y + 1)].first.first[((X + i))][((Y + j) % Zmax)]);
								col += coltemp  * (coltemp > heightMap[X][Y] ? 1.1f : 0.90f);
							}
						}
					}
					col = col ? col : heightMap[X][Y];
					nb = nb ? nb : 1;
					col /= (nb);
					newData[X][Y] = uint8_t(col);
					nb = 0;
				}
			}
			memcpy(heightMap, newData, sizeof(uint8_t) * Xmax * Zmax);
			//memcpy(newData, heightMap, sizeof(uint8_t) * Xmax * Zmax);
		}
	}

	void World::GenerateChunk(const vecs::Vec2& pos)
	{
		ThreadLib::PoolThread& pool = ThreadLib::PoolThread::getInstance();
		Chunk* c = new Chunk(vecs::Vec3(pos.x, 0.f, pos.y), m_biomeLib.GetRandomBiome().GetID());
		pool.AddTask(new TaskThreadLib::TaskTemplated::TaskTemplate<>([c, this]() {c->CreateChunk(m_biomeLib, m_blockLibrary, *this); }, false));
		m_genChunkList.insert(std::make_pair(mathlib::To1D(c->GetPos(), DefaultWidth, DefaultHeight), c));
	}

	void World::ClearFarChunk(const vecs::Vec2& playerPos)
	{
		if ((int(mathlib::Abs(playerPos.x/DefaultWidth)) > int(mathlib::Abs(m_previousPlayerPos.x / DefaultWidth)) ||
			int(mathlib::Abs(playerPos.y / DefaultDepth)) > int(mathlib::Abs(m_previousPlayerPos.y / DefaultDepth))) &&
			m_bufferChunkList.size() > 1)
		{
			ThreadLib::PoolThread& pool = ThreadLib::PoolThread::getInstance();
			std::pair<int, Chunk*>* chunkToClear = new std::pair<int, Chunk *>[m_bufferChunkList.size()];
			int idx = 0;
			vecs::Vec3 playerChunk = vecs::Vec3(floor(playerPos.x / DefaultWidth) * DefaultWidth, 0.f, floor(playerPos.y / DefaultDepth)  * DefaultDepth);
			int viewDist1 = INISettings::viewDistance + 1;
			m_worldMutex.lock();
			for (auto& chunk : m_bufferChunkList)
			{
				vecs::Vec3 chunkPos = chunk.second->GetPos();
				if (!mathlib::IsInside(chunkPos, playerChunk - vecs::Vec3(viewDist1 * DefaultWidth, 0.f, viewDist1 * DefaultDepth), playerChunk + vecs::Vec3(viewDist1 * DefaultWidth, DefaultHeight, viewDist1 * DefaultDepth)))
				{
					chunkToClear[idx] = chunk;
					++idx;
				}
			}
			for (int i = 0; i < idx; ++i)
			{
				m_bufferChunkList.erase(chunkToClear[i].first);
				Chunk* c = chunkToClear[i].second;
				pool.AddTask(new TaskThreadLib::TaskTemplated::TaskTemplate<>([c]() { delete c; }, false));
			}
			m_worldMutex.unlock();
			delete[] chunkToClear;
		}
		m_previousPlayerPos = playerPos;
	}

	void World::ClearFarRegion(const vecs::Vec2& playerPos)
	{
		if ((int(mathlib::Abs(playerPos.x / DefaultWidth)) > int(mathlib::Abs(m_previousPlayerPos.x / DefaultWidth)) ||
			int(mathlib::Abs(playerPos.y / DefaultDepth)) > int(mathlib::Abs(m_previousPlayerPos.y / DefaultDepth))) &&
			m_regionHeightMaps.size() > 1)
		{
			std::pair<std::string, std::pair<std::pair<unsigned char(*)[Zmax], uint8_t>, vecs::Vec2>>* regionToClear = new std::pair<std::string, std::pair<std::pair<unsigned char(*)[Zmax], uint8_t>, vecs::Vec2>>[m_regionHeightMaps.size()];
			int idx = 0;
			vecs::Vec3 playerRegion = vecs::Vec3(floor(floor(playerPos.x / DefaultWidth) * DefaultWidth / (DefaultWidth * WIDTH_DEPTH_REGION_FILE)), 0.f, floor(floor(playerPos.y / DefaultDepth)  * DefaultDepth / (DefaultWidth * WIDTH_DEPTH_REGION_FILE)));
			m_genMutex.unlock();
			for (auto& reg : m_regionHeightMaps)
			{
				vecs::Vec2 regionPos = reg.second.second;
				if ((regionPos.x > playerRegion.x + DefaultWidth || regionPos.x < playerRegion.x - DefaultWidth) && 
					(regionPos.y > playerRegion.z + DefaultWidth || regionPos.y < playerRegion.z - DefaultWidth))
				{
					regionToClear[idx] = reg;
					++idx;
				}
			}
			for (int i = 0; i < idx; ++i)
			{
				m_regionHeightMaps.erase(regionToClear[i].first);
				delete[] regionToClear[i].second.first.first;
			}
			m_genMutex.unlock();
			delete[] regionToClear;
		}
		m_previousPlayerPos = playerPos;
	}

	uint16_t World::BreakBlock(const Ray& ray, const float damage)
	{
		static float			currentDurability;
		static float			maxDurability;
		static block::Block*	lastBlock = nullptr;
		static float			elapsedTime = 0.f;
		const float				COOLDOWN = 0.2f;

		block::Block*	block;
		uint16_t		blockID;
		vecs::Vec3		closestCenter;

		elapsedTime += GameTime::Instance()->GetDeltaTime();

		if (FindClosestBlock(ray, closestCenter))
		{
			block = GetBlock(closestCenter);
			if (block && block->GetID() != block::AIR)
			{
				blockID = block->GetID();
				if (elapsedTime >= COOLDOWN)
				{
					m_soundController.GetBlockSoundData(blockID).m_mineSound.PlaySound(false, SOUND_EFFECT_VOLUME / 2.f);
					elapsedTime = 0.f;
				}

				if (GetBlockLibrary().GetBlockData(blockID).IsBreakable())
				{
					if (lastBlock == block)
					{
						currentDurability -= damage * GameTime::Instance()->GetDeltaTime();
						if (1 - (currentDurability / maxDurability) >= 0 && 1 - (currentDurability / maxDurability) < 0.1f)
							static_cast<CubeShader*>(m_shaderLib.GetShader("CubeShader"))->setTextureBreak(CubeShader::E_PERCENT::ZERO);
						else if (1.f - (currentDurability / maxDurability) >= 0.1f && 1.f - (currentDurability / maxDurability) < 0.2f)
							static_cast<CubeShader*>(m_shaderLib.GetShader("CubeShader"))->setTextureBreak(CubeShader::E_PERCENT::TEN);
						else if (1.f - (currentDurability / maxDurability) >= 0.2f && 1.f - (currentDurability / maxDurability) < 0.3f)
							static_cast<CubeShader*>(m_shaderLib.GetShader("CubeShader"))->setTextureBreak(CubeShader::E_PERCENT::TWENTY);
						else if (1.f - (currentDurability / maxDurability) >= 0.3f && 1.f - (currentDurability / maxDurability) < 0.4f)
							static_cast<CubeShader*>(m_shaderLib.GetShader("CubeShader"))->setTextureBreak(CubeShader::E_PERCENT::THIRTY);
						else if (1.f - (currentDurability / maxDurability) >= 0.4f && 1.f - (currentDurability / maxDurability) < 0.5f)
							static_cast<CubeShader*>(m_shaderLib.GetShader("CubeShader"))->setTextureBreak(CubeShader::E_PERCENT::FORTHY);
						else if (1.f - (currentDurability / maxDurability) >= 0.5f && 1.f - (currentDurability / maxDurability) < 0.6f)
							static_cast<CubeShader*>(m_shaderLib.GetShader("CubeShader"))->setTextureBreak(CubeShader::E_PERCENT::FIFTY);
						else if (1.f - (currentDurability / maxDurability) >= 0.6f && 1.f - (currentDurability / maxDurability) < 0.7f)
							static_cast<CubeShader*>(m_shaderLib.GetShader("CubeShader"))->setTextureBreak(CubeShader::E_PERCENT::SIXTY);
						else if (1.f - (currentDurability / maxDurability) >= 0.7f && 1.f - (currentDurability / maxDurability) < 0.8f)
							static_cast<CubeShader*>(m_shaderLib.GetShader("CubeShader"))->setTextureBreak(CubeShader::E_PERCENT::SEVENTY);
						else if (1.f - (currentDurability / maxDurability) >= 0.8f && 1.f - (currentDurability / maxDurability) < 0.9f)
							static_cast<CubeShader*>(m_shaderLib.GetShader("CubeShader"))->setTextureBreak(CubeShader::E_PERCENT::HEIGHTY);
						else if (1.f - (currentDurability / maxDurability) >= 0.9f && 1.f - (currentDurability / maxDurability) < 1.f)
							static_cast<CubeShader*>(m_shaderLib.GetShader("CubeShader"))->setTextureBreak(CubeShader::E_PERCENT::NINETY);
					}
					else
					{
						if(lastBlock)
							lastBlock->RemoveOcclState(block::Block::BREAK);
						block->AddOcclState(block::Block::BREAK);
						maxDurability		= GetBlockLibrary().GetBlockData(blockID).GetMaxDurability();
						currentDurability	= maxDurability - damage * GameTime::Instance()->GetDeltaTime();
						GetChunk(closestCenter)->SetDirtyVBOs(true);
					}
					lastBlock = block;

					if (currentDurability <= 0.f)
					{
						m_soundController.GetBlockSoundData(blockID).m_mineSound.StopSound();
						m_soundController.GetBlockSoundData(blockID).m_breakSound.PlaySound(false, SOUND_EFFECT_VOLUME);
						ModifyBlock(closestCenter, block::AIR);
						return blockID;
					}
				}
			}
			lastBlock = block;
		}
		return block::AIR;
	}

	bool World::FindClosestBlock(const Ray& ray, vecs::Vec3& closestCenter)
	{
		AABBCollider aabb;
		float closestPoint				= INFINITY, point = INFINITY;
		std::vector<vecs::Vec3> blocks	= GetBlocksAround(ray.m_origin, 4u);
		bool blockFound					= false;

		for (auto block : blocks)
		{
			aabb.SetCenter(block);
			point = INFINITY;
			if (ray.Intersects(aabb, point) && point < closestPoint && GetBlock(block)->GetID() != block::AIR)
			{
				closestPoint = point;
				closestCenter = aabb.GetCenter();
				blockFound = true;
			}
		}
		return blockFound;
	}

	bool World::PlaceBlock(const Ray& ray, CapsuleCollider capsule, const uint16_t ID)
	{
		vecs::Vec3 closestCenter;

		if (FindClosestBlock(ray, closestCenter))
		{
			AABBCollider aabb(closestCenter);
			vecs::Vec3 intersection, directionToPlace;
			float t;

			ray.Intersects(aabb, t);

			intersection = ray.m_origin + ray.m_direction * (t);

			directionToPlace = FindDirection(intersection, aabb);
			aabb.SetCenter(aabb.GetCenter() + directionToPlace);

			if (capsule.Intersects(aabb, closestCenter))
			{
				return false;
			}

			m_soundController.GetBlockSoundData(ID).m_placeSound.PlaySound(false, SOUND_EFFECT_VOLUME);
			ModifyBlock(aabb.GetCenter(), ID);
			return true;
		}
		return false;
	}

	vecs::Vec3 World::FindDirection(const vecs::Vec3& intersection, const AABBCollider& aabb)
	{
		vecs::Vec3 direction;
		if (intersection.x <= aabb.GetPointMin().x + EPSILON)
		{
			direction.x = -1.f;
		}
		else if (intersection.x >= aabb.GetPointMax().x - EPSILON)
		{
			direction.x = 1.f;
		}
		else if (intersection.y <= aabb.GetPointMin().y + EPSILON)
		{
			direction.y = -1.f;
		}
		else if (intersection.y >= aabb.GetPointMax().y - EPSILON)
		{
			direction.y = 1.f;
		}
		else if (intersection.z <= aabb.GetPointMin().z + EPSILON)
		{
			direction.z = -1.f;
		}
		else if (intersection.z >= aabb.GetPointMax().z - EPSILON)
		{
			direction.z = 1.f;
		}
		return direction;
	}

	void World::ReloadShader()
	{
		m_shaderLib.Clean();
		//m_shaderLib = ShaderLib();
		m_shaderLib.LoadShaders(3, new DefaultErrorShader(), new CubeShader(), new Shader2D);
	}

	std::pair<unsigned char(*)[Zmax], uint8_t>& World::GetHeightMapData(const vecs::Vec2& ChunkPos)
	{
		int posY = floor(ChunkPos.y / (DefaultDepth * WIDTH_DEPTH_REGION_FILE)), posX = floor(ChunkPos.x / (DefaultWidth * WIDTH_DEPTH_REGION_FILE));
		if (m_regionHeightMaps.find(std::to_string(posX) + std::to_string(posY)) == m_regionHeightMaps.end())
		{
			GenerateRegionHeightMap(vecs::Vec2( posX, posY ));
		}
		return m_regionHeightMaps[std::to_string(posX) + std::to_string(posY)].first;
	}
}
