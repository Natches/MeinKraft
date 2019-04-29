#include "GameObject.h"

GameObject::GameObject(const short ID)
	: m_transform(*this), m_ID(ID)
{
	
}

GameObject::~GameObject()
{

}

void GameObject::Update()
{

}