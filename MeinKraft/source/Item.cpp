#include "Item.h"



Item::Item(float X, float Y, char ID, const char* name):
	m_XPosInTexture(X),
	m_YPosInTexture(Y),
	m_name(name),
	m_ID(ID)
{
	(void)m_ID;
}


Item::~Item()
{
}
