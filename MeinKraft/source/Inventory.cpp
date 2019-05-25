#include "Inventory.h"
#include "Text.h"
#include "ItemStack.h"
#include "InventorySlot.h"
#include "Font.h"
#include "Game.h"
#include "Inputs.h"
#include <iostream>
#include <string>

#define MAXSTACK 64u
#define PICKEDUP_ITEM_DIMENSION 0.2f

Inventory::Inventory(Font * font)
{
	m_pickedUpStack = nullptr;
	m_itemDescription = new Text("", font, 80.f / 900.f * INISettings::windowHeight);
	m_visible = false;
	float width = 2.f/10.f;
	vecs::Vec2 pos(0 - width *2.5, 1 - (width / 2) * 4);

	vecs::Vec2 size(width* ((float)INISettings::windowHeight / (float)INISettings::windowWidth), width );

		//m_inventorySlots.push_back(new InventorySlot(vecs::Vec2(pos.x + size.x * i, pos.y), vecs::Vec2(size.x, size.y), nullptr, &m_itemLibrary, font));

	//pos.y -= 0.005f +size.y;
	pos.y -= size.y * 5;

	for (int j = 0; j < 9; ++j)
	{
		InventorySlot* s = new InventorySlot(vecs::Vec2(pos.x + size.x * j, pos.y), vecs::Vec2(size.x, size.y), nullptr, &m_itemLibrary, font);
		m_inventorySlots.push_back(s);
		m_hotbar.push_back(s);
	}

	pos.y += width * 5;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			m_inventorySlots.push_back(new InventorySlot(vecs::Vec2(pos.x + size.x * j, pos.y - size.y * i), vecs::Vec2(size.x, size.y), nullptr, &m_itemLibrary, font));
		}
	}

	/*pos.y -= 0.4f;
	for (int j = 0; j < 9; ++j)
	{
		InventorySlot* s = new InventorySlot(vecs::Vec2(pos.x + size.x * j, pos.y - size.y * 3), vecs::Vec2(size.x, size.y), nullptr, &m_itemLibrary, font);
		m_inventorySlots.push_back(s);
		m_hotbar.push_back(s);
	}*/

	m_selectedHotbarSlotID = 0;
	m_hotbar[m_selectedHotbarSlotID]->CalculateSelectedVertexAndUVData(m_selectedVERTEXDATA, m_selectedUVDATA);										   


	for(unsigned int i = 0; i < m_inventorySlots.size(); ++i)
		m_inventorySlots[i]->CalculateItemVertexAndUVData();
	m_isDirty = true;
}

Inventory::~Inventory()
{
	for (unsigned int i = 0; i < m_inventorySlots.size(); ++i)
		delete m_inventorySlots[i];
	delete m_itemDescription;
	delete m_pickedUpStack;
}

ItemStack * Inventory::ShowItemName()
{
	vecs::Vec2 positionOfMouse = Inputs::GetMousePosition();
	positionOfMouse.x -= INISettings::windowWidth / 2.f;
	positionOfMouse.x /= INISettings::windowWidth / 2.f;
	positionOfMouse.y -= INISettings::windowHeight / 2.f;
	positionOfMouse.y /= INISettings::windowHeight / 2.f;
	positionOfMouse.y *= -1;
	ItemStack * IS = nullptr;
	for (unsigned int i = 0; i < m_inventorySlots.size(); ++i)
	{
		if (m_inventorySlots[i]->mouseCollision(positionOfMouse))
		{
			IS = m_inventorySlots[i]->GetItemStack();
			if (IS != nullptr)
				break;
		}
	}
	if (IS != nullptr)
	{
		if (!m_itemLibrary.m_item[IS->m_itemID].GetName())
		{
			m_itemDescription->SetString("UNDEFINED");
			m_isDirtyItemDiscription = true;
			if (m_itemDescription->SetPosition(positionOfMouse))
				m_isDirtyItemDiscription = true;
			m_isMouseTouchingItem = true;
		}
		else
		{
			m_isDirtyItemDiscription = false;
			if (m_itemDescription->SetString(m_itemLibrary.m_item[IS->m_itemID].GetName()))
				m_isDirtyItemDiscription = true;
			if (m_itemDescription->SetPosition(positionOfMouse))
				m_isDirtyItemDiscription = true;
			m_isMouseTouchingItem = true;
		}
	}
	else
		m_isMouseTouchingItem = false;
	return IS;
}

void Inventory::TakeItem()
{
	if (m_visible)
	{
		vecs::Vec2 positionOfMouse = Inputs::GetMousePosition();
		positionOfMouse.x -= INISettings::windowWidth / 2.f;
		positionOfMouse.x /= INISettings::windowWidth / 2.f;
		positionOfMouse.y -= INISettings::windowHeight / 2.f;
		positionOfMouse.y /= INISettings::windowHeight / 2.f;
		positionOfMouse.y *= -1;
		ItemStack * IS = nullptr;
		for (unsigned int i = 0; i < m_inventorySlots.size(); ++i)
		{
			if (m_inventorySlots[i]->mouseCollision(positionOfMouse))
			{
				if (m_pickedUpStack)
				{
					if (m_inventorySlots[i]->GetItemStack())
					{
						if (m_pickedUpStack->m_itemID == m_inventorySlots[i]->GetItemStack()->m_itemID)
						{
							m_inventorySlots[i]->GetItemStack()->m_quantity += m_pickedUpStack->m_quantity;
							if (m_inventorySlots[i]->GetItemStack()->m_quantity > MAXSTACK)
							{
								m_pickedUpStack->m_quantity = m_inventorySlots[i]->GetItemStack()->m_quantity - MAXSTACK;
								m_inventorySlots[i]->GetItemStack()->m_quantity = MAXSTACK;
							}
							else
							{
								m_pickedUpStack = nullptr;
							}
							m_inventorySlots[i]->GetText()->SetString(std::to_string(m_inventorySlots[i]->GetItemStack()->m_quantity));
						}
						else
						{
							IS = m_inventorySlots[i]->GetItemStack();
							m_inventorySlots[i]->SetItemStack(m_pickedUpStack);
							m_pickedUpStack = IS;
						}
					}
					else
					{
						m_inventorySlots[i]->SetItemStack(m_pickedUpStack);
						m_pickedUpStack = nullptr;
					}

				}
				else
				{
					m_pickedUpStack = m_inventorySlots[i]->GetItemStack();
					m_inventorySlots[i]->SetItemStack(nullptr);
				}
				m_isDirty = true;
				if (m_pickedUpStack != nullptr)
				{
					BuildPickedUpItemBufferData(positionOfMouse);
				}
				break;
			}
		}
	}
}

void Inventory::TakeSplitStack()
{
	if (m_visible)
	{
		vecs::Vec2 positionOfMouse = Inputs::GetMousePosition();
		positionOfMouse.x -= INISettings::windowWidth / 2.f;
		positionOfMouse.x /= INISettings::windowWidth / 2.f;
		positionOfMouse.y -= INISettings::windowHeight / 2.f;
		positionOfMouse.y /= INISettings::windowHeight / 2.f;
		positionOfMouse.y *= -1;
		for (unsigned int i = 0; i < m_inventorySlots.size(); ++i)
		{
			if (m_inventorySlots[i]->mouseCollision(positionOfMouse))
			{
				if (m_pickedUpStack)
				{
				}
				else
				{
					if (m_inventorySlots[i]->GetItemStack()->m_quantity > 1)
					{
						int qty = m_inventorySlots[i]->GetItemStack()->m_quantity;
						qty /= 2;
						m_inventorySlots[i]->GetItemStack()->m_quantity -= qty;
						m_pickedUpStack = new ItemStack(m_inventorySlots[i]->GetItemStack()->m_itemID, qty);
						m_isDirty = true;
						m_inventorySlots[i]->GetText()->SetString(std::to_string(m_inventorySlots[i]->GetItemStack()->m_quantity));
						BuildPickedUpItemBufferData(positionOfMouse);
						break;
					}
				}
			}
		}
	}
}

void Inventory::Update()
{
	if (m_visible)
	{
		vecs::Vec2 positionOfMouse = Inputs::GetMousePosition();
		positionOfMouse.x -= INISettings::windowWidth / 2.f;
		positionOfMouse.x /= INISettings::windowWidth / 2.f;
		positionOfMouse.y -= INISettings::windowHeight / 2.f;
		positionOfMouse.y /= INISettings::windowHeight / 2.f;
		positionOfMouse.y *= -1;
		ShowItemName();
		if (Inputs::HasMouseMoved())
		{
			m_pickedUpVERTEXDATA.clear();
			vecs::Vec2 Dimensions(PICKEDUP_ITEM_DIMENSION* ((float)INISettings::windowHeight / (float)INISettings::windowWidth), PICKEDUP_ITEM_DIMENSION);
			m_pickedUpVERTEXDATA.push_back(positionOfMouse.x);
			m_pickedUpVERTEXDATA.push_back(positionOfMouse.y);
			m_pickedUpVERTEXDATA.push_back(positionOfMouse.x);
			m_pickedUpVERTEXDATA.push_back(positionOfMouse.y - Dimensions.y);
			m_pickedUpVERTEXDATA.push_back(positionOfMouse.x + Dimensions.x);
			m_pickedUpVERTEXDATA.push_back(positionOfMouse.y - Dimensions.y);

			m_pickedUpVERTEXDATA.push_back(positionOfMouse.x + Dimensions.x);
			m_pickedUpVERTEXDATA.push_back(positionOfMouse.y - Dimensions.y);
			m_pickedUpVERTEXDATA.push_back(positionOfMouse.x + Dimensions.x);
			m_pickedUpVERTEXDATA.push_back(positionOfMouse.y);
			m_pickedUpVERTEXDATA.push_back(positionOfMouse.x);
			m_pickedUpVERTEXDATA.push_back(positionOfMouse.y);
			m_isDirtyPickedItem = true;
		}
	}
	else
	{
		if (Inputs::GetScrollState() < 0)
		{
			SelectHotbar(m_selectedHotbarSlotID == 8 ? 0 : m_selectedHotbarSlotID + 1);
		}
		else if (Inputs::GetScrollState() > 0)
		{
			SelectHotbar(m_selectedHotbarSlotID == 0 ? 8 : m_selectedHotbarSlotID - 1);
		}
	}
}

bool Inventory::AddItem(ItemStack * stack)
{
	if (slotsWithSameItemID.find(stack->m_itemID) == slotsWithSameItemID.end())
	{
		slotsWithSameItemID[stack->m_itemID] = std::vector<int>();
		for (unsigned int i = 0; i < m_inventorySlots.size(); ++i)
		{
			if (m_inventorySlots[i]->GetItemStack() != nullptr && m_inventorySlots[i]->GetItemStack()->m_itemID == stack->m_itemID && m_inventorySlots[i]->GetItemStack()->m_quantity < MAXSTACK)
				slotsWithSameItemID[stack->m_itemID].push_back(i);
		}
	}
	else
	{
		slotsWithSameItemID[stack->m_itemID].clear();
		for (unsigned int i = 0; i < m_inventorySlots.size(); ++i)
		{
			if (m_inventorySlots[i]->GetItemStack() != nullptr && m_inventorySlots[i]->GetItemStack()->m_itemID == stack->m_itemID && m_inventorySlots[i]->GetItemStack()->m_quantity < MAXSTACK)
				slotsWithSameItemID[stack->m_itemID].push_back(i);
		}
	}

	if (slotsWithSameItemID[stack->m_itemID].size() > 0)
	{
		for (unsigned int i = 0; i < slotsWithSameItemID[stack->m_itemID].size(); ++i)
		{
			m_inventorySlots[slotsWithSameItemID[stack->m_itemID][i]]->GetItemStack()->m_quantity += stack->m_quantity;
			if (m_inventorySlots[slotsWithSameItemID[stack->m_itemID][i]]->GetItemStack()->m_quantity > MAXSTACK)
			{
				stack->m_quantity = m_inventorySlots[slotsWithSameItemID[stack->m_itemID][i]]->GetItemStack()->m_quantity - MAXSTACK;
				m_inventorySlots[slotsWithSameItemID[stack->m_itemID][i]]->GetItemStack()->m_quantity = MAXSTACK;
				m_inventorySlots[slotsWithSameItemID[stack->m_itemID][i]]->GetText()->SetString(std::to_string(m_inventorySlots[slotsWithSameItemID[stack->m_itemID][i]]->GetItemStack()->m_quantity));
				m_isDirty = true;
				slotsWithSameItemID[stack->m_itemID].erase(slotsWithSameItemID[stack->m_itemID].begin() + i);
			}
			else
			{
				m_inventorySlots[slotsWithSameItemID[stack->m_itemID][i]]->GetText()->SetString(std::to_string(m_inventorySlots[slotsWithSameItemID[stack->m_itemID][i]]->GetItemStack()->m_quantity));
				m_isDirty = true;
				delete stack;
				return true;
			}
		}
	}
	for (unsigned int i = 0; i < m_inventorySlots.size(); ++i)
	{
		if (m_inventorySlots[i]->GetItemStack() == nullptr)
		{
			m_inventorySlots[i]->SetItemStack(stack);
			m_inventorySlots[i]->GetText()->SetString(std::to_string(m_inventorySlots[i]->GetItemStack()->m_quantity));
			m_isDirty = true;
			slotsWithSameItemID[stack->m_itemID].push_back(i);
			return true;
		}
	}
	return false;
}

void Inventory::ToggleInventory()
{
	m_visible = !m_visible;
	if (m_visible)
	{
		Inputs::BindInput(GLFW_MOUSE_BUTTON_LEFT, std::bind(&Inventory::TakeItem, this), true);
		Inputs::BindInput(GLFW_MOUSE_BUTTON_RIGHT, std::bind(&Inventory::TakeSplitStack, this), true);
		
	}
	else
	{
		Inputs::CenterMouse();
	}
}

void Inventory::DropOneItemInSlot(int id)
{
	m_hotbar[id]->GetItemStack()->m_quantity -= 1;
	if (m_hotbar[id]->GetItemStack()->m_quantity == 0)
		m_hotbar[id]->SetItemStack(nullptr);
	m_hotbar[id]->CalculateItemVertexAndUVData();
	m_hotbar[id]->UpdateItemCountText();
	m_isDirty = true;
}

std::string Inventory::SaveData()
{
	std::string data;
	for (unsigned int i = 0; i < m_inventorySlots.size(); ++i)
	{
		if (m_inventorySlots[i]->GetItemStack())
		{
			data += std::to_string(m_inventorySlots[i]->GetItemStack()->m_itemID);
			data += ' ';
			data += std::to_string(m_inventorySlots[i]->GetItemStack()->m_quantity);
			data += '\n';
		}
		else
		{
			data += "N";
			data += '\n';
		}
	}
	return data;
}

void Inventory::LoadData(std::string& s, unsigned int idx)
{
	if (s != "N")
	{
		short id = (short)std::stoi(s.substr(0, s.find_first_of(' ')));
		short qty = (short)std::stoi(s.substr(s.find_first_of(' '), s.length() - s.find_first_of(' ')));
		ItemStack* i = new ItemStack(id, qty);
		m_inventorySlots[idx]->SetItemStack(i);
		m_inventorySlots[idx]->CalculateItemVertexAndUVData();
		m_isDirty = true;
	}
}

void Inventory::BuildPickedUpItemBufferData(vecs::Vec2 positionOfMouse)
{
	m_pickedUpVERTEXDATA.clear();
	m_pickedUpUVDATA.clear();
	float widthOfItemInTexture = 1.f / 16.f;
	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetX() / 16.f);
	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetY() / 16.f);

	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetX() / 16.f);
	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetY() / 16.f + widthOfItemInTexture);

	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetX() / 16.f + widthOfItemInTexture);
	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetY() / 16.f + widthOfItemInTexture);

	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetX() / 16.f + widthOfItemInTexture);
	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetY() / 16.f + widthOfItemInTexture);

	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetX() / 16.f + widthOfItemInTexture);
	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetY() / 16.f);

	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetX() / 16.f);
	m_pickedUpUVDATA.push_back(m_itemLibrary.m_item[m_pickedUpStack->m_itemID].GetY() / 16.f);

	vecs::Vec2 Dimensions(PICKEDUP_ITEM_DIMENSION* ((float)INISettings::windowHeight / (float)INISettings::windowWidth), PICKEDUP_ITEM_DIMENSION);
	m_pickedUpVERTEXDATA.push_back(positionOfMouse.x);
	m_pickedUpVERTEXDATA.push_back(positionOfMouse.y);
	m_pickedUpVERTEXDATA.push_back(positionOfMouse.x);
	m_pickedUpVERTEXDATA.push_back(positionOfMouse.y - Dimensions.y);
	m_pickedUpVERTEXDATA.push_back(positionOfMouse.x + Dimensions.x);
	m_pickedUpVERTEXDATA.push_back(positionOfMouse.y - Dimensions.y);

	m_pickedUpVERTEXDATA.push_back(positionOfMouse.x + Dimensions.x);
	m_pickedUpVERTEXDATA.push_back(positionOfMouse.y - Dimensions.y);
	m_pickedUpVERTEXDATA.push_back(positionOfMouse.x + Dimensions.x);
	m_pickedUpVERTEXDATA.push_back(positionOfMouse.y);
	m_pickedUpVERTEXDATA.push_back(positionOfMouse.x);
	m_pickedUpVERTEXDATA.push_back(positionOfMouse.y);
	m_isDirtyPickedItem = true;
}