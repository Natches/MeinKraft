#include <fstream>
#include <string>
#include <algorithm>
#include "mathlib.h"
#include "INISettings.h"
#include "StringUtils.h"

namespace INISettings
{
	namespace defaults
	{
		const int			WINDOW_WIDTH			= 800;
		const int			WINDOW_HEIGHT			= 600;
		const E_WINDOW_MODE	WINDOW_MODE				= E_WINDOW_MODE::WINDOW;
		const bool			HIDDEN_CURSOR			= false;
		const vecs::Vec4	BACKGROUND_COLOR		= vecs::Vec4(0.4f, 0.4f, 0.4f);
		const float			CAMERA_FOV				= 45.f;
		const vecs::Vec3	CAMERA_POSITION			= vecs::Vec3(0.f, 0.f, 2.f);
		const vecs::Vec3	CAMERA_TARGET			= vecs::Vec3(0.f, 0.f, 0.f);
		const vecs::Vec3	CAMERA_UP				= vecs::Vec3::Up();
		const float			CAMERA_SENSITIVITY		= 15.f;
		const float			CAMERA_ANGULAR_SPEED	= 0.15f;
		const float			PLAYER_MOVEMENT_SPEED	= 8.f;
		const int			WORLD_WIDTH				= 64;
		const int			WORLD_HEIGHT			= 64;
		const int			WORLD_DEPTH				= 64;
		const int			BLUR_PASSES				= 23;
		const vecs::Vec3	GRAVITY					= vecs::Vec3(0.f, 9.81f, 0.f);
		const vecs::Vec3	WIREFRAME_COLOR			= vecs::Vec3(1.f, 1.f, 1.f);
		const float			PLAYER_SPRINT_MODIFIER	= 2.f;
		const unsigned int	VIEW_DISTANCE			= 10u;
		const std::string	BLOCK_TEXTURE_PATH		= "Assets/pack2.png";
		const std::string	ITEM_TEXTURE_PATH		= "Assets/items.png";
	}

	int				windowWidth				= defaults::WINDOW_WIDTH;
	int				windowHeight			= defaults::WINDOW_HEIGHT;
	E_WINDOW_MODE	windowMode				= defaults::WINDOW_MODE;
	bool			hiddenCursor			= defaults::HIDDEN_CURSOR;
	vecs::Vec4		backgroundColor			= defaults::BACKGROUND_COLOR;
	float			cameraFOV				= defaults::CAMERA_FOV;
	vecs::Vec3		cameraPosition			= defaults::CAMERA_POSITION;
	vecs::Vec3		cameraTarget			= defaults::CAMERA_TARGET;
	vecs::Vec3		cameraUp				= defaults::CAMERA_UP;
	float			cameraSensitivity		= defaults::CAMERA_SENSITIVITY;
	float			playerMovementSpeed		= defaults::PLAYER_MOVEMENT_SPEED;
	float			cameraAngularSpeed		= defaults::CAMERA_ANGULAR_SPEED;
	vecs::Vec3		gravity					= defaults::GRAVITY;
	int				worldWidth				= defaults::WORLD_WIDTH;
	int				worldHeight				= defaults::WORLD_HEIGHT;
	int				worldDepth				= defaults::WORLD_DEPTH;
	int				blurPasses				= defaults::BLUR_PASSES;
	vecs::Vec3		wireframeColor			= defaults::WIREFRAME_COLOR;
	float			playerSprintModifier	= defaults::PLAYER_SPRINT_MODIFIER;
	int				viewDistance			= defaults::VIEW_DISTANCE;
	std::string		blockTexturePath		= defaults::BLOCK_TEXTURE_PATH;
	std::string		itemTexturePath			= defaults::ITEM_TEXTURE_PATH;

	void LoadConfigFile(const char* path)
	{
		std::ifstream	file(path);
		std::string		line;
		std::string		value;
		std::string		region;

		while (std::getline(file, line))
		{
			value = SubStringAfterDelim(line, "=");
			VerifyRegion(line, region);
			ParseValuePerRegion(line, value, region);
		}
	}

	std::string SubStringAfterDelim(const std::string& str, const std::string& delim)
	{
		return str.substr(str.find(delim) + delim.size());
	}

	std::string	SubStringBetweenDelimiters(const std::string& str, const std::string& startDelim, const std::string& endDelim)
	{
		size_t start = str.find(startDelim) + startDelim.size(), end = str.find(endDelim);
		return str.substr(start, end - start);
	}

	E_WINDOW_MODE ValidateWindowMode(const std::string& value)
	{
		return (E_WINDOW_MODE)mathlib::Clamp(0, E_WINDOW_MODE::COUNT - 1, abs(std::stoi(value)));
	}

	void VerifyRegion(const std::string& line, std::string& region)
	{
		if (line.find("[") != std::string::npos)
		{
			region = SubStringBetweenDelimiters(line, "[", "]");
		}
	}

	bool ValidateBool(const std::string& value)
	{
		std::string copy(value);
		std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
		if (copy.empty() || copy == "false" || stoi(copy) == 0)
		{
			return false;
		}
		else { return true; }
	}

	void ParseValuePerRegion(const std::string& line, const std::string& value, const std::string& region)
	{
		if (region == "Window")
		{
			ParseWindowRegion(line, value);
		}
		else if (region == "Camera")
		{
			ParseCameraRegion(line, value);
		}
		else if (region == "Player")
		{
			ParsePlayerRegion(line, value);
		}
		else if (region == "Physics")
		{
			ParsePhysicsRegion(line, value);
		}
		else if (region == "World")
		{
			ParseWorldRegion(line, value);
		}
		else if (region == "Rendering")
		{
			ParseRenderingRegion(line, value);
		}
	}

	void ParseWindowRegion(const std::string& line, const std::string& value)
	{
		if (line.find("width") != std::string::npos)
		{
			windowWidth = abs(std::stoi(value));
		}
		else if (line.find("height") != std::string::npos)
		{
			windowHeight = abs(std::stoi(value));
		}
		else if (line.find("mode") != std::string::npos)
		{
			windowMode = ValidateWindowMode(value);
		}
		else if (line.find("hidden-cursor") != std::string::npos)
		{
			hiddenCursor = ValidateBool(value);
		}
		else if (line.find("background-color") != std::string::npos)
		{
			ParseArrayToVector(value, backgroundColor);
		}
	}

	void ParseCameraRegion(const std::string& line, const std::string& value)
	{
		if (line.find("fov") != std::string::npos)
		{
			cameraFOV = abs(std::stof(value));
		}
		else if (line.find("position") != std::string::npos)
		{
			ParseArrayToVector(value, cameraPosition);
		}
		else if (line.find("target") != std::string::npos)
		{
			ParseArrayToVector(value, cameraTarget);
		}
		else if (line.find("up") != std::string::npos)
		{
			ParseArrayToVector(value, cameraUp);
		}
		else if (line.find("sensitivity") != std::string::npos)
		{
			cameraSensitivity = abs(std::stof(value));
		}
		else if (line.find("angularSpeed") != std::string::npos)
		{
			cameraAngularSpeed = abs(std::stof(value));
		}
		else if (line.find("viewDistance") != std::string::npos)
		{
			viewDistance = abs(stoi(value));
		}
	}

	void ParsePhysicsRegion(const std::string& line, const std::string& value)
	{
		if (line.find("gravity") != std::string::npos)
		{
			ParseArrayToVector(value, gravity);
		}
	}

	void ParseWorldRegion(const std::string& line, const std::string& value)
	{
		if (line.find("width") != std::string::npos)
		{
			worldWidth = abs(std::stoi(value));
		}
		else if (line.find("height") != std::string::npos)
		{
			worldHeight = abs(std::stoi(value));
		}
		else if (line.find("depth") != std::string::npos)
		{
			worldDepth = abs(std::stoi(value));
		}
		else if (line.find("blurPasses") != std::string::npos)
		{
			blurPasses = abs(std::stoi(value));
		}
	}

	void ParseRenderingRegion(const std::string & line, const std::string & value)
	{
		if (line.find("wireframeColor") != std::string::npos)
		{
			ParseArrayToVector(value, wireframeColor);
		}
		else if (line.find("blockTexturePack") != std::string::npos)
		{
			blockTexturePath = std::string("Assets/") + value;
		}
		else if (line.find("itemTexturePack") != std::string::npos)
		{
			itemTexturePath = std::string("Assets/") + value;
		}
	}

	void ParsePlayerRegion(const std::string & line, const std::string & value)
	{
		if (line.find("sprintModifier") != std::string::npos)
		{
			playerSprintModifier = abs(std::stof(value));
		}
		else if (line.find("movementSpeed") != std::string::npos)
		{
			playerMovementSpeed = abs(std::stof(value));
		}
	}

	void ParseArrayToVector(const std::string& value, vecs::Vec3& v)
	{
		float a[4];
		v.ToArray(a);
		strutils::ParseVector(value, a);

		v.x = a[0];
		v.y = a[1];
		v.z = a[2];
	}

	void ParseArrayToVector(const std::string& value, vecs::Vec4& v)
	{
		float a[4];
		v.ToArray(a);
		strutils::ParseVector(value, a);

		v.x = a[0];
		v.y = a[1];
		v.z = a[2];
		v.w = a[3];
	}
}