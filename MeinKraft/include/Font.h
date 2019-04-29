#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>
	class Font
	{
#pragma region Non-Static Attributes
	private:
		float	glyph_y_offsets[256] = { 0.0f };
		float	glyph_widths[256] = { 0.0f };
		GLuint	m_texture;
#pragma endregion
#pragma region Non-Static Methods
	public:
		Font() = delete;
		Font(const char * pathToTexture);
		~Font();
	private:
		void TweakGlyphs();
		void Initialize(const char * pathToTexture);
#pragma endregion
#pragma region Getters & Setters
	public:
		inline GLuint GetTexture() { return m_texture; }
		inline float* GetGlyphYOffsets() { return glyph_y_offsets; }
		inline float* GetGlyphWidth() { return glyph_widths; }
#pragma endregion
	};
