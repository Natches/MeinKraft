#pragma once
#include "Vec2.h"
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#define ACCROSSINTEXTURE 0
#define DOWNINTEXTURE 0
#define POINTS 6

struct ItemStack;
struct ItemLibrary;
class Text;
class Font;

class InventorySlot
{
#pragma region Non-Static Attributes
	std::vector<GLfloat>	m_vertexData;
	std::vector<GLfloat>	m_UVData;
	std::vector<GLfloat>	m_ItemVertexData;
	std::vector<GLfloat>	m_ItemUVData;
	vecs::Vec2				m_position;
	vecs::Vec2				m_dimensions;
	Text*					m_text;
	ItemLibrary*			m_library;
	ItemStack*				m_itemStack;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	InventorySlot() = delete;
	InventorySlot(vecs::Vec2 pos, vecs::Vec2 dimensions, ItemStack * itemStack, ItemLibrary * lib, Font * font);
	~InventorySlot();
	bool mouseCollision(vecs::Vec2 clickPos);
	void CalculateItemVertexAndUVData();
	void CalculateSelectedVertexAndUVData(std::vector<GLfloat>& vertexData, std::vector<GLfloat>& UVData);
	void UpdateItemCountText();
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
	inline int GetPoints() { return POINTS; }
	inline std::vector<GLfloat> GetVertex() { return m_vertexData; }
	inline std::vector<GLfloat> GetUVData() { return m_UVData; }
	inline std::vector<GLfloat> GetItemVertex() { return m_vertexData; }
	inline std::vector<GLfloat> GetItemUVData() { return m_ItemUVData; }
	inline ItemStack* GetItemStack() { return m_itemStack; }
	inline Text*	GetText() { return m_text; }
	void SetItemStack(ItemStack * itemStack);
#pragma endregion
};

