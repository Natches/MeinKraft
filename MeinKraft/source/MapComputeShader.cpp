#include "MapComputeShader.h"
#include "SOIL.h"
#include <iostream>

bool MapComputeShader::Init()
{
	if (CreateComputeShader("Shader/MapShader.comp"))
		if (CreateProgram())
		{
			InitID();
			return true;
		}
	Clean();
	return false;
}

void MapComputeShader::Update(const ShaderParameter& param)
{
	(void)param;
	glUseProgram(m_programID);
	int location = glGetUniformLocation(m_programID, "img_out");
	glUniform1i(location, 0);
	glBindImageTexture(0, m_heightMapID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glDispatchCompute(1024, 1024, 1);
	glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	int format;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);
	unsigned char* img = new unsigned char[1024 * 1024 * 4];
	glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, img);
	SOIL_save_image("img.bmp", SOIL_SAVE_TYPE_BMP, 1024, 1024, SOIL_LOAD_RGBA, img);
	std::cout << SOIL_last_result() << std::endl;
	delete[] img;
	glUseProgram(0);
}

void MapComputeShader::Clean()
{
	glUseProgram(0);
	glDetachShader(m_programID, m_computeShaderID);
	glDeleteProgram(m_programID);
}

const std::string& MapComputeShader::GetShaderName()
{
	return name;
}

void MapComputeShader::InitID()
{
	glUseProgram(m_programID);
	//LoadTexture(m_heightMapID, "Assets/HeightMap.png");
}

bool MapComputeShader::CreateProgram()
{
	m_programID = glCreateProgram();
	glAttachShader(m_programID, m_computeShaderID);
	glLinkProgram(m_programID);
	glDeleteShader(m_computeShaderID);
	CheckError(m_programID);
	return true;
}

bool MapComputeShader::CreateComputeShader(const std::string& FilePath)
{
	m_computeShaderID = glCreateShader(GL_COMPUTE_SHADER);
	return Load_File(m_computeShaderID, FilePath);
}