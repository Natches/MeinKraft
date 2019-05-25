#pragma once

#include "GL/glew.h"
#include <string>

struct ShaderParameter
{
	float mat[16];
	float camPos[3];
	float camDir[3];
	float color[3];
	float ambient[3];
	bool wireframe;
};

class Shader
{
public:
	Shader() = default;
	virtual ~Shader() = default;

	virtual bool Init()																	= 0;
	virtual void Update(const ShaderParameter& param)									= 0;
	virtual void Clean()																= 0;
	virtual const std::string& GetShaderName()											= 0;
protected:
	virtual void InitID()																= 0;
	virtual bool  CreateProgram()														= 0;
	virtual bool  CreateVertexShader	(const std::string& FilePath)					= 0;
	virtual bool  CreateFragmentShader	(const std::string& FilePath)					= 0;
	bool		  Load_File				(const GLuint ID, const std::string& FilePath);
	void		  CheckError			(const GLuint ID);
public:
	static bool LoadTexture					(GLuint& ID, const std::string& path, int* width, int* height);
};

