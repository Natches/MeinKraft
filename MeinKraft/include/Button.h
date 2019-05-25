#pragma once
#include <vector>
#include <GL\glew.h>
#include "Vec2.h"
#include <functional>


class Text;
class Font;
class Button
{
	std::vector<GLfloat>	m_vertexData;
	std::vector<GLfloat>	m_UVData;
	vecs::Vec2				m_pos;
	vecs::Vec2				m_dimensions;
	std::function<void()>	m_func;
	Text*					m_text;
public:
	//Dimensions is in pixels
	//Pos is in screen coordinatex ie:-1, 1 is upper left hand corner
	Button(const std::function<void()>& funtionToExecuteOnClick, std::string text, Font* font, vecs::Vec2 pos, vecs::Vec2 dimensions);
	~Button();
	inline std::vector<GLfloat>&	GetVertexData() {return m_vertexData ; }
	inline std::vector<GLfloat>&	GetUVData() { return m_UVData; }
	inline Text*					GetText() { return m_text; }
	bool IsTouchingMouse(vecs::Vec2 mousePos);
	void Activate();
};

