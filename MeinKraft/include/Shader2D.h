#pragma once
#include "Shader.h"
class Shader2D : public Shader
{
#pragma region Non-Static Attributes
private:
	std::string		m_name;
	GLuint			m_programID;
	GLuint			m_vertexShaderID;
	GLuint			m_fragmentShaderID;
	GLuint			m_UniTexID;
	GLuint			m_UniColourID;
	bool			m_initialized;
#pragma endregion
#pragma region Non-Static Methods
public:
	Shader2D() { m_initialized = false; m_name = "Shader2D"; };
	~Shader2D() = default;

	virtual bool Init() override;
	virtual void Update(const ShaderParameter & param) override;
	virtual void Clean() override;
	virtual const std::string & GetShaderName() override { return m_name; };

private:
	virtual void InitID() override;
	virtual bool CreateProgram() override;
	virtual bool CreateVertexShader(const std::string & FilePath) override;
	virtual bool CreateFragmentShader(const std::string & FilePath) override;
#pragma endregion
#pragma region Getters & Setters
public:
	inline GLuint GetUniformTextureLocation() { return m_UniTexID; }
	inline GLuint GetUniformColourLocation() { return m_UniColourID; }
	inline bool IsInitialized() { return m_initialized; }
#pragma endregion
};

