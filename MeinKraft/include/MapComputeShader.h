#pragma once
#include "Shader.h"

class MapComputeShader final :
	public Shader
{
public:
	MapComputeShader() { name = std::string("MapComputeShader"); };
	~MapComputeShader() = default;

	virtual bool Init() override;
	virtual void Update(const ShaderParameter& param) override;
	virtual void Clean() override;
	virtual const std::string& GetShaderName() override;
	virtual void InitID() override;
	virtual bool CreateProgram() override;
	bool CreateComputeShader(const std::string& FilePath);
	virtual bool CreateVertexShader(const std::string& FilePath) override { (void)FilePath; return false; };
	virtual bool CreateFragmentShader(const std::string& FilePath) override { (void)FilePath; return false; };

private:
	std::string name;
	GLuint m_computeShaderID;
	GLuint m_programID;

	GLuint m_heightMapID;
	

};

