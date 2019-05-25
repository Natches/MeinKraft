#include "Text.h"
#include <typeinfo>

#undef ATLAS_COLS
#undef ATLAS_ROWS
#define ATLAS_COLS 10
#define ATLAS_ROWS 10
#define CHAR_SEPARATION 20.f

Text::Text(std::string string, Font * font, float heightOfLetter, vecs::Vec2 position)
	:m_string(string),
	m_position(position),
	m_font(font),
	m_heightOfChar(heightOfLetter)
{
	int len = string.length();
	vertexData.clear();
	UVData.clear();
	vertexData.reserve(len * 12);
	UVData.reserve(len * 12);
	for (int i = 0; i < len; i++) {
		// get ascii code as integer
		int ascii_code = string[i];

		// work out row and column in atlas
		int atlas_col = (ascii_code - ' ') % ATLAS_COLS;
		int atlas_row = (ascii_code - ' ') / ATLAS_COLS;

		// work out texture coordinates in atlas
		float s = atlas_col * (1.0f / ATLAS_COLS);
		float t = atlas_row * (1.0f / ATLAS_ROWS);

		// work out position of glyphtriangle_width
		float x_pos = position.x + (heightOfLetter / INISettings::windowWidth)*i - (CHAR_SEPARATION/INISettings::windowWidth)*i;
		float y_pos = position.y;
		// add 6 points and texture coordinates to buffers for each glyph
		vertexData.push_back(x_pos);
		vertexData.push_back(y_pos);
		vertexData.push_back(x_pos);
		vertexData.push_back(y_pos - heightOfLetter / INISettings::windowHeight);
		vertexData.push_back(x_pos + heightOfLetter / INISettings::windowWidth);
		vertexData.push_back(y_pos - heightOfLetter / INISettings::windowHeight);

		vertexData.push_back(x_pos + heightOfLetter / INISettings::windowWidth);
		vertexData.push_back(y_pos - heightOfLetter / INISettings::windowHeight);
		vertexData.push_back(x_pos + heightOfLetter / INISettings::windowWidth);
		vertexData.push_back(y_pos);
		vertexData.push_back(x_pos);
		vertexData.push_back(y_pos);

		UVData.push_back(s);
		UVData.push_back(t);
		UVData.push_back(s);
		UVData.push_back(t + 1.0f / ATLAS_ROWS);
		UVData.push_back(s + 1.0f / ATLAS_COLS);
		UVData.push_back(t + 1.0f / ATLAS_ROWS);

		UVData.push_back(s + 1.0f / ATLAS_COLS);
		UVData.push_back(t + 1.0f / ATLAS_ROWS);
		UVData.push_back(s + 1.0f / ATLAS_COLS);
		UVData.push_back(t);
		UVData.push_back(s);
		UVData.push_back(t);
	}
	m_points = len * 6;
}

Text::~Text()
{
}

bool Text::SetString(std::string str)
{
	if (str == m_string)
	{
		return false;
	}
	m_string = str;
	int len = m_string.length();
	vertexData.clear();
	UVData.clear();
	vertexData.reserve(len * 12);
	UVData.reserve(len * 12);
	for (int i = 0; i < len; i++) {
		// get ascii code as integer
		int ascii_code = m_string[i];

		// work out row and column in atlas
		int atlas_col = (ascii_code - ' ') % ATLAS_COLS;
		int atlas_row = (ascii_code - ' ') / ATLAS_COLS;

		// work out texture coordinates in atlas
		float s = atlas_col * (1.0f / ATLAS_COLS);
		float t = atlas_row * (1.0f / ATLAS_ROWS);

		// work out position of glyphtriangle_width
		float x_pos = m_position.x + (m_heightOfChar / INISettings::windowWidth)*i - (CHAR_SEPARATION / INISettings::windowWidth)*i;
		float y_pos = m_position.y;
		// add 6 points and texture coordinates to buffers for each glyph
		vertexData.push_back(x_pos);
		vertexData.push_back(y_pos);
		vertexData.push_back(x_pos);
		vertexData.push_back(y_pos - m_heightOfChar / INISettings::windowHeight);
		vertexData.push_back(x_pos + m_heightOfChar / INISettings::windowWidth);
		vertexData.push_back(y_pos - m_heightOfChar / INISettings::windowHeight);

		vertexData.push_back(x_pos + m_heightOfChar / INISettings::windowWidth);
		vertexData.push_back(y_pos - m_heightOfChar / INISettings::windowHeight);
		vertexData.push_back(x_pos + m_heightOfChar / INISettings::windowWidth);
		vertexData.push_back(y_pos);
		vertexData.push_back(x_pos);
		vertexData.push_back(y_pos);

		UVData.push_back(s);
		UVData.push_back(t);
		UVData.push_back(s);
		UVData.push_back(t + 1.0f / ATLAS_ROWS);
		UVData.push_back(s + 1.0f / ATLAS_COLS);
		UVData.push_back(t + 1.0f / ATLAS_ROWS);

		UVData.push_back(s + 1.0f / ATLAS_COLS);
		UVData.push_back(t + 1.0f / ATLAS_ROWS);
		UVData.push_back(s + 1.0f / ATLAS_COLS);
		UVData.push_back(t);
		UVData.push_back(s);
		UVData.push_back(t);
	}
	m_points = len * 6;
	return true;
}

bool Text::SetPosition(vecs::Vec2 pos)
{
	if (m_position == pos)
	{
		return false;
	}
	m_position = pos;
	int len = m_string.length();
	vertexData.clear();
	UVData.clear();
	vertexData.reserve(len * 12);
	UVData.reserve(len * 12);
	for (int i = 0; i < len; i++) {
		// get ascii code as integer
		int ascii_code = m_string[i];

		// work out row and column in atlas
		int atlas_col = (ascii_code - ' ') % ATLAS_COLS;
		int atlas_row = (ascii_code - ' ') / ATLAS_COLS;

		// work out texture coordinates in atlas
		float s = atlas_col * (1.0f / ATLAS_COLS);
		float t = atlas_row * (1.0f / ATLAS_ROWS);

		// work out position of glyphtriangle_width
		float x_pos = m_position.x + (m_heightOfChar / INISettings::windowWidth)*i - (CHAR_SEPARATION / INISettings::windowWidth)*i;
		float y_pos = m_position.y;
		// add 6 points and texture coordinates to buffers for each glyph
		vertexData.push_back(x_pos);
		vertexData.push_back(y_pos);
		vertexData.push_back(x_pos);
		vertexData.push_back(y_pos - m_heightOfChar / INISettings::windowHeight);
		vertexData.push_back(x_pos + m_heightOfChar / INISettings::windowWidth);
		vertexData.push_back(y_pos - m_heightOfChar / INISettings::windowHeight);

		vertexData.push_back(x_pos + m_heightOfChar / INISettings::windowWidth);
		vertexData.push_back(y_pos - m_heightOfChar / INISettings::windowHeight);
		vertexData.push_back(x_pos + m_heightOfChar / INISettings::windowWidth);
		vertexData.push_back(y_pos);
		vertexData.push_back(x_pos);
		vertexData.push_back(y_pos);

		UVData.push_back(s);
		UVData.push_back(t);
		UVData.push_back(s);
		UVData.push_back(t + 1.0f / ATLAS_ROWS);
		UVData.push_back(s + 1.0f / ATLAS_COLS);
		UVData.push_back(t + 1.0f / ATLAS_ROWS);

		UVData.push_back(s + 1.0f / ATLAS_COLS);
		UVData.push_back(t + 1.0f / ATLAS_ROWS);
		UVData.push_back(s + 1.0f / ATLAS_COLS);
		UVData.push_back(t);
		UVData.push_back(s);
		UVData.push_back(t);
	}
	m_points = len * 6;
	return true;
}
