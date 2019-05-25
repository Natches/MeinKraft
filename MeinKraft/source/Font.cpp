#include "Font.h"
#include <Shader.h>

void Font::Initialize(const char * pathToTexture)
{
	int x = 0, y = 0;
	Shader::LoadTexture(m_texture, pathToTexture, &x, &y);
}

Font::Font(const char * pathToTexture)
{
	Initialize(pathToTexture);
	TweakGlyphs();
}

Font::~Font()
{
}

void Font::TweakGlyphs()
{
	/* lower down glyhs by some factor */
	for (int i = 0; i < 255; i++) {
		/* default lower-case to half-size spacing */
		if (i >= 'a' && i <= 'z') {
			glyph_y_offsets[i] = 0.1f;
		}
		else if (i >= 'A' && i <= 'Z') {
			glyph_y_offsets[i] = 0.1f;
		}
	}
	glyph_y_offsets[(int)'b'] = 0.1f;
	glyph_y_offsets[(int)'d'] = 0.1f;
	glyph_y_offsets[(int)'f'] = 0.1f;
	glyph_y_offsets[(int)'g'] = 0.1f;
	glyph_y_offsets[(int)'h'] = 0.1f;
	glyph_y_offsets[(int)'i'] = 0.1f;
	glyph_y_offsets[(int)'j'] = 0.1f;
	glyph_y_offsets[(int)'k'] = 0.1f;
	glyph_y_offsets[(int)'l'] = 0.1f;
	glyph_y_offsets[(int)'p'] = 0.1f;
	glyph_y_offsets[(int)'q'] = 0.1f;
	glyph_y_offsets[(int)'s'] = 0.1f;
	glyph_y_offsets[(int)'t'] = 0.1f;
	glyph_y_offsets[(int)'!'] = 0.1f;
	glyph_y_offsets[(int)','] = 0.1f;
	/* reduce spacing after glyph */
	for (int i = 0; i < 255; i++) {
		/* default lower-case to half-size spacing */
		if (i >= 'a' && i <= 'z') {
			glyph_widths[i] = 0.11f;
		}
		else if (i >= 'A' && i <= 'Z') {
			glyph_widths[i] = 0.11f;
		}
		else {
			glyph_widths[i] = 0.11f;
		}
	}
	glyph_widths[(int)' '] = 0.11f;
	glyph_widths[(int)'!'] = 0.11f;
	glyph_widths[(int)','] = 0.11f;
	glyph_widths[(int)'A'] = 0.11f;
	glyph_widths[(int)'B'] = 0.11f;
	glyph_widths[(int)'D'] = 0.11f;
	glyph_widths[(int)'F'] = 0.11f;
	glyph_widths[(int)'L'] = 0.11f;
	glyph_widths[(int)'M'] = 0.11f;
	glyph_widths[(int)'N'] = 0.11f;
	glyph_widths[(int)'P'] = 0.11f;
	glyph_widths[(int)'R'] = 0.11f;
	glyph_widths[(int)'S'] = 0.11f;
	glyph_widths[(int)'T'] = 0.11f;
	glyph_widths[(int)'U'] = 0.11f;
	glyph_widths[(int)'V'] = 0.11f;
	glyph_widths[(int)'W'] = 0.11f;
	glyph_widths[(int)'e'] = 0.11f;
	glyph_widths[(int)'f'] = 0.11f;
	glyph_widths[(int)'g'] = 0.11f;
	glyph_widths[(int)'h'] = 0.11f;
	glyph_widths[(int)'i'] = 0.11f;
	glyph_widths[(int)'j'] = 0.11f;
	glyph_widths[(int)'k'] = 0.11f;
	glyph_widths[(int)'l'] = 0.11f;
	glyph_widths[(int)'p'] = 0.11f;
	glyph_widths[(int)'q'] = 0.11f;
	glyph_widths[(int)'r'] = 0.11f;
	glyph_widths[(int)'t'] = 0.11f;
	glyph_widths[(int)'u'] = 0.11f;
	glyph_widths[(int)'v'] = 0.11f;
	glyph_widths[(int)'w'] = 0.11f;
	glyph_widths[(int)'y'] = 0.11f;
}
