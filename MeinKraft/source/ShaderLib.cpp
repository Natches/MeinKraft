#include <cstdarg>
#include "Shader.h"
#include "ShaderLib.h"

ShaderLib::~ShaderLib()
{
	for (auto shader : m_lib)
	{
		shader.second->Clean();
		delete shader.second;
	}
}

void ShaderLib::LoadShaders(const unsigned int shaderCount, ...)
{
	va_list args;
	va_start(args, shaderCount);
	for (unsigned int i = 0; i < shaderCount; ++i)
	{
		Shader* s = va_arg(args, Shader*);
		s->Init();
		m_lib.insert(std::make_pair(s->GetShaderName(), s));
	}
	va_end(args);
}

void ShaderLib::Clean()
{
	for (auto shader : m_lib)
	{
		shader.second->Clean();
		delete shader.second;
	}
}

Shader* ShaderLib::GetShader(const std::string& name)
{
	return (m_lib.find(name) == m_lib.end() ? m_lib["DefaultErrorShader"] :  m_lib[name]);
}
