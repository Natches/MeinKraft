#include <string>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "SOIL.h"


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
	unsigned char* img = SOIL_load_image(path.c_str(), width, height, NULL, SOIL_LOAD_AUTO);
	if (img)
	{
		glGenTextures(1, &ID);
		glActiveTexture(GL_TEXTURE0 + ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		
		glActiveTexture(GL_TEXTURE0 + ID + 1);
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(img);
		return true;
	}
	return false;
}


/*
// layout(local_size_x = 1, local_size_y = 1) in;
*

layout(binding = 0, RGBA32F) uniform image2D img_out;

uniform float A = 128.f;
uniform float B = 1.f / (2.f * (8.f * 8.f));
uniform float C = 1.f / (2.f * (8.f * 8.f));

shared float AS;

float rand(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void GenHeightMap()
{
	for (int idx = 0; idx < 8; ++idx)
	{
		//if( int(rand(vec2(idx, rand(vec2(gl_WorkGroupID.x, gl_WorkGroupID.y))))) %  2 == 0);
		//{		
		//barrier();
		//memoryBarrierShared();
		ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
		//float demiMaxX = gl_GlobalInvocationID.x - gl_WorkGroupSize.x;
		//float demiMaxY = gl_GlobalInvocationID.y - gl_WorkGroupSize.y;
		//AS += rand(vec2(rand(vec2(idx, gl_WorkGroupID.x)), gl_WorkGroupID.y));
		//float a = AS;
		//float value = a * exp(-((((gl_WorkGroupID.x - demiMaxX) * (gl_WorkGroupID.x - demiMaxX)) * B) + (((gl_WorkGroupID.y - demiMaxY) * (gl_WorkGroupID.y - demiMaxY))  * C)));
		imageStore(img_out, pos, vec4(0.f, 0.f, 1.f, 1.f));
		//}
	}
}

void main()
{
	//AS = A;
	//GenHeightMap();
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
	imageStore(img_out, pos, vec4(0.f, 0.f, 0.f, 1.f));
}*/