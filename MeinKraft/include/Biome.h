#pragma once

#include <map>
#include "Block.h"

namespace biomes
{
	class Biome
	{
	public:
		Biome(uint8_t ID) : m_id(ID) {};
		~Biome() = default;

		inline uint8_t GetID() { return m_id; }

	private:
		uint8_t m_id;
	};

	class BiomeData
	{
		friend class BiomeLibrary;
	public:
		inline unsigned short GetID() const { return m_id; }
		inline unsigned char GetMaxHeight() const { return m_maxHeight; }
		inline unsigned char GetMinHeightX()const { return m_minHeightX; }
		inline unsigned char GetMinHeightZ()const { return m_minHeightZ; }
		inline uint16_t GetLengthBlocklist()const { return (uint16_t)m_block.size(); }
		const std::map<uint16_t, std::pair<block::Block, float>>& GetBlock()const;
		block::Block  GetBlock(const uint16_t ID)const;
		std::pair<block::Block, float> GetPair(const uint16_t ID)const;
		uint16_t* GetBlockIDList()const;
		block::Block  GetRandomBlock()const;
		float GetBlockChanceMultiplier(const uint16_t ID)const;
	
	private:
		std::map<uint16_t, std::pair<block::Block, float>>  m_block;
		unsigned char m_id;
		unsigned char m_maxHeight;
		unsigned char m_minHeightX;
		unsigned char m_minHeightZ;
	};
}
