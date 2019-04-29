#pragma once
#include <map>
#include <string>
#include <typeinfo>

class Shader;

class ShaderLib
{
public:
	ShaderLib() = default;
	~ShaderLib();
#pragma region Non-Static Attributes
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
	void LoadShaders(const unsigned int shaderCount, Shader*...);
	void Clean();
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
	Shader* GetShader(const std::string& name);
#pragma endregion

private:
	std::map<std::string, Shader*> m_lib;
};

