#include "Font.h"

void Font::Initialize(const char * pathToTexture)
{
	int x = 0, y = 0;
	unsigned char * image_data = SOIL_load_image(pathToTexture, &x, &y, 0, SOIL_LOAD_RGBA);
	//unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		return;
	}
	glGenTextures(1, &m_texture);
	glActiveTexture(GL_TEXTURE0 + m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
	);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
	SOIL_free_image_data(image_data);
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
