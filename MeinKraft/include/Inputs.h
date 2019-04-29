#pragma once
#include <map>
#include <functional>
#include <tuple>
#include "Vec2.h"

struct GLFWwindow;

#define KEY_CODE		unsigned int
#define STATE			unsigned int
#define OLD_STATE		unsigned int
#define IS_KEY_TOGGLE	bool

struct InputData
{
	InputData()
	{
		state		= 0u;
		oldState	= 0u;
		isToggle	= false;
		func		= nullptr;
		cooldown	= 0.f;
		elapsed		= 0.f;
	}

	InputData(STATE aState, OLD_STATE aOldState, IS_KEY_TOGGLE aIsToggle, std::function<void()> aFunc, float aCooldown)
	{
		state		= aState;
		oldState	= aOldState;
		isToggle	= aIsToggle;
		func		= aFunc;
		cooldown	= aCooldown;
		elapsed		= 0.f;
	}

	STATE					state;
	OLD_STATE				oldState;
	IS_KEY_TOGGLE			isToggle;
	float					cooldown;
	float					elapsed;
	std::function<void()>	func;
};

namespace inputs
{
	enum E_SCROLL_STATE
	{
		UP = -1,
		NEUTRAL,
		DOWN
	};
}

class Inputs
{
#pragma region Non-Static Attributes
#pragma endregion
#pragma region Static Attributes
private:
	static bool							m_mouseVisible;
	static vecs::Vec2					m_mousePos;
	static bool							m_mouseLocked;
	static vecs::Vec2					m_oldMousePos;
	static GLFWwindow*					mp_window;
	static int							m_scrollState;
	//Element per position in tuple
	//0: STATE
	//1: OLD_STATE
	//2: IS_KEY_TOGGLE
	//3: FUNCTION TO CALL
	static std::map<KEY_CODE, InputData> m_keyData;
#pragma endregion
#pragma region Non-Static Methods
public:
	Inputs();
	~Inputs();
#pragma endregion
#pragma region Static Methods
public:
	static void			KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void			StoreMousePosition();
	static void			StoreMouseClicks();
	static void			ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
	static bool			GetIsKeyPressed(unsigned int key);
	static bool			GetIsKeyReleased(unsigned int key);
	static bool			GetIsKeyRepeated(unsigned int key);
	static bool			HasMouseMoved();
	static void			BindWindow(GLFWwindow* window);
	static void			PollEvents();
	static void			ToggleMouseVisibility();
	static void			ToggleMouseLock();
	static void			CenterMouse();
	static vecs::Vec2&	GetMousePosition();
	static void			UpdateInputs();
	static void			BindInput(const KEY_CODE key, const std::function<void()>& func, bool isToggleKey = true, float cooldown = 0.f);

	static inline int	GetScrollState() { return m_scrollState; }
#pragma endregion
#pragma region Getters & Setters
#pragma endregion
};