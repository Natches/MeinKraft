#pragma once
#include "MetaData.h"

class GameObject;
class Transform;

class Component : public MetaData
{
#pragma region Non-Static Attributes
protected:
	GameObject& m_gameObject;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	Component(GameObject& gameObject);
	virtual ~Component();

public:
	virtual void Update() = 0;
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
public:
	GameObject&		GetGameObject() noexcept;
	Transform&		GetTransform()	noexcept;
#pragma endregion
};