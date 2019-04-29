#pragma once
#include <mutex>
#include <random>
#include "gl/glew.h"
#include "BlockLibrary.h"
#include "ShaderLib.h"
#include "BiomeLibrary.h"
#include "CapsuleCollider.h"
#include "Ray.h"
#include "SoundController.h"

#define HEIGHT_OFFSET 1u
#define TIME_BEFORE_GC 120 //in Generation Call
#define MULTIPLIER_BEHIND_US_FOR_RENDERING 3

#define WIDTH_DEPTH_REGION_FILE 3
//#define WIDTH_DEPTH_REGION_HEIGHTMAP 6
#define MAX_LIGTH_LEVEL 5
#define MIN_LIGTH_LEVEL 1

constexpr int DefaultDepth = 128;
constexpr int DefaultWidth = 128;
constexpr int DefaultHeight = 172;
constexpr int Xmax = DefaultWidth * WIDTH_DEPTH_REGION_FILE;
constexpr int Zmax = DefaultDepth * WIDTH_DEPTH_REGION_FILE;

struct ShaderParameter;
class Chunk;

namespace world
{
	class World final
	{
	public:
		World();
		~World();

		Shader*					GetShader(const std::string& name);
		void					RenderChunk(const ShaderParameter& param);
		block::Block*			GetBlock(const vecs::Vec3& pos);
		Chunk*					GetChunk(const vecs::Vec3& Pos);
		std::vector<vecs::Vec3> GetBlocksAround(const vecs::Vec3& pos, const unsigned int range = 3u);
		void					ModifyBlock(const vecs::Vec3& pos, const uint16_t& ID);
		inline SoundController&	GetSoundController() { return m_soundController; }

		void UpdateChunkLists(const vecs::Vec2& playerPos);
		inline const block::BlockLibrary& GetBlockLibrary() const { return m_blockLibrary; }
	    void MoveChunkFromGenBufferToChunkBuffer(Chunk* chunk);

		void ReloadShader();
		std::pair<unsigned char(*)[Zmax], uint8_t>& GetHeightMapData(const vecs::Vec2& ChunkPos);
		inline static std::string& GetMapName() { return m_mapName; }
		inline static const char* GetSaveFolder() { return SaveFolder; }
		uint16_t	BreakBlock(const Ray& ray, const float damage);
		bool		FindClosestBlock(const Ray& ray, vecs::Vec3& closestCenter);
		bool		PlaceBlock(const Ray& ray, CapsuleCollider capsule, const uint16_t ID);
		vecs::Vec3	FindDirection(const vecs::Vec3& intersection, const AABBCollider& aabb);

	private:
		void GenerateRegionHeightMap(const vecs::Vec2& pos);
		void BlurHeightMap(unsigned char(*heightMap)[Zmax] , const vecs::Vec2& regionPos);
		void GenerateChunk(const vecs::Vec2& pos);
		void ClearFarChunk(const vecs::Vec2& playerPos);
		void ClearFarRegion(const vecs::Vec2& playerPos);

	private:
		ShaderLib						m_shaderLib;
		biomes::BiomeLibrary			m_biomeLib;
		block::BlockLibrary				m_blockLibrary;
		std::map<int, Chunk*>			m_chunkList;
		std::map<int, Chunk*>			m_bufferChunkList;
		std::map<int, Chunk*>			m_genChunkList;
		std::map<std::string, std::pair<std::pair<unsigned char(*)[Zmax], uint8_t>, vecs::Vec2>> m_regionHeightMaps;
		std::mutex						m_worldMutex;
		std::mutex						m_genMutex;
		std::mt19937					m_random;
		vecs::Vec2						m_previousPlayerPos;
		size_t							m_hashMapName;
		SoundController					m_soundController;

	private:
		static std::string				m_mapName;
		static const char*				SaveFolder;
	};
}

