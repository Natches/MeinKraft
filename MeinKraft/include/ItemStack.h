#pragma once
#include <stdint.h>
struct ItemStack
{
#pragma region Non-Static Attributes
	uint16_t		m_itemID;
	unsigned char	m_quantity;
#pragma endregion
#pragma region Non-Static Methods
	ItemStack(uint16_t ID, char Quantity) { m_quantity = Quantity, m_itemID = ID; }
	~ItemStack();
#pragma endregion
};

