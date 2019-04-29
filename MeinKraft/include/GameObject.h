#pragma once
#include <string>
#include <vector>
#include "Transform.h"

class GameObject
{
#pragma region Non-Static Attributes
protected:
	Transform				m_transform;
	const __int16			m_ID;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	GameObject() = delete;
	GameObject(const short ID);
	virtual ~GameObject();

	virtual void Update();
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
public:
	inline Transform&	 GetTransform() noexcept { return m_transform; }
	inline const __int16 GetID() const noexcept { return m_ID; }
#pragma endregion
};