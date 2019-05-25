#pragma once
#include <GL\glew.h>
#include "Inputs.h"
#include "ItemLibrary.h"
#include "InventorySlot.h"
#include <iostream>
#include <vector>


class Text;
class Font;
struct ItemStack;
class GUI;

class Inventory
{
#pragma region Non-Static Attributes
	ItemLibrary								m_itemLibrary;
	std::vector<InventorySlot*>				m_inventorySlots;
	std::vector<GLfloat>					m_pickedUpVERTEXDATA;
	std::vector<GLfloat>					m_pickedUpUVDATA;
	std::vector<GLfloat>					m_selectedVERTEXDATA;
	std::vector<GLfloat>					m_selectedUVDATA;
	std::vector<InventorySlot*>				m_hotbar;
	std::map<uint16_t, std::vector<int>>	slotsWithSameItemID;
	Text*									m_itemDescription;
	ItemStack*								m_pickedUpStack;
	char									m_selectedHotbarSlotID;
	bool									m_isDirty;
	bool									m_isDirtyItemDiscription;
	bool									m_isDirtyPickedItem;
	bool									m_isMouseTouchingItem;
	bool									m_visible;
#pragma endregion
#pragma region Non-Static Methods
public:
	Inventory(Font * font);
	~Inventory();
	ItemStack* ShowItemName();
	void TakeItem();
	void TakeSplitStack();
	void Update();
	bool AddItem(ItemStack *stack);
	void ToggleInventory();
	inline void SelectHotbar(int item) 
	{ 
		m_selectedHotbarSlotID = item; 
		m_hotbar[m_selectedHotbarSlotID]->CalculateSelectedVertexAndUVData(m_selectedVERTEXDATA, m_selectedUVDATA);
		m_isDirty = true;
	}

	void DropOneItemInSlot(int id);
	std::string SaveData();
	void LoadData(std::string& s, unsigned int idx);
private:
	void BuildPickedUpItemBufferData(vecs::Vec2 mousePos);
#pragma endregion
#pragma region Getters & Setters
public:
	inline std::vector<InventorySlot*>& GetInventorySlot() { return m_inventorySlots; }
	inline std::vector<InventorySlot*>& GetHotbar() { return m_hotbar; }
	inline char GetSelectedHotbarSlotID() { return m_selectedHotbarSlotID; }
	inline ItemStack*	GetPickedUpStack() { return m_pickedUpStack; }
	inline std::vector<GLfloat>& GetPickUpItemVerticies() { return m_pickedUpVERTEXDATA; }
	inline std::vector<GLfloat>& GetPickUpItemUVs() { return m_pickedUpUVDATA; }
	inline std::vector<GLfloat>& GetSelectedVerticies() { return m_selectedVERTEXDATA; }
	inline std::vector<GLfloat>& GetSelectedUVs() { return m_selectedUVDATA; }
	inline bool IsDirty() { return m_isDirty; }
	inline bool IsVisible() { return m_visible; }
	inline bool IsItemDescriptionDirty() { return m_isDirtyItemDiscription; }
	inline bool IsPickedItemImageDirty() { return m_isDirtyPickedItem; }
	inline void SetIsDirtyPickedItem(bool value) { m_isDirtyPickedItem = value; }
	inline bool IsMouseTouchingItem() { return m_isMouseTouchingItem; }
	inline void SetIsItemDescriptionDirty(bool val) { m_isDirtyItemDiscription = val; }
	inline void SetIsDirty(bool val) { m_isDirty = val; }
	inline Text* GetItemDescription() { return m_itemDescription; }
#pragma endregion
};

