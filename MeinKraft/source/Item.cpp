#include "Item.h"



Item::Item(float X, float Y, char ID, char* name):
	m_XPosInTexture(X),
	m_YPosInTexture(Y),
	m_ID(ID),
	m_name(name)
{
}


Item::~Item()
{
}
