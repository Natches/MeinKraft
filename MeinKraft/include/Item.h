#pragma once
class Item
{
#pragma region Non-Static Attributes
	float	m_XPosInTexture;
	float	m_YPosInTexture;
	char*	m_name;
	char	m_ID;
#pragma endregion
#pragma region Non-Static Methods
public:
	Item() = default;
	Item(float X, float Y, char ID, char* name);
	~Item();
#pragma endregion
#pragma region Getters & Setters
	inline float GetX() { return m_XPosInTexture; }
	inline float GetY() { return m_YPosInTexture; }
	inline char* GetName() { return m_name; }
#pragma endregion
};

