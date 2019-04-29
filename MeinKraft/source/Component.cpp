#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject& gameObject)
	: m_gameObject(gameObject)
{

}

Component::~Component()
{
	
}

GameObject& Component::GetGameObject() noexcept
{
	return m_gameObject;
}

Transform& Component::GetTransform() noexcept
{
	return m_gameObject.GetTransform();
}