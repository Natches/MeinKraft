#include "DefaultErrorShader.h"
#include "GLFW/glfw3.h"

bool DefaultErrorShader::Init()
{
	if (CreateVertexShader("Shader/ErrorShader.vert"))
		if (CreateGeomShader("Shader/ErrorShader.geom"))
			if (CreateFragmentShader("Shader/ErrorShader.frag"))
				if (CreateProgram())
				{
					InitID();
					return true;
				}
	Clean();
	return false;
}

void DefaultErrorShader::Update(const ShaderParameter& param)
{
	glUniformMatrix4fv(m_MVPID, 1, GL_FALSE, param.mat);

	glUniform1f(m_TimeID, (float)glfwGetTime());

	glUseProgram(m_ProgramID);
}

void DefaultErrorShader::Clean()
{
	glUseProgram(0);
	glDetachShader(m_ProgramID, m_VertexShaderID);
	glDetachShader(m_ProgramID, m_GeomShaderID);
	glDetachShader(m_ProgramID, m_FragmentShaderID);
	glDeleteProgram(m_ProgramID);
}

void DefaultErrorShader::InitID()
{
	m_MVPID		= glGetUniformLocation(m_ProgramID, "MVP");
	m_TimeID	= glGetUniformLocation(m_ProgramID, "ITime");
}

bool DefaultErrorShader::CreateProgram()
{
	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, m_VertexShaderID);
	glAttachShader(m_ProgramID, m_GeomShaderID);
	glAttachShader(m_ProgramID, m_FragmentShaderID);
	glLinkProgram(m_ProgramID);
	glDeleteShader(m_VertexShaderID);
	glDeleteShader(m_FragmentShaderID);
	glDeleteShader(m_GeomShaderID);
	CheckError(m_ProgramID);
	return true;
}

bool DefaultErrorShader::CreateVertexShader(const std::string & FilePath)
{
	m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	return Load_File(m_VertexShaderID, FilePath);
}

bool DefaultErrorShader::CreateFragmentShader(const std::string & FilePath)
{
	m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	return Load_File(m_FragmentShaderID, FilePath);
}

bool DefaultErrorShader::CreateGeomShader(const std::string & FilePath)
{
	m_GeomShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	return Load_File(m_GeomShaderID, FilePath);
}
