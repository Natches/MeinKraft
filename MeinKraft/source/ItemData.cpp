#include "ItemData.h"



ItemData::ItemData(float X, float Y, char* name, E_ITEMFLAGS flags):
	m_XPosInTexture(X),
	m_YPosInTexture(Y),
	m_flags(flags),
	m_name(name)
{
}


ItemData::~ItemData()
{
}
