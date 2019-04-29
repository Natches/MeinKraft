#pragma once
#include "Shader.h"

class DefaultErrorShader final :
	public Shader
{
public:
	DefaultErrorShader() { name = std::string("DefaultErrorShader"); };
	~DefaultErrorShader()	= default;

	virtual bool Init() override;
	virtual void Update(const ShaderParameter& param) override;
	virtual void Clean() override;

private:
	virtual void InitID() override;
	virtual bool CreateProgram() override;
	virtual bool CreateVertexShader(const std::string& FilePath) override;
	virtual bool CreateFragmentShader(const std::string& FilePath) override;
	bool		 CreateGeomShader(const std::string& FilePath);
	inline virtual const std::string& GetShaderName()override { return name; }

private:
	GLuint m_ProgramID;
	GLuint m_VertexShaderID;
	GLuint m_FragmentShaderID;
	GLuint m_GeomShaderID;

	GLuint m_MVPID;
	GLuint m_TimeID;
	std::string name;
};

