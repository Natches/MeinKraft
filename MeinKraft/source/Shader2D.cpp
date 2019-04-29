#include "Shader2D.h"

bool Shader2D::Init()
{
	if(CreateVertexShader("Shader/Shader2D.vert"))
		if(CreateFragmentShader("Shader/Shader2D.frag"))
			if (CreateProgram())
			{
				InitID();
				m_initialized = true;
				return true;
			}
	return false;
}

void Shader2D::Update(const ShaderParameter & param)
{
	glUseProgram(m_programID);
}

void Shader2D::Clean()
{
	glUseProgram(0);
	glDetachShader(m_programID, m_vertexShaderID);
	glDetachShader(m_programID, m_fragmentShaderID);
	glDeleteProgram(m_programID);
}

void Shader2D::InitID()
{
	m_UniTexID = glGetUniformLocation(m_programID, "tex");
	m_UniColourID = glGetUniformLocation(m_programID, "text_colour");
}

bool Shader2D::CreateProgram()
{
	m_programID = glCreateProgram();
	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);
	glLinkProgram(m_programID);
	glDeleteProgram(m_vertexShaderID);
	glDeleteProgram(m_fragmentShaderID);
	CheckError(m_programID);
	return true;
}

bool Shader2D::CreateVertexShader(const std::string & FilePath)
{
	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	return Load_File(m_vertexShaderID, FilePath);
}

bool Shader2D::CreateFragmentShader(const std::string & FilePath)
{
	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	return Load_File(m_fragmentShaderID, FilePath);
}
