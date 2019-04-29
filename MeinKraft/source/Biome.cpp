#include "Biome.h"

namespace biomes
{
	const std::map<uint16_t, std::pair<block::Block, float>>& biomes::BiomeData::GetBlock()const
	{
		return m_block;
	}

	block::Block biomes::BiomeData::GetBlock(const uint16_t ID)const
	{
		return (m_block.find(ID) == m_block.end() ? m_block.at(-1).first : m_block.at(ID).first);
	}

	std::pair<block::Block, float> BiomeData::GetPair(const uint16_t ID)const
	{
		return (m_block.find(ID) == m_block.end() ? m_block.at(-1) : m_block.at(ID));
	}

	uint16_t* biomes::BiomeData::GetBlockIDList()const
	{
		uint16_t* r = new uint16_t[m_block.size()];
		unsigned int id = 0;
		for (auto b : m_block)
		{
			r[id] = b.first;
			++id;
		}
		return r;
	}

	block::Block biomes::BiomeData::GetRandomBlock()const
	{
		block::Block b;
		unsigned int dest = rand() % m_block.size(), idx = 0;
		for (auto block : m_block)
		{
			if (idx == dest)
			{
				b = block.second.first;
				break;
			}
		}
		return b;
	}

	float biomes::BiomeData::GetBlockChanceMultiplier(const uint16_t ID)const
	{
		return (m_block.find(ID) == m_block.end() ? 0 : m_block.at(ID).second);
	}
}
