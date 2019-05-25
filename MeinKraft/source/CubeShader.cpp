#include <string>
#include <fstream>
#include <sstream>
#include "CubeShader.h"
#include "GLFW/glfw3.h"
#include "iostream"
#include "INISettings.h"
#include "MathLib.h"
#include "Chunk.h"
#include "INISettings.h"

bool CubeShader::Init()
{
	if (CreateVertexShader("Shader/CubeVertShader.vert"))
		if (CreateGeomShader("Shader/CubeGeomShader.geom"))
			if (CreateFragmentShader("Shader/CubeFragShader.frag"))
				if (CreateProgram())
				{
					InitID();
					return true;
				}
	Clean();
	return false;
}

void CubeShader::Update(const ShaderParameter& param)
{
	glUseProgram(m_ProgramID);
	glUniformMatrix4fv(m_VPID, 1, GL_FALSE, param.mat);
	glUniform3fv(m_cameraPosLocation, 1, param.camPos);
	glUniform3fv(m_cameraDirLocation, 1, param.camDir);
	glUniform3fv(m_wireframeColorLocation, 1, param.color);
	glBindTextureUnit(0, m_textureMapID);
	glUniform1i(m_textureMapLocation, 0);
	glUniform1i(m_isWireframeLocation, param.wireframe);
	glUniform1f(m_offsetLocation, 32);
	glUniform1f(m_cameraFovLocation, mathlib::toRadian * (180.f));
	glUniform1f(m_viewDistanceLocation, INISettings::viewDistance * DefaultWidth * 25);
	glUniform3fv(m_ambientLocation, 1, param.ambient);
	glUniform1i(m_breakTextureLocation, TextureBreak);
	glUniform1f(m_maxLightLevelLocation, MAX_LIGTH_LEVEL);
	glUniform1f(m_minLightLevelLocation, MIN_LIGTH_LEVEL);
}


void CubeShader::Clean()
{
	glUseProgram(0);
	glDetachShader(m_ProgramID, m_VertexShaderID);
	glDetachShader(m_ProgramID, m_GeomShaderID);
	glDetachShader(m_ProgramID, m_FragmentShaderID);
	glDeleteProgram(m_ProgramID);
}

GLuint CubeShader::GetTexture(const std::string& name)
{
	return (m_textureWanted.find(name) == m_textureWanted.end() ? 0 : m_textureWanted[name]);
}

bool CubeShader::CreateProgram()
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

bool CubeShader::CreateVertexShader(const std::string& FilePath)
{
	m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	return Load_File(m_VertexShaderID, FilePath);
}

bool CubeShader::CreateGeomShader(const std::string& FilePath)
{
	m_GeomShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	return Load_File(m_GeomShaderID, FilePath);
}

bool CubeShader::CreateFragmentShader(const std::string& FilePath)
{
	m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	return Load_File(m_FragmentShaderID, FilePath);
}

void CubeShader::InitID()
{
	m_VPID = glGetUniformLocation(m_ProgramID, "VP");
	m_textureMapLocation = glGetUniformLocation(m_ProgramID, "texMap");
	m_offsetLocation = glGetUniformLocation(m_ProgramID, "offset");
	m_cameraPosLocation = glGetUniformLocation(m_ProgramID, "CameraPos");
	m_cameraFovLocation = glGetUniformLocation(m_ProgramID, "Fov");
	m_cameraDirLocation = glGetUniformLocation(m_ProgramID, "CameraDir");
	m_viewDistanceLocation = glGetUniformLocation(m_ProgramID, "ViewDistance");
	m_isWireframeLocation = glGetUniformLocation(m_ProgramID, "isWireframe");
	m_wireframeColorLocation = glGetUniformLocation(m_ProgramID, "color");
	m_ambientLocation = glGetUniformLocation(m_ProgramID, "ambient");
	m_breakTextureLocation = glGetUniformLocation(m_ProgramID, "breakTexture");
	m_maxLightLevelLocation = glGetUniformLocation(m_ProgramID, "maxLightLevel");
	m_minLightLevelLocation = glGetUniformLocation(m_ProgramID, "minLightLevel");

	int height, width;
	LoadTexture(m_textureMapID, INISettings::blockTexturePath, &width, &height);
}