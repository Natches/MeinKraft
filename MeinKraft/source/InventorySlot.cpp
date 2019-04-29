#include "InventorySlot.h"
#include "ItemStack.h"
#include "ItemLibrary.h"
#include "Text.h"
#include "SOIL.h"
#include "INISettings.h"

InventorySlot::InventorySlot(vecs::Vec2 pos, vecs::Vec2 dimentions, ItemStack* itemStack, ItemLibrary* lib, Font * font) :
	m_position(pos),
	m_dimensions(dimentions),
	m_itemStack(itemStack),
	m_library(lib)
{
	m_text = new Text(itemStack != nullptr ? std::to_string((int)itemStack->m_quantity) : "", font, 100.f/900.f * INISettings::windowHeight, vecs::Vec2(pos.x, pos.y));
	
	m_vertexData.push_back(pos.x);
	m_vertexData.push_back(pos.y);
	m_vertexData.push_back(pos.x);
	m_vertexData.push_back(pos.y - dimentions.y);
	m_vertexData.push_back(pos.x + dimentions.x);
	m_vertexData.push_back(pos.y - dimentions.y);

	m_vertexData.push_back(pos.x + dimentions.x);
	m_vertexData.push_back(pos.y - dimentions.y);
	m_vertexData.push_back(pos.x + dimentions.x);
	m_vertexData.push_back(pos.y);
	m_vertexData.push_back(pos.x);
	m_vertexData.push_back(pos.y);

	float widthOfItemInTexture = 1.f / 16.f;


	m_UVData.push_back(ACCROSSINTEXTURE / 16.f);
	m_UVData.push_back(DOWNINTEXTURE / 16.f);

	m_UVData.push_back(ACCROSSINTEXTURE / 16.f);
	m_UVData.push_back(DOWNINTEXTURE / 16.f + widthOfItemInTexture);

	m_UVData.push_back(ACCROSSINTEXTURE / 16.f + widthOfItemInTexture);
	m_UVData.push_back(DOWNINTEXTURE / 16.f + widthOfItemInTexture);

	m_UVData.push_back(ACCROSSINTEXTURE / 16.f + widthOfItemInTexture);
	m_UVData.push_back(DOWNINTEXTURE / 16.f + widthOfItemInTexture);

	m_UVData.push_back(ACCROSSINTEXTURE / 16.f + widthOfItemInTexture);
	m_UVData.push_back(DOWNINTEXTURE / 16.f);

	m_UVData.push_back(ACCROSSINTEXTURE / 16.f);
	m_UVData.push_back(DOWNINTEXTURE / 16.f);
}

InventorySlot::~InventorySlot()
{
	delete m_text;
	delete m_itemStack;
}

bool InventorySlot::mouseCollision(vecs::Vec2 clickPos)
{
	if (clickPos.x > m_position.x
		&& clickPos.x < m_position.x + m_dimensions.x
		&& clickPos.y < m_position.y 
		&& clickPos.y > m_position.y - m_dimensions.y)
		return true;
	return false;
}

void InventorySlot::CalculateItemVertexAndUVData()
{
	m_ItemVertexData.clear();
	m_ItemUVData.clear();
	if (m_itemStack != nullptr)
	{
		m_ItemVertexData.push_back(m_position.x);
		m_ItemVertexData.push_back(m_position.y);
		m_ItemVertexData.push_back(m_position.x);
		m_ItemVertexData.push_back(m_position.y - m_dimensions.y );
		m_ItemVertexData.push_back(m_position.x + m_dimensions.x);
		m_ItemVertexData.push_back(m_position.y - m_dimensions.y);

		m_ItemVertexData.push_back(m_position.x + m_dimensions.x);
		m_ItemVertexData.push_back(m_position.y - m_dimensions.y);
		m_ItemVertexData.push_back(m_position.x + m_dimensions.x);
		m_ItemVertexData.push_back(m_position.y);
		m_ItemVertexData.push_back(m_position.x);
		m_ItemVertexData.push_back(m_position.y);

		float widthOfItemInTexture = 1.f / 16.f;

		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetX() / 16.f);
		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetY() / 16.f);

		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetX() / 16.f);
		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetY() / 16.f + widthOfItemInTexture);

		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetX() / 16.f + widthOfItemInTexture);
		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetY() / 16.f + widthOfItemInTexture);

		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetX() / 16.f + widthOfItemInTexture);
		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetY() / 16.f + widthOfItemInTexture);

		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetX() / 16.f + widthOfItemInTexture);
		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetY() / 16.f);

		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetX() / 16.f);
		m_ItemUVData.push_back(m_library->m_item[m_itemStack->m_itemID].GetY() / 16.f);
	}
}

void InventorySlot::CalculateSelectedVertexAndUVData(std::vector<GLfloat>& vertexData, std::vector<GLfloat>& UVData)
{
	vertexData.clear();
	UVData.clear();

	vertexData.push_back(m_position.x);
	vertexData.push_back(m_position.y);
	vertexData.push_back(m_position.x);
	vertexData.push_back(m_position.y - m_dimensions.y);
	vertexData.push_back(m_position.x + m_dimensions.x);
	vertexData.push_back(m_position.y - m_dimensions.y);

	vertexData.push_back(m_position.x + m_dimensions.x);
	vertexData.push_back(m_position.y - m_dimensions.y);
	vertexData.push_back(m_position.x + m_dimensions.x);
	vertexData.push_back(m_position.y);
	vertexData.push_back(m_position.x);
	vertexData.push_back(m_position.y);

	float widthOfItemInTexture = 1.f / 16.f;

	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetX() / 16.f);
	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetY() / 16.f);

	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetX() / 16.f);
	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetY() / 16.f + widthOfItemInTexture);

	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetX() / 16.f + widthOfItemInTexture);
	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetY() / 16.f + widthOfItemInTexture);

	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetX() / 16.f + widthOfItemInTexture);
	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetY() / 16.f + widthOfItemInTexture);

	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetX() / 16.f + widthOfItemInTexture);
	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetY() / 16.f);

	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetX() / 16.f);
	UVData.push_back(m_library->m_item[E_ITEMID::OUTLINE].GetY() / 16.f);
}

void InventorySlot::UpdateItemCountText()
{
	if (m_itemStack == nullptr)
	{
		m_text->SetString("");
	}
	else
	{
		m_text->SetString(std::to_string((int)m_itemStack->m_quantity));
	}
}

void InventorySlot::SetItemStack(ItemStack * itemStack)
{
	m_itemStack = itemStack;
	if (itemStack == nullptr)
	{
		m_text->SetString("");
	}
	else
	{
		m_text->SetString(std::to_string((int)itemStack->m_quantity));
	}
	CalculateItemVertexAndUVData();
}
