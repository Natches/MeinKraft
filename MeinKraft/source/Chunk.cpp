#include <fstream>
#include <sstream>
#include <io.h>
#include <string>
#include "Chunk.h"
#include "ShaderLib.h"
#include "CubeShader.h"
#include "PoolThread.h"
#include "World.h"
#include "FileUtils.h"

std::mutex								Chunk::m_saveMutex;
std::atomic<std::vector<std::string>*>	Chunk::OpenFile;
const char*								Chunk::RegionDataFolder = "RegionData/";
const char*								Chunk::RegionDataExtension = ".mrf";

Chunk::Chunk(vecs::Vec3 Pos, uint8_t BiomeId)
	: m_pos(Pos), m_biomeID(BiomeId)
{
	if (OpenFile.load(std::memory_order_acquire) == nullptr)
		OpenFile.store(new std::vector<std::string>(), std::memory_order_release);
	m_gen = false;
	m_vao = 0;
	m_vbo = 0;
	m_texVBO = 0;
	m_texVBO2 = 0;
	m_scaleVbo = 0;
}

Chunk::~Chunk()
{
	SaveChunkInRegion();
	if (m_vao)
		glDeleteVertexArrays(1, &m_vao);
	if (m_vbo)
		glDeleteBuffers(1, &m_vbo);
	if (m_texVBO)
		glDeleteBuffers(1, &m_texVBO);
	if (m_texVBO2)
		glDeleteBuffers(1, &m_texVBO2);
	if (m_scaleVbo)
		glDeleteBuffers(1, &m_scaleVbo);
	/*if (m_lightVBO)
		glDeleteBuffers(1, &m_lightVBO);
	if (m_lightVBO2)
		glDeleteBuffers(1, &m_lightVBO2);*/
}

void Chunk::CreateChunk(const biomes::BiomeLibrary& biomeLib, const block::BlockLibrary& blockLib, world::World& world)
{
	if (!LoadChunkFromRegion(blockLib))
	{
		std::pair<unsigned char(*)[Zmax], uint8_t>& HeightMapData = world.GetHeightMapData(vecs::Vec2(m_pos.x, m_pos.z));
		m_biomeID = HeightMapData.second;
		ParseHeightMapData(HeightMapData.first, biomeLib, blockLib);
		OcclusionCheck(blockLib);
	}
	else
	{
		OcclusionCheck(blockLib);
	}
	GenerateVector(blockLib);
	world.MoveChunkFromGenBufferToChunkBuffer(this);
	m_dirtyVBOs = true;
	m_gen = true;
}

void Chunk::Render(const block::BlockLibrary& blockLib, CubeShader& shader, const ShaderParameter& param)
{
	if (m_dirtyVBOsAndVector)
	{
		RegenerateVao(blockLib);
	}
	else if (m_dirtyVBOs)
	{
		GenerateVAO();
	}
	glBindVertexArray(m_vao);
	shader.Update(param);
	glDrawArrays(GL_POINTS, 0, VBO.size() / 4);
}

block::Block* Chunk::GetBlock(const vecs::Vec3& pos)
{
	if (IsInBound(vecs::Vec3(round(pos.x - m_pos.x), round(pos.y - m_pos.y), round(pos.z - m_pos.z))))
		return &m_blockArray[mathlib::To1D(vecs::Vec3(round(pos.x - m_pos.x), round(pos.y - m_pos.y), round(pos.z - m_pos.z)), DefaultWidth, DefaultHeight)];
	else
		return nullptr;
}

void Chunk::ParseHeightMapData(unsigned char(*heightMap)[Zmax], const biomes::BiomeLibrary & biomeLib, const block::BlockLibrary & blockLib)
{
	int posX = mathlib::Abs((floor(m_pos.x / (DefaultWidth * WIDTH_DEPTH_REGION_FILE)) * DefaultWidth * WIDTH_DEPTH_REGION_FILE) - m_pos.x), posZ = mathlib::Abs(floor(m_pos.z / (DefaultWidth * WIDTH_DEPTH_REGION_FILE)) * DefaultWidth * WIDTH_DEPTH_REGION_FILE - m_pos.z);
	std::pair<uint16_t, float>* availableBlock = new std::pair<uint16_t, float>[biomeLib.GetBiomeData(m_biomeID).GetLengthBlocklist()];
	InitBlockToSort(availableBlock, biomeLib);
	for (int Y = 0; Y < DefaultHeight; ++Y)
	{
		SortAvailableBlock(Y, availableBlock, biomeLib, blockLib);
		for (int X = posX; X < posX + DefaultWidth; ++X)
		{
			for (int Z = posZ; Z < posZ + DefaultDepth; ++Z)
			{
				int in1D = mathlib::To1D(vecs::Vec3((float)X - posX, (float)Y, (float)Z - posZ), DefaultWidth, DefaultHeight);
				int MaxY = (int)heightMap[X][Z];
				if (Y < MaxY + 1)
				{
					for (int i2 = 0; i2 < biomeLib.GetBiomeData(m_biomeID).GetLengthBlocklist(); ++i2)
					{
						float nb = (rand() % 10000) * 0.01f;
						if (nb < availableBlock[i2].second)
						{
							m_blockArray[in1D] = blockLib.GetBlock(availableBlock[i2].first);
							break;
						}
					}
					if (m_blockArray[in1D].GetID() == block::ERROR)
						m_blockArray[in1D] = blockLib.GetBlock(availableBlock[0].first);
				}
				else
				{
					m_blockArray[in1D] = blockLib.GetBlock(block::AIR);
				}
			}
		}
	}
	delete[] availableBlock;
}

void Chunk::InitBlockToSort(std::pair<uint16_t, float>* sortedAvailableBlock, const biomes::BiomeLibrary& biomeLib)
{
	uint16_t length = biomeLib.GetBiomeData(m_biomeID).GetLengthBlocklist();
	uint16_t* blockIdList = biomeLib.GetBiomeData(m_biomeID).GetBlockIDList();
	for (uint16_t i = 0; i < length; ++i)
	{
		sortedAvailableBlock[i].first = blockIdList[i];
	}
	delete blockIdList;
}

void Chunk::SortAvailableBlock(int y, std::pair<uint16_t, float>* sortedAvailableBlock, const biomes::BiomeLibrary& biomeLib, const block::BlockLibrary& blockLib)
{
	uint16_t length = biomeLib.GetBiomeData(m_biomeID).GetLengthBlocklist();

	for (uint16_t i = 0; i < length; ++i)
	{
		sortedAvailableBlock[i].second = biomeLib.GetBiomeData(m_biomeID).GetBlockChanceMultiplier(sortedAvailableBlock[i].first) * blockLib.GetBlockData(sortedAvailableBlock[i].first).GetGenPercent((uint16_t)y);
	}
	bool sorted = true;
	for (uint16_t i = 0; i < length - 1; ++i)
	{
		for (uint16_t j = i; j < length - 1; ++j)
		{
			if (sortedAvailableBlock[j].second < sortedAvailableBlock[j + 1].second)
			{
				sortedAvailableBlock[j].swap(sortedAvailableBlock[j + 1]);
				sorted = false;
			}
		}
		if (sorted)
			break;
	}
}

void Chunk::OcclusionCheck(const block::BlockLibrary& blockLibrary)
{
	int length = DefaultDepth * DefaultWidth * DefaultHeight;
	for (int i = 0; i < length; ++i)
	{
		if (m_blockArray[i].GetID() != block::AIR && !blockLibrary.GetBlockData(m_blockArray[i].GetID()).IsTransparent())
		{
			m_blockArray[i].SetOcclState(block::Block::NONE);
			vecs::Vec3 pos;
			mathlib::To3D(i, DefaultWidth, DefaultHeight, pos);
			IsInBound(pos + vecs::Vec3(1, 0, 0)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos + vecs::Vec3(1, 0, 0), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? m_blockArray[i].AddOcclState(block::Block::NONE) : m_blockArray[i].AddOcclState(block::Block::OCCLR)) : (void)0;
			IsInBound(pos + vecs::Vec3(-1, 0, 0)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos + vecs::Vec3(-1, 0, 0), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? m_blockArray[i].AddOcclState(block::Block::NONE) : m_blockArray[i].AddOcclState(block::Block::OCCLL)) : (void)0;
			IsInBound(pos + vecs::Vec3(0, 1, 0)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos + vecs::Vec3(0, 1, 0), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? m_blockArray[i].AddOcclState(block::Block::NONE) : m_blockArray[i].AddOcclState(block::Block::OCCLU)) : (void)0;
			IsInBound(pos + vecs::Vec3(0, -1, 0)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos + vecs::Vec3(0, -1, 0), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? m_blockArray[i].AddOcclState(block::Block::NONE) : m_blockArray[i].AddOcclState(block::Block::OCCLD)) : (void)0;
			IsInBound(pos + vecs::Vec3(0, 0, 1)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos + vecs::Vec3(0, 0, 1), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? m_blockArray[i].AddOcclState(block::Block::NONE) : m_blockArray[i].AddOcclState(block::Block::OCCLF)) : (void)0;
			IsInBound(pos + vecs::Vec3(0, 0, -1)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos + vecs::Vec3(0, 0, -1), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? m_blockArray[i].AddOcclState(block::Block::NONE) : m_blockArray[i].AddOcclState(block::Block::OCCLB)) : (void)0;
		}
		else if (blockLibrary.GetBlockData(m_blockArray[i].GetID()).IsTransparent())
		{
			m_blockArray[i].SetOcclState(block::Block::NONE);
		}
		else
		{
			m_blockArray[i].SetOcclState(block::Block::ALL);
		}
	}
	ComputeSkyOcclusion(blockLibrary);
}

void Chunk::ComputeSkyOcclusion(const block::BlockLibrary& blockLibrary)
{
	for (int X = 0; X < DefaultWidth; ++X)
	{
		for (int Z = 0; Z < DefaultDepth; ++Z)
		{
			for (int Y = 0; Y < DefaultHeight; ++Y)
			{
				int i = mathlib::To1D(vecs::Vec3(X, Y, Z), DefaultWidth, DefaultHeight);
				m_blockArray[i].RemoveOcclState(block::Block::SKY_OCCLUSION);
				if (!m_blockArray[i].IsOccludedUp())
				{
					for (int Y2 = Y + 1; Y2 < DefaultHeight; ++Y2, ++Y)
					{
						int i2 = mathlib::To1D(vecs::Vec3(X, Y2, Z), DefaultWidth, DefaultHeight);
						if (!blockLibrary.GetBlockData(m_blockArray[i2].GetID()).IsTransparent())
						{
							m_blockArray[i].AddOcclState(block::Block::SKY_OCCLUSION);
							break;
						}
					}
				}
			}
		}
	}
}

void Chunk::ComputeLightLevel(int index, unsigned char temp[6], const block::BlockLibrary& blockLibrary)
{
	/*if (blockLibrary.GetBlockData(m_blockArray[index].GetID()).IsIlluminating())
	{
		temp[block::BlockData::E_FACE::TOP] = MAX_LIGTH_LEVEL;
		temp[block::BlockData::E_FACE::DOWN] = MAX_LIGTH_LEVEL;
		temp[block::BlockData::E_FACE::LEFT] = MAX_LIGTH_LEVEL;
		temp[block::BlockData::E_FACE::RIGHT] = MAX_LIGTH_LEVEL;
		temp[block::BlockData::E_FACE::FORWARD] = MAX_LIGTH_LEVEL;
		temp[block::BlockData::E_FACE::BACKWARD] = MAX_LIGTH_LEVEL; 
	}*/
	if (!m_blockArray[index].IsSkyOccluded() && !blockLibrary.GetBlockData(m_blockArray[index].GetID()).IsIlluminating())
	{
		temp[block::BlockData::E_FACE::TOP] = MAX_LIGTH_LEVEL;
		memset(&temp[1], MIN_LIGTH_LEVEL, 5);
	}
	else
	{
		memset(temp, MIN_LIGTH_LEVEL, 6);
	}
}

void Chunk::BlockOcclusionCheck(const vecs::Vec3& pos, const block::BlockLibrary& blockLibrary)
{
	if (IsInBoundGlobal(pos))
	{
		block::Block* b = &m_blockArray[mathlib::To1D(pos - m_pos, DefaultWidth, DefaultHeight)];
		if (b->GetID() == block::AIR)
		{
			b->SetOcclState(block::Block::ALL);
			IsInBoundGlobal(pos + vecs::Vec3(1, 0, 0)) ? (m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(1, 0, 0), DefaultWidth, DefaultHeight)].GetID() == block::AIR ? (void)0 : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(1, 0, 0), DefaultWidth, DefaultHeight)].RemoveOcclState(block::Block::OCCLL)) : (void)0;
			IsInBoundGlobal(pos + vecs::Vec3(-1, 0, 0)) ? (m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(-1, 0, 0), DefaultWidth, DefaultHeight)].GetID() == block::AIR ? (void)0 : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(-1, 0, 0), DefaultWidth, DefaultHeight)].RemoveOcclState(block::Block::OCCLR)) : (void)0;
			IsInBoundGlobal(pos + vecs::Vec3(0, 1, 0)) ? (m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 1, 0), DefaultWidth, DefaultHeight)].GetID() == block::AIR ? (void)0 : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 1, 0), DefaultWidth, DefaultHeight)].RemoveOcclState(block::Block::OCCLD)) : (void)0;
			IsInBoundGlobal(pos + vecs::Vec3(0, -1, 0)) ? (m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, -1, 0), DefaultWidth, DefaultHeight)].GetID() == block::AIR ? (void)0 : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, -1, 0), DefaultWidth, DefaultHeight)].RemoveOcclState(block::Block::OCCLU)) : (void)0;
			IsInBoundGlobal(pos + vecs::Vec3(0, 0, 1)) ? (m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 0, 1), DefaultWidth, DefaultHeight)].GetID() == block::AIR ? (void)0 : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 0, 1), DefaultWidth, DefaultHeight)].RemoveOcclState(block::Block::OCCLB)) : (void)0;
			IsInBoundGlobal(pos + vecs::Vec3(0, 0, -1)) ? (m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 0, -1), DefaultWidth, DefaultHeight)].GetID() == block::AIR ? (void)0 : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 0, -1), DefaultWidth, DefaultHeight)].RemoveOcclState(block::Block::OCCLF)) : (void)0;
		}
		else if(!blockLibrary.GetBlockData(b->GetID()).IsTransparent())
		{
			b->SetOcclState(block::Block::ALL);
			IsInBoundGlobal(pos + vecs::Vec3(1, 0, 0)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(1, 0, 0), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? b->RemoveOcclState(block::Block::OCCLR) : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(1, 0, 0), DefaultWidth, DefaultHeight)].AddOcclState(block::Block::OCCLL)) : b->RemoveOcclState(block::Block::OCCLR);
			IsInBoundGlobal(pos + vecs::Vec3(-1, 0, 0)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(-1, 0, 0), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? b->RemoveOcclState(block::Block::OCCLL) : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(-1, 0, 0), DefaultWidth, DefaultHeight)].AddOcclState(block::Block::OCCLR)) : b->RemoveOcclState(block::Block::OCCLL);
			IsInBoundGlobal(pos + vecs::Vec3(0, 1, 0)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 1, 0), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? b->RemoveOcclState(block::Block::OCCLU) : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 1, 0), DefaultWidth, DefaultHeight)].AddOcclState(block::Block::OCCLD)) : b->RemoveOcclState(block::Block::OCCLU);
			IsInBoundGlobal(pos + vecs::Vec3(0, -1, 0)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, -1, 0), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? b->RemoveOcclState(block::Block::OCCLD) : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, -1, 0), DefaultWidth, DefaultHeight)].AddOcclState(block::Block::OCCLU)) : b->RemoveOcclState(block::Block::OCCLD);
			IsInBoundGlobal(pos + vecs::Vec3(0, 0, 1)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 0, 1), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? b->RemoveOcclState(block::Block::OCCLF) : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 0, 1), DefaultWidth, DefaultHeight)].AddOcclState(block::Block::OCCLB)) : b->RemoveOcclState(block::Block::OCCLF);
			IsInBoundGlobal(pos + vecs::Vec3(0, 0, -1)) ? (blockLibrary.GetBlockData(m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 0, -1), DefaultWidth, DefaultHeight)].GetID()).IsTransparent() ? b->RemoveOcclState(block::Block::OCCLB) : m_blockArray[mathlib::To1D(pos - m_pos + vecs::Vec3(0, 0, -1), DefaultWidth, DefaultHeight)].AddOcclState(block::Block::OCCLF)) : b->RemoveOcclState(block::Block::OCCLB);
		}
		else
			b->SetOcclState(block::Block::NONE);

		ComputeSkyOcclusion(blockLibrary);
		m_dirtyVBOsAndVector = true;
	}
}
void Chunk::GenerateVAO()
{
	if (m_vao)
		glDeleteVertexArrays(1, &m_vao);
	if (m_vbo)
		glDeleteBuffers(1, &m_vbo);
	if (m_scaleVbo)
		glDeleteBuffers(1, &m_scaleVbo);
	if (m_texVBO)
		glDeleteBuffers(1, &m_texVBO);
	if (m_texVBO2)
		glDeleteBuffers(1, &m_texVBO2);
	/*if (m_lightVBO)
		glDeleteBuffers(1, &m_lightVBO);
	if (m_lightVBO2)
		glDeleteBuffers(1, &m_lightVBO2);*/

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glEnableVertexAttribArray(0);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VBO.size(), VBO.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glGenBuffers(1, &m_texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_texVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned char) * TEXVBO.size(), TEXVBO.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glGenBuffers(1, &m_texVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, m_texVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned char) * TEXVBO2.size(), TEXVBO2.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(3);
	glGenBuffers(1, &m_scaleVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_scaleVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned char) * SCALEVBO.size(), SCALEVBO.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	/*glEnableVertexAttribArray(4);
	glGenBuffers(1, &m_lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned char) * LIGHTVBO.size(), LIGHTVBO.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(5);
	glGenBuffers(1, &m_lightVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, m_lightVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned char) * LIGHTVBO2.size(), LIGHTVBO2.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(5, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_dirtyVBOs = false;
	m_dirtyVBOsAndVector = false;
}

void Chunk::GenerateVector(const block::BlockLibrary& blockLib)
{
	VBO.clear();
	SCALEVBO.clear();
	TEXVBO.clear();
	TEXVBO2.clear();
	/*LIGHTVBO.clear();
	LIGHTVBO2.clear();*/
	ComputeSkyOcclusion(blockLib);
	int nb = 0;
	block::Block same[1];
	vecs::Vec3 multiplier;
	vecs::Vec3 coord;
	vecs::Vec3 coord2;
	uint16_t id;
	same[0] = m_blockArray[0];
	unsigned char lightlevel[6];
	unsigned char lighttemp[6];
	ComputeLightLevel(0, lightlevel, blockLib);
	for (int X = 0; X < DefaultWidth; ++X)
	{
		for (int Z = 0; Z < DefaultDepth; ++Z)
		{
			for (int Y = 0; Y < DefaultHeight; ++Y)
			{
				int i = mathlib::To1D(vecs::Vec3(X, Y, Z), DefaultWidth, DefaultHeight);
				if(m_blockArray[i].GetOCCLState() != same[0].GetOCCLState())
					ComputeLightLevel(i, lighttemp, blockLib);
				if (m_blockArray[i] == same[0] && lighttemp[block::BlockData::E_FACE::TOP] == lightlevel[block::BlockData::E_FACE::TOP] &&
					lighttemp[block::BlockData::E_FACE::DOWN] == lightlevel[block::BlockData::E_FACE::DOWN] &&
					lighttemp[block::BlockData::E_FACE::LEFT] == lightlevel[block::BlockData::E_FACE::LEFT] &&
					lighttemp[block::BlockData::E_FACE::RIGHT] == lightlevel[block::BlockData::E_FACE::RIGHT] &&
					lighttemp[block::BlockData::E_FACE::FORWARD] == lightlevel[block::BlockData::E_FACE::FORWARD] &&
					lighttemp[block::BlockData::E_FACE::BACKWARD] == lightlevel[block::BlockData::E_FACE::BACKWARD])
				{
					++nb;
				}
				else
				{
					if (same[0].GetID() != block::AIR && !same[0].IsOccludedAll())
					{
						mathlib::To3DYFirst(nb, DefaultWidth, DefaultHeight, multiplier);
						multiplier += vecs::Vec3(1.0f, 1.0f, 1.0f);
						SCALEVBO.push_back((unsigned char)1);
						SCALEVBO.push_back((unsigned char)multiplier.y);
						SCALEVBO.push_back((unsigned char)1);
						coord = vecs::Vec3(X, Y - 1, Z);
						coord2 = vecs::Vec3(X, Y - 1 - nb, Z);
						coord = coord + mathlib::Direction(coord, coord2) * 0.5f + m_pos;
						VBO.push_back(coord.x);
						VBO.push_back(coord.y);
						VBO.push_back(coord.z);
						VBO.push_back(float(same[0].GetOCCLState()));
						const block::BlockData& b = blockLib.GetBlockData(same[0].GetID());
						TEXVBO.push_back((unsigned char)b.GetTextureID(block::BlockData::TOP));
						TEXVBO.push_back((unsigned char)b.GetTextureID(block::BlockData::DOWN));
						TEXVBO.push_back((unsigned char)b.GetTextureID(block::BlockData::LEFT));
						TEXVBO2.push_back((unsigned char)b.GetTextureID(block::BlockData::RIGHT));
						TEXVBO2.push_back((unsigned char)b.GetTextureID(block::BlockData::FORWARD));
						TEXVBO2.push_back((unsigned char)b.GetTextureID(block::BlockData::BACKWARD));
						/*LIGHTVBO.push_back(lightlevel[0]);
						LIGHTVBO.push_back(lightlevel[1]);
						LIGHTVBO.push_back(lightlevel[2]);
						LIGHTVBO2.push_back(lightlevel[3]);
						LIGHTVBO2.push_back(lightlevel[4]);
						LIGHTVBO2.push_back(lightlevel[5]);*/
					}
					nb = 0;
					same[0] = m_blockArray[i];
					memcpy(lightlevel, lighttemp, 6);
				}
			}
		}
	}
	if (nb != 0 && same[0].GetID() != block::AIR && !same[0].IsOccludedAll())
	{
		id = same[0].GetID();
		mathlib::To3DYFirst(nb, DefaultWidth, DefaultHeight, multiplier);
		multiplier += vecs::Vec3(1.f, 1.f, 1.f);
		SCALEVBO.push_back((unsigned char)1);
		SCALEVBO.push_back((unsigned char)multiplier.y);
		SCALEVBO.push_back((unsigned char)1);
		coord = vecs::Vec3(DefaultWidth - 1, DefaultHeight - 1, DefaultDepth - 1);
		coord2 = vecs::Vec3(DefaultWidth - 1, DefaultHeight - 1 - nb, DefaultDepth - 1);
		coord = coord + mathlib::Direction(coord, coord2) * 0.5f + m_pos;
		VBO.push_back(coord.x);
		VBO.push_back(coord.y);
		VBO.push_back(coord.z);
		VBO.push_back(float(same[0].GetOCCLState()));
		TEXVBO.push_back((unsigned char)blockLib.GetBlockData(same[0].GetID()).GetTextureID(block::BlockData::TOP));
		TEXVBO.push_back((unsigned char)blockLib.GetBlockData(same[0].GetID()).GetTextureID(block::BlockData::DOWN));
		TEXVBO.push_back((unsigned char)blockLib.GetBlockData(same[0].GetID()).GetTextureID(block::BlockData::LEFT));
		TEXVBO2.push_back((unsigned char)blockLib.GetBlockData(same[0].GetID()).GetTextureID(block::BlockData::RIGHT));
		TEXVBO2.push_back((unsigned char)blockLib.GetBlockData(same[0].GetID()).GetTextureID(block::BlockData::FORWARD));
		TEXVBO2.push_back((unsigned char)blockLib.GetBlockData(same[0].GetID()).GetTextureID(block::BlockData::BACKWARD));
	}
	VBO.shrink_to_fit();
	SCALEVBO.shrink_to_fit();
	TEXVBO.shrink_to_fit();
	TEXVBO2.shrink_to_fit();
	/*LIGHTVBO.shrink_to_fit();
	LIGHTVBO2.shrink_to_fit();*/
}

void Chunk::RegenerateVao(const block::BlockLibrary& blockLib)
{
	GenerateVector(blockLib);
	GenerateVAO();
}

bool Chunk::LoadChunkFromRegion(const block::BlockLibrary& blockLib)
{
	int posZ = floor(m_pos.z / (DefaultDepth * WIDTH_DEPTH_REGION_FILE)), posX = floor(m_pos.x / (DefaultWidth * WIDTH_DEPTH_REGION_FILE));
	std::string File = "r-" + std::to_string(posX) + std::to_string(posZ) + RegionDataExtension;
	std::stringstream file(std::fstream::binary | std::fstream::in | std::fstream::out);
	std::ifstream f((world::World::GetSaveFolder() + world::World::GetMapName() + RegionDataFolder + File).c_str(), std::ifstream::binary);
	file << f.rdbuf();
	f.close();

	std::string posChunk;
	posChunk += std::to_string((int)m_pos.x) + std::to_string((int)m_pos.z);
	int nbChunk = 0;
	std::string str;
	bool exist = false;
	while (file >> str)
	{
		if (str == "ChunkList")
		{
			JumpToNext(file);
			while (file >> str)
			{
				if (str == posChunk)
				{
					JumpToNext(file);
					file >> m_biomeID;
					JumpToNext(file);
					file >> nbChunk;
					exist = true;
					break;
				}
			}
			break;
		}
	}
	if (exist)
	{
		file.clear();
		int length = 0;
		for (int nb = 0, seekNB = 0; nb < nbChunk;)
		{
			file >> str;
			if (str == "[BEGIN]")
			{
				file.seekg(1, file.cur);
				file.read((char*)&seekNB, sizeof(int));
				file.seekg(seekNB, file.cur);
				++nb;
			}
		}
		std::string str("[BEGIN]");
		FindSTR(file, str);
		file.seekg(1, file.cur);
		file.read((char*)&length, sizeof(int));
		file.seekg(1, file.cur);
		length = DefaultHeight * DefaultDepth * DefaultWidth;
		uint16_t id = 0;
		for (int temp = 0, nb = 0; nb < length;)
		{
			file.read((char*)&temp, sizeof(int));
			file.read((char*)&id, sizeof(uint16_t));
			for (int i2 = 0; i2 < temp && nb < length; ++nb, ++i2)
			{
				m_blockArray[nb] = blockLib.GetBlock(id);
			}
		}
	}
	return exist;
}


void Chunk::SaveChunkInRegion()
{
	m_saveMutex.lock();
	CheckIfFolderExist((world::World::GetSaveFolder() + world::World::GetMapName() + RegionDataFolder).c_str());
	m_saveMutex.unlock();
	int posZ = floor(m_pos.z / (DefaultDepth * WIDTH_DEPTH_REGION_FILE)), posX = floor(m_pos.x / (DefaultWidth * WIDTH_DEPTH_REGION_FILE));
	std::string File = "r-" + std::to_string(posX) + std::to_string(posZ) + RegionDataExtension;
	if (IsThisFileFree(File))
	{
		OpenFile.load(std::memory_order_acquire)->push_back(File);
		m_saveMutex.unlock();
	}
	else
	{
		while (!IsThisFileFree(File));
		OpenFile.load(std::memory_order_acquire)->push_back(File);
		m_saveMutex.unlock();
	}
	std::stringstream file(std::fstream::binary | std::fstream::in | std::fstream::out);
	if (_access((world::World::GetSaveFolder() + world::World::GetMapName() + RegionDataFolder + File).c_str(), 0) & -1)
		InitRegionFile(file);
	else
	{
		std::ifstream f((world::World::GetSaveFolder() + world::World::GetMapName() + RegionDataFolder + File).c_str(), std::fstream::binary);
		file << f.rdbuf();
		f.close();
	}
	AddChunkToFile(file);
	file.clear();
	ResetCursorPos(file);
	std::ofstream f((world::World::GetSaveFolder() + world::World::GetMapName() + RegionDataFolder + File).c_str(), std::ofstream::binary, std::ofstream::trunc);
	f << file.rdbuf();
	f.close();
	m_saveMutex.lock();
	OpenFile.load(std::memory_order_acquire)->erase(std::find(OpenFile.load(std::memory_order_acquire)->begin(), OpenFile.load(std::memory_order_acquire)->end(), File));
	m_saveMutex.unlock();
}

void Chunk::AddChunkToFile(std::stringstream& file)
{
	int length = (DefaultWidth * DefaultDepth * DefaultHeight);
	int nbChunk = 0;
	std::string str;
	bool chunkListUpdated = false;
	bool exist = false;
	while (file >> str)
	{
		if (str == "ChunkList" && !chunkListUpdated)
		{
			exist = UpdateChunkList(file, nbChunk);
			chunkListUpdated = true;
		}
		else if (str == "[CHUNKDATA]")
		{
			std::stringstream buffer(std::fstream::binary | std::fstream::in | std::fstream::out);
			file.clear();
			int seekNB = 0;
			for (int nb = 0; nb < nbChunk;)
			{
				file >> str;
				if (str == "[BEGIN]")
				{
					file.seekg(1, file.cur);
					file.read((char*)&seekNB, sizeof(int));
					file.seekg(seekNB, file.cur);
					++nb;
				}
			}
			file.seekp((int)file.tellg(), file.beg);

			if (exist)
			{
				std::string str("[BEGIN]");
				FindSTR(file, str);
				file.seekg(1, file.cur);
				file.read((char*)&seekNB, sizeof(int));
				file.seekg(seekNB, file.cur);
				buffer << file.rdbuf();
			}
			file.clear();
			file << "\n[BEGIN] ";
			int pos = (int)file.tellp();
			file << "0000\n";
			int posBeg = (int)file.tellp();
			uint16_t same[1];
			same[0] = m_blockArray[0].GetID();
			int nb = 1;
			for (int i = 1; i < length; ++i)
			{
				if (m_blockArray[i].GetID() == same[0])
				{
					++nb;
				}
				else
				{
					file.write((const char*)&nb, sizeof(int));
					file.write((const char*)&same[0], sizeof(uint16_t));
					nb = 1;
					same[0] = m_blockArray[i].GetID();
				}
			}
			if (nb != 1)
			{
				file.write((const char*)&nb, sizeof(int));
				file.write((const char*)&same[0], sizeof(uint16_t));
			}
			file << "\n[END]\n";
			int tot = (int)file.tellp() - posBeg;
			if (exist)
				file << buffer.rdbuf();
			file.seekp(pos, file.beg);
			file.write((const char*)&tot, sizeof(int));
			break;
		}
	}
}
void Chunk::InitRegionFile(std::stringstream& file)
{
	file << "[METADATA]\n";
	file << "ChunkNumber -> ";
	int nb = -1;
	file.write((const char*)&nb, sizeof(int));
	file << "\n";
	file << "ChunkList -> \n";
	file << "[CHUNKDATA]";
}

bool Chunk::UpdateChunkList(std::stringstream& file, int& nbChunk)
{
	std::string posChunk;
	posChunk += std::to_string((int)m_pos.x) + std::to_string((int)m_pos.z);
	JumpToNext(file);
	int pos = (int)file.tellg();
	bool exist = false;
	std::string str;
	while (file >> str)
	{
		if (str == posChunk)
		{
			JumpToNext(file);
			JumpToNext(file);
			JumpToNext(file);
			file >> nbChunk;
			exist = true;
			break;
		}
	}
	file.clear();
	ResetCursorPos(file);
	if (!exist)
	{
		UpdateChunkNumber(file, nbChunk);
		ResetCursorPos(file);
		str = file.str();
		str.erase(str.begin(), str.begin() + pos + 1);
		std::string temp = std::string("[ ") + posChunk + std::string(" , ") + (char)m_biomeID + std::string(" , ") + std::to_string(nbChunk) + std::string(" ]");
		temp += str;
		file.clear();
		file.seekp(pos + 1);
		file.write(temp.c_str(), sizeof(char) * temp.length());
	}
	return exist;
}

void Chunk::UpdateChunkNumber(std::stringstream& file, int& nbChunk)
{
	ResetCursorPos(file);
	JumpToNext(file);
	JumpToNext(file);
	JumpToNext(file);
	file.seekg(1, file.cur);
	int pos = (int)file.tellg();
	file.read((char*)&nbChunk, sizeof(int));
	++nbChunk;
	file.seekp(pos, file.beg);
	file.write((const char*)&nbChunk, sizeof(int));
}

bool Chunk::IsThisFileFree(const std::string& file)
{
	bool res = false;
	m_saveMutex.lock();
	res = std::find(OpenFile.load(std::memory_order_acquire)->begin(), OpenFile.load(std::memory_order_acquire)->end(), file) == OpenFile.load(std::memory_order_acquire)->end();
	if (!res)
		m_saveMutex.unlock();
	return res;
}

bool Chunk::IsInBound(const vecs::Vec3& pos)
{
	return (pos.x < DefaultWidth &&  pos.x >= 0) &&
		(pos.y < DefaultHeight &&  pos.y >= 0) &&
		(pos.z < DefaultDepth &&  pos.z >= 0);
}

bool Chunk::IsInBoundGlobal(const vecs::Vec3 & pos)
{
	return (pos.x < DefaultWidth + m_pos.x &&  pos.x >= m_pos.x) &&
		(pos.y < DefaultHeight + m_pos.y &&  pos.y >= m_pos.y) &&
		(pos.z < DefaultDepth + m_pos.z &&  pos.z >= m_pos.z);
}
