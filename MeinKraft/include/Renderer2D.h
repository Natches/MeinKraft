#pragma once
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Component.h"
#include "GameObject.h"
#include "Shader2D.h"
#include <typeinfo>

#define ATLAS_COLS 16
#define ATLAS_ROWS 16

class Renderer2D
{
#pragma region Non-Static Attributes
private:
	Shader2D* m_shader;
	vecs::Vec4 m_colour;
	GLuint m_tex;
	GLuint m_vbo;
	GLuint m_uvbo;
	GLuint m_vao;
	int m_points;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	Renderer2D() = delete;
	Renderer2D(vecs::Vec4 colour = vecs::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	virtual ~Renderer2D();
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
public:
	inline GLuint GetVBO() const { return m_vbo; }
	inline GLuint GetUVBO() const { return m_uvbo; }
	inline GLuint GetVAO() const { return m_vao; }
	inline unsigned int GetPoints() const { return m_points; }
	inline void SetVBO(GLuint vboID) { m_vbo = vboID; }
	inline void SetUVBO(GLuint uvboID) { m_uvbo = uvboID; }
	inline void SetVAO(GLuint vaoID) { m_vao = vaoID; }
	inline void SetTex(GLuint texID) { m_tex = texID; }
	inline void SetPoints(unsigned int triangleCount) { this->m_points = triangleCount; }
	void SetShader(Shader2D* shader);


	///////////////////////////////////
	void InitRenderer();


	///////////////////////////////////



#pragma endregion
};

bool load_texture(const char* file_name, GLuint* tex);