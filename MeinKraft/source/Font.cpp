#include "Font.h"

void Font::Initialize(const char * pathToTexture)
{
	int x = 0, y = 0, n = 0;
	int force_channels = 4;
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
	glyph_y_offsets['b'] = 0.1f;
	glyph_y_offsets['d'] = 0.1f;
	glyph_y_offsets['f'] = 0.1f;
	glyph_y_offsets['g'] = 0.1f;
	glyph_y_offsets['h'] = 0.1f;
	glyph_y_offsets['i'] = 0.1f;
	glyph_y_offsets['j'] = 0.1f;
	glyph_y_offsets['k'] = 0.1f;
	glyph_y_offsets['l'] = 0.1f;
	glyph_y_offsets['p'] = 0.1f;
	glyph_y_offsets['q'] = 0.1f;
	glyph_y_offsets['s'] = 0.1f;
	glyph_y_offsets['t'] = 0.1f;
	glyph_y_offsets['!'] = 0.1f;
	glyph_y_offsets[','] = 0.1f;
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
	glyph_widths[' '] = 0.11f;
	glyph_widths['!'] = 0.11f;
	glyph_widths[','] = 0.11f;
	glyph_widths['A'] = 0.11f;
	glyph_widths['B'] = 0.11f;
	glyph_widths['D'] = 0.11f;
	glyph_widths['F'] = 0.11f;
	glyph_widths['L'] = 0.11f;
	glyph_widths['M'] = 0.11f;
	glyph_widths['N'] = 0.11f;
	glyph_widths['P'] = 0.11f;
	glyph_widths['R'] = 0.11f;
	glyph_widths['S'] = 0.11f;
	glyph_widths['T'] = 0.11f;
	glyph_widths['U'] = 0.11f;
	glyph_widths['V'] = 0.11f;
	glyph_widths['W'] = 0.11f;
	glyph_widths['e'] = 0.11f;
	glyph_widths['f'] = 0.11f;
	glyph_widths['g'] = 0.11f;
	glyph_widths['h'] = 0.11f;
	glyph_widths['i'] = 0.11f;
	glyph_widths['j'] = 0.11f;
	glyph_widths['k'] = 0.11f;
	glyph_widths['l'] = 0.11f;
	glyph_widths['p'] = 0.11f;
	glyph_widths['q'] = 0.11f;
	glyph_widths['r'] = 0.11f;
	glyph_widths['t'] = 0.11f;
	glyph_widths['u'] = 0.11f;
	glyph_widths['v'] = 0.11f;
	glyph_widths['w'] = 0.11f;
	glyph_widths['y'] = 0.11f;
}
