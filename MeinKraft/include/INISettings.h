#pragma once
#include "Vec4.h"

namespace INISettings
{
	enum E_WINDOW_MODE
	{
		FULLSCREEN,
		WINDOW,
		WINDOW_FULLSCREEN,
		COUNT
	};

	namespace defaults
	{
		extern const int			WINDOW_WIDTH;
		extern const int			WINDOW_HEIGHT;
		extern const float			CAMERA_SENSITIVITY;
		extern const float			CAMERA_ANGULAR_SPEED;
		extern const float			PLAYER_MOVEMENT_SPEED;
		extern const float			PLAYER_SPRINT_MODIFIER;
		extern const int			WORLD_WIDTH;
		extern const int			WORLD_HEIGHT;
		extern const int			WORLD_DEPTH;
		extern const int			BLUR_PASSES;
		extern const float			CAMERA_FOV;
		extern const vecs::Vec4		BACKGROUND_COLOR;
		extern const vecs::Vec3		GRAVITY;
		extern const vecs::Vec3		CAMERA_POSITION;
		extern const vecs::Vec3		CAMERA_TARGET;
		extern const vecs::Vec3		CAMERA_UP;
		extern const vecs::Vec3		WIREFRAME_COLOR;
		extern const E_WINDOW_MODE	WINDOW_MODE;
		extern const bool			HIDDEN_CURSOR;
		extern const unsigned int	VIEW_DISTANCE;
		extern const std::string	BLOCK_TEXTURE_PATH;
		extern const std::string	ITEM_TEXTURE_PATH;
	}

	extern int				windowWidth;
	extern int				windowHeight;
	extern float			cameraFOV;
	extern float			cameraSensitivity;
	extern float			playerMovementSpeed;
	extern float			cameraAngularSpeed;
	extern float			playerSprintModifier;
	extern int				worldWidth;
	extern int				worldHeight;
	extern int				worldDepth;
	extern int				blurPasses;
	extern vecs::Vec4		backgroundColor;
	extern vecs::Vec3		cameraPosition;
	extern vecs::Vec3		cameraTarget;
	extern vecs::Vec3		cameraUp;
	extern vecs::Vec3		gravity;
	extern vecs::Vec3		wireframeColor;
	extern E_WINDOW_MODE	windowMode;
	extern bool				hiddenCursor;
	extern int				viewDistance;
	extern std::string		blockTexturePath;
	extern std::string		itemTexturePath;

	void			LoadConfigFile				(const char* path);
	void			VerifyRegion				(const std::string& line, std::string& region);
	void			ParseValuePerRegion			(const std::string& line, const std::string& value, const std::string& region);
	E_WINDOW_MODE	ValidateWindowMode			(const std::string& value);
	void			ParseWindowRegion			(const std::string& line, const std::string& value);
	void			ParseCameraRegion			(const std::string& line, const std::string& value);
	void			ParsePhysicsRegion			(const std::string& line, const std::string& value);
	void			ParseWorldRegion			(const std::string& line, const std::string& value);
	void			ParseRenderingRegion		(const std::string& line, const std::string& value);
	void			ParsePlayerRegion			(const std::string& line, const std::string& value);
	bool			ValidateBool				(const std::string& value);
	void			ParseArrayToVector			(const std::string& value, vecs::Vec3& v);
	void			ParseArrayToVector			(const std::string& value, vecs::Vec4& v);
	std::string		SubStringAfterDelim			(const std::string& str, const std::string& delim);
	std::string		SubStringBetweenDelimiters	(const std::string& str, const std::string& startDelim, const std::string& endDelim);
}