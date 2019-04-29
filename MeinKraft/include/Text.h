#pragma once
#include "Font.h"
#include "Vec2.h"
#include "Renderer2D.h"
#include "INISettings.h"
#include <GLFW\glfw3.h>

	class Text
	{
#pragma region Non-Static Attributes
		std::vector<GLfloat> vertexData;
		std::vector<GLfloat> UVData;
		std::string m_string;
		vecs::Vec2 m_position;
		int m_points;
		Font * m_font;
		float m_heightOfChar;
#pragma endregion
#pragma region Non-Static Methods
	public:
		Text() = delete;
		Text(std::string string,
			Font * font,
			float heightOfLetterInPX = 64.0f,
			vecs::Vec2 position = vecs::Vec2(0, 0)
		);
		~Text();
#pragma endregion
#pragma region Getters & Setters
	public:
		inline std::vector<GLfloat>& GetVertex() { return vertexData; }
		inline std::vector<GLfloat>& GetUVData() { return UVData; }
		inline Font* GetFont() { return m_font; }
		inline int GetPoints() { return m_points; }
		bool SetString(std::string str);
		bool SetPosition(vecs::Vec2 pos);
#pragma endregion
	};

