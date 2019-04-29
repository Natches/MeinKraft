#include "ItemLibrary.h"



ItemLibrary::ItemLibrary()
{
	//Leather armour
	m_item.insert(std::make_pair(E_ITEMID::OUTLINE, ItemData(1, 0, "OUTLINE", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(E_ITEMID::L_TOP, ItemData(2, 0, "Rawhide Hat", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(E_ITEMID::L_MID, ItemData(3, 0, "Rawhide Shirt", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(E_ITEMID::L_PANT, ItemData(4, 0, "Rawhide Pants", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(E_ITEMID::L_BOT, ItemData(5, 0, "Rawhide Boots", E_ITEMFLAGS::ARMOUR)));

	m_item.insert(std::make_pair(block::E_BLOCKTYPE::STONE, ItemData(11, 0, "Stone", E_ITEMFLAGS::BLOCK)));
	m_item.insert(std::make_pair(block::E_BLOCKTYPE::DIRT, ItemData(12, 0, "Dirt", E_ITEMFLAGS::BLOCK)));
	m_item.insert(std::make_pair(block::E_BLOCKTYPE::SNOW, ItemData(2, 1, "Snow", E_ITEMFLAGS::BLOCK)));
	m_item.insert(std::make_pair(block::E_BLOCKTYPE::GOLD_ORE, ItemData(15, 0, "Gold", E_ITEMFLAGS::BLOCK)));
	m_item.insert(std::make_pair(block::E_BLOCKTYPE::IRON_ORE, ItemData(14, 0, "Iron", E_ITEMFLAGS::BLOCK)));
	m_item.insert(std::make_pair(block::E_BLOCKTYPE::DIAMOND_ORE, ItemData(0, 1, "Diamond", E_ITEMFLAGS::BLOCK)));
	m_item.insert(std::make_pair(block::E_BLOCKTYPE::OBSIDIAN, ItemData(1, 1, "Obsidian", E_ITEMFLAGS::BLOCK)));
	m_item.insert(std::make_pair(block::E_BLOCKTYPE::GLASS, ItemData(3, 1, "Glass", E_ITEMFLAGS::BLOCK)));
	m_item.insert(std::make_pair(block::E_BLOCKTYPE::GRASS, ItemData(13, 0, "Grass", E_ITEMFLAGS::BLOCK)));
	m_item.insert(std::make_pair(block::E_BLOCKTYPE::AIR, ItemData(6, 1, "Air", E_ITEMFLAGS::BLOCK)));
	/*
	//Iron armour
	m_item.insert(std::make_pair(4, ItemData(2, 0, "Iron Helm", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(5, ItemData(2, 1, "Iron Breastplate", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(6, ItemData(2, 2, "Iron Tasset", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(7, ItemData(2, 3, "Iron Sabatons", E_ITEMFLAGS::ARMOUR)));

	//Gold armour
	m_item.insert(std::make_pair(8, ItemData(4, 0, "Gold Helm", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(9, ItemData(4, 1, "Gold Breastplate", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(10, ItemData(4, 2, "Gold Tasset", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(11, ItemData(4, 3, "Gold Sabatons", E_ITEMFLAGS::ARMOUR)));

	//Diamond armour
	m_item.insert(std::make_pair(12, ItemData(3, 0, "Diamons Helm", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(13, ItemData(3, 1, "Diamons Breastplate", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(14, ItemData(3, 2, "Diamons Tasset", E_ITEMFLAGS::ARMOUR)));
	m_item.insert(std::make_pair(15, ItemData(3, 3, "Diamons Sabatons", E_ITEMFLAGS::ARMOUR)));

	//Wooden tools
	m_item.insert(std::make_pair(16, ItemData(0, 4, "Wooden Club", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(17, ItemData(0, 5, "Wooden Trowel", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(18, ItemData(0, 6, "Wooden Pick", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(19, ItemData(0, 7, "Wooden Axe", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(20, ItemData(0, 8, "Wooden Hoe", E_ITEMFLAGS::TOOL)));

	//Stone tools
	m_item.insert(std::make_pair(21, ItemData(1, 4, "Stone Club", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(22, ItemData(1, 5, "Stone Trowel", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(23, ItemData(1, 6, "Stone Pick", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(24, ItemData(1, 7, "Stone Axe", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(25, ItemData(1, 8, "Stone Hoe", E_ITEMFLAGS::TOOL)));

	//Iron tools
	m_item.insert(std::make_pair(26, ItemData(2, 4, "Iron Blade", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(27, ItemData(2, 5, "Iron Shovel", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(28, ItemData(2, 6, "Iron Pickaxe", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(29, ItemData(2, 7, "Iron Axe", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(30, ItemData(2, 8, "Iron Hoe", E_ITEMFLAGS::TOOL)));

	//Gold tools
	m_item.insert(std::make_pair(31, ItemData(4, 4, "Gold Sword", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(32, ItemData(4, 5, "Gold Shovel", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(33, ItemData(4, 6, "Gold Pickaxe", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(34, ItemData(4, 7, "Gold Axe", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(35, ItemData(4, 8, "Gold Hoe", E_ITEMFLAGS::TOOL)));

	//Diamond tools
	m_item.insert(std::make_pair(36, ItemData(3, 4, "Diamond Sword", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(37, ItemData(3, 5, "Diamond Shovel", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(38, ItemData(3, 6, "Diamond Pickaxe", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(39, ItemData(3, 7, "Diamond Axe", E_ITEMFLAGS::TOOL)));
	m_item.insert(std::make_pair(40, ItemData(3, 8, "Diamond Hoe", E_ITEMFLAGS::TOOL)));
	*/
}


ItemLibrary::~ItemLibrary()
{
	m_item.clear();
}
