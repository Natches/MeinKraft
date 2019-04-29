#pragma once

#include <atomic>
#include "GL/glew.h"
#include "Vec3.h"
#include "BiomeLibrary.h"
#include "BlockLibrary.h"
#include "World.h"

class CubeShader;
struct ShaderParameter;


class Chunk
{
public:
	Chunk() = delete;
	Chunk(vecs::Vec3& Pos, uint8_t BiomeId);
	~Chunk();

	void CreateChunk(const biomes::BiomeLibrary& biomeLib, const block::BlockLibrary& blockLib, world::World& world);
	void Render(const block::BlockLibrary& blockLib, CubeShader& shader, const ShaderParameter& param, world::World& w);
	block::Block* GetBlock(const vecs::Vec3& pos);
	bool IsInBound(const vecs::Vec3& pos);
	bool IsInBoundGlobal(const vecs::Vec3& pos);

	inline vecs::Vec3 GetPos() { return m_pos; }
	inline void SetDirtyVBOs(bool b) { m_dirtyVBOsAndVector = b; }
	inline bool IsGen() { return m_gen; }

	void BlockOcclusionCheck(const vecs::Vec3& pos, const block::BlockLibrary& blockLibrary);
	void SaveChunkInRegion();
	static inline void DeleteOpenFile() { delete OpenFile.load(std::memory_order_acquire); }

	inline void SetPos(vecs::Vec3& pos) { m_pos = pos; }

private:
	void ParseHeightMapData(unsigned char(*heightMap)[Zmax], const biomes::BiomeLibrary& biomeLib, const block::BlockLibrary& blockLib);
	void InitBlockToSort(std::pair<uint16_t, float>* sortedAvailableBlock, const biomes::BiomeLibrary& biomeLib, const block::BlockLibrary& blockLib);
	void SortAvailableBlock(int y, std::pair<uint16_t, float>* sortedAvailableBlock, const biomes::BiomeLibrary& biomeLib, const block::BlockLibrary& blockLib);
	void GenerateVAO();
	void GenerateVector(const block::BlockLibrary& blockLib);
	void RegenerateVao(const block::BlockLibrary& blockLib);
	bool LoadChunkFromRegion(const block::BlockLibrary& blockLib);
	void AddChunkToFile(std::stringstream& file);
	void InitRegionFile(std::stringstream& file);
	bool UpdateChunkList(std::stringstream& file, int& nbChunk);
	void UpdateChunkNumber(std::stringstream& file, int& nbChunk);
	bool IsThisFileFree(const std::string& file);
	void OcclusionCheck(const block::BlockLibrary& blockLibrary, world::World& world);
	void ComputeSkyOcclusion(const block::BlockLibrary& blockLibrary);
	void ComputeLightLevel(int index, unsigned char temp[6], const block::BlockLibrary& blockLibrary);

private:

	vecs::Vec3 m_pos;

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_scaleVbo;
	GLuint m_texVBO;
	GLuint m_texVBO2;
	/*GLuint m_lightVBO;
	GLuint m_lightVBO2;*/

	std::vector<float> VBO;
	std::vector<unsigned char> SCALEVBO;
	std::vector<unsigned char> TEXVBO;
	std::vector<unsigned char> TEXVBO2;

	unsigned sizeVbo, scaleVbo, textureVbo, textureVbo2;

	/*std::vector<unsigned char> LIGHTVBO;
	std::vector<unsigned char> LIGHTVBO2;*/

	block::Block m_blockArray[DefaultDepth * DefaultWidth * DefaultHeight];

	bool m_dirtyVBOsAndVector;
	bool m_dirtyVBOs;
	bool m_gen;
	uint8_t m_biomeID;

private:
	static std::mutex    m_saveMutex;
	static std::atomic<std::vector<std::string>*> OpenFile;
	static const char*   RegionDataFolder;
	static const char*   RegionDataExtension;

};

