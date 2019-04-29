#include "Button.h"
#define CHAR_SEPARATION 20.f


Button::Button(const std::function<void()>& funtionToExecuteOnClick, std::string text, Font* font, vecs::Vec2 pos, vecs::Vec2 dimensions) :
	m_pos(pos),
	m_dimensions(dimensions),
	m_func(funtionToExecuteOnClick)
{
	m_text = new Text(text, font, dimensions.x/text.length(), vecs::Vec2(pos.x + CHAR_SEPARATION/INISettings::windowWidth, pos.y - dimensions.y/INISettings::windowHeight/2 + dimensions.x / text.length() /INISettings::windowHeight/2));

	m_vertexData.push_back(m_pos.x);
	m_vertexData.push_back(m_pos.y);

	m_vertexData.push_back(m_pos.x);
	m_vertexData.push_back(m_pos.y - dimensions.y / INISettings::windowHeight);

	m_vertexData.push_back(m_pos.x + dimensions.x / INISettings::windowWidth);
	m_vertexData.push_back(m_pos.y - dimensions.y / INISettings::windowHeight);

	m_vertexData.push_back(m_pos.x + dimensions.x / INISettings::windowWidth);
	m_vertexData.push_back(m_pos.y - dimensions.y / INISettings::windowHeight);
	m_vertexData.push_back(m_pos.x + dimensions.x / INISettings::windowWidth);
	m_vertexData.push_back(m_pos.y);
	m_vertexData.push_back(m_pos.x);
	m_vertexData.push_back(m_pos.y);
	 
	m_UVData.push_back(0);
	m_UVData.push_back(0);

	m_UVData.push_back(0);
	m_UVData.push_back(1);

	m_UVData.push_back(1);
	m_UVData.push_back(1);

	m_UVData.push_back(1);
	m_UVData.push_back(1);

	m_UVData.push_back(1);
	m_UVData.push_back(0);

	m_UVData.push_back(0);
	m_UVData.push_back(0);
}


Button::~Button()
{
	delete m_text;
}

bool Button::IsTouchingMouse(vecs::Vec2 mousePos)
{
	if (mousePos.x > m_pos.x
		&& mousePos.x < m_pos.x + m_dimensions.x/INISettings::windowWidth
		&& mousePos.y < m_pos.y
		&& mousePos.y > m_pos.y - m_dimensions.y/INISettings::windowHeight)
		return true;
	return false;
}

void Button::Activate()
{
	m_func();
}
