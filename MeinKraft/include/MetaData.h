#pragma once
class MetaData
{
#pragma region Non-Static Attributes
protected:
	bool m_isActive;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
protected:
	MetaData() { m_isActive = true; }
	virtual ~MetaData() = default;
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
protected:
	inline bool GetIsActive()						{ return m_isActive; }
	inline void SetIsActive(const bool isActive)	{ m_isActive = isActive; }
#pragma endregion
};