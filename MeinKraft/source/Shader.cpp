#include <string>
#include <fstream>
#include <sstream>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


bool Shader::Load_File(const GLuint ID, const std::string& FilePath)
{
	std::ifstream File;
	File.open(FilePath);
	if (File.is_open())
	{
		GLint compilationState;
		std::stringstream shaderStream;
		shaderStream << File.rdbuf();
		File.close();
		std::string shaderStr = shaderStream.str();
		const char* shaderSrc = shaderStr.c_str();

		glShaderSource(ID, 1, &shaderSrc, NULL);
		glCompileShader(ID);
		glGetShaderiv(ID, GL_COMPILE_STATUS, &compilationState);
		CheckError(ID);

		return (compilationState == 1);
	}
	return false;
}

void Shader::CheckError(const GLuint ID)
{
	(void)ID;
#ifdef _DEBUG
	GLint LengthLastOp;
	glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &LengthLastOp);
	if (LengthLastOp > 1)
	{
		GLchar* Log = new GLchar[LengthLastOp + 1];
		glGetShaderInfoLog(ID, LengthLastOp, 0, Log);
		std::string error; 
		error += "Failed To Load Shader with ID :" + std::to_string(ID) + '\n';
		error += (const char*)Log;
		throw std::exception(error.c_str());
		Clean();
	}
#endif
}

bool Shader::LoadTexture(GLuint& ID, const std::string& path, int* width, int* height)
{
	int n = 0;
	int force_channels = 4;
	unsigned char * image_data = stbi_load(path.c_str(), width, height, &n, force_channels);
	if (image_data)
	{
		int nearestWidthPowerofTwo = std::floor(std::log(*width) / std::log(2));
		int nearestHeightPowerofTwo = std::floor(std::log(*height) / std::log(2));
		glCreateTextures(GL_TEXTURE_2D, 1, &ID);
		glTextureStorage2D(ID, nearestWidthPowerofTwo <= nearestHeightPowerofTwo ? nearestWidthPowerofTwo : nearestHeightPowerofTwo, GL_RGBA8, *width, *height);
		glTextureSubImage2D(ID, 0, 0, 0, *width, *height, GL_RGBA,
			GL_UNSIGNED_BYTE, image_data);
		glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(ID, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glGenerateTextureMipmap(ID);

		stbi_image_free(image_data);
		return true;
	}
	return false;
}