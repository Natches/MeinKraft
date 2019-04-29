#pragma once

#include <map>
#include "Block.h"

namespace block
{
	enum E_BLOCKTYPE : uint16_t
	{
		AIR			= uint16_t(-2),
		ERROR		= uint16_t(-1),
		DIRT		= 8191,
		STONE,
		SAND,
		IRON_ORE,
		EMERALD_ORE,
		GOLD_ORE,
		DIAMOND_ORE,
		OBSIDIAN,
		BEDROCK,
		SNOW,
		GRASS,	
		GLASS,
		SNOWYGRASS,
		REDSTONE_ORE,
		COAL_ORE,
		GRAVEL,
		MYCELLIUM
	};

	class BlockLibrary
	{
#pragma region Non-Static Attributes
		std::map<uint16_t, Block> m_block;
		std::map<uint16_t, BlockData> m_blockData;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
	public:
		BlockLibrary() = default;
		~BlockLibrary();

		void InitLibrary(const std::string& path = "Data/Block.data");
	private:
		void LoadBlocks(std::stringstream& fileStream);
		void LoadNextBlock(std::stringstream& fileStream, const std::string& blockRegion);
		void LoadFlags(const std::string& line, BlockData& data);
		void LoadPercents(BlockData& data, std::stringstream& fileStream);
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
	public:
		Block GetBlock(const uint16_t id)const;
		const BlockData& GetBlockData(const uint16_t id)const;
#pragma endregion 
	};
}
