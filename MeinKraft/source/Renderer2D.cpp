#include <assert.h>
#include "Renderer2D.h"
#include "INISettings.h"
#include "Shader2D.h"

#define ATLAS_COLS 16
#define ATLAS_ROWS 16

Renderer2D::Renderer2D(vecs::Vec4 colour) :
	m_colour(colour)
{
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
	glEnable(GL_CULL_FACE); // cull face
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // partial transparency
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
}


void Renderer2D::SetShader(Shader2D * shader)
{
	if (!shader->IsInitialized())
		shader->Init();
	m_shader = shader;
}

void Renderer2D::InitRenderer()
{
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
	glEnable(GL_CULL_FACE); // cull face
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // partial transparency
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

Renderer2D::~Renderer2D()
{
}