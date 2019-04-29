#pragma once
#include "ItemData.h"
#include "BlockLibrary.h"
#include <map>

enum E_ITEMID : uint16_t
{
	OUTLINE = 0,
	L_TOP = 1,
	L_MID = 2,
	L_PANT = 3,
	L_BOT = 4
};

struct ItemLibrary
{
	std::map<uint16_t, ItemData> m_item;
	ItemLibrary();
	~ItemLibrary();
};

