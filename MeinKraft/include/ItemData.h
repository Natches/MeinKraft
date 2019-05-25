#pragma once

enum E_ITEMFLAGS
{
	OTHER = 0,
	ARMOUR = 1,
	TOOL = 1 << 1,
	BLOCK = 1 << 2
};

class ItemData
{
#pragma region Non-Static Attributes
	float			m_XPosInTexture;
	float			m_YPosInTexture;
	E_ITEMFLAGS		m_flags;
	const char*			m_name;
#pragma endregion
#pragma region Non-Static Methods
public:
	ItemData() = default;
	ItemData(float X, float Y, const char* name, E_ITEMFLAGS flags);
	~ItemData();
#pragma endregion
#pragma region Getters & Setters
	inline float GetX() { return m_XPosInTexture; }
	inline float GetY() { return m_YPosInTexture; }
	inline const char* GetName() { return m_name; }
#pragma endregion
};

