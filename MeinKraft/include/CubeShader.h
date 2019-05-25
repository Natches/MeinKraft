#pragma once

#include <map>
#include "Shader.h"

class CubeShader final : public Shader
{
public:
	enum E_PERCENT
	{
		ZERO = 240,
		TEN,
		TWENTY,
		THIRTY,
		FORTHY,
		FIFTY,
		SIXTY,
		SEVENTY,
		HEIGHTY,
		NINETY
	};

public:
	CubeShader() { name = std::string("CubeShader"); };
	~CubeShader()	= default;
	virtual bool Init() override;

	virtual void Update(const ShaderParameter& param) override;

	virtual void Clean() override;

	GLuint GetTexture(const std::string& name);
	inline const std::map<std::string, GLuint>& GetTextures() { return m_textureWanted; }
	inline virtual const std::string& GetShaderName()override { return name; }
	inline void setTextureBreak(E_PERCENT perccent) { TextureBreak = perccent; }

private:
	virtual void InitID() override;

	virtual bool	CreateProgram() override;
	virtual bool	CreateVertexShader		(const std::string& FilePath) override;
	virtual bool	CreateFragmentShader	(const std::string& FilePath) override;
	bool			CreateGeomShader		(const std::string& FilePath);

private:
	GLuint m_ProgramID;
	GLuint m_VertexShaderID;
	GLuint m_FragmentShaderID;
	GLuint m_GeomShaderID;

	GLuint m_VPID;
	GLuint m_textureMapID;

	GLuint m_cameraPosLocation;
	GLuint m_cameraFovLocation;
	GLuint m_cameraDirLocation;
	GLuint m_textureMapLocation;
	GLuint m_offsetLocation;
	GLuint m_viewDistanceLocation;
	GLuint m_isWireframeLocation;
	GLuint m_wireframeColorLocation;
	GLuint m_ambientLocation;
	GLuint m_breakTextureLocation;
	GLuint m_maxLightLevelLocation;
	GLuint m_minLightLevelLocation;

	E_PERCENT TextureBreak;

	std::map<std::string, GLuint> m_textureWanted;

	std::string name;
};

