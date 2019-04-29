#include "Inputs.h"
#include "GLFW\glfw3.h"
#include "INISettings.h"
#include "GameTime.h"

bool							Inputs::m_mouseLocked	= true;
vecs::Vec2						Inputs::m_mousePos;
vecs::Vec2						Inputs::m_oldMousePos;
bool							Inputs::m_mouseVisible	= true;
GLFWwindow*						Inputs::mp_window		= nullptr;
std::map<KEY_CODE, InputData>	Inputs::m_keyData;
int								Inputs::m_scrollState	= 0u;

Inputs::Inputs()
{
	Inputs::BindInput(GLFW_MOUSE_BUTTON_LEFT, [] {});
	Inputs::BindInput(GLFW_MOUSE_BUTTON_RIGHT, [] {});
	Inputs::BindInput(GLFW_MOUSE_BUTTON_MIDDLE, [] {});
}

Inputs::~Inputs()
{
}

void Inputs::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key != GLFW_KEY_UNKNOWN) 
	{ 
		m_keyData[key].oldState = m_keyData[key].state;
		m_keyData[key].state	= action;
	}
}

void Inputs::StoreMousePosition()
{
	m_oldMousePos = m_mousePos;
	double x, y;
	glfwGetCursorPos(mp_window, &x, &y);
	m_mousePos.x = (float)x;
	m_mousePos.y = (float)y;
	if(m_mouseLocked)
		CenterMouse();
}

void Inputs::StoreMouseClicks()
{
	m_keyData[GLFW_MOUSE_BUTTON_LEFT].oldState		= m_keyData[GLFW_MOUSE_BUTTON_LEFT].state;
	m_keyData[GLFW_MOUSE_BUTTON_RIGHT].oldState		= m_keyData[GLFW_MOUSE_BUTTON_RIGHT].state;
	m_keyData[GLFW_MOUSE_BUTTON_MIDDLE].oldState	= m_keyData[GLFW_MOUSE_BUTTON_MIDDLE].state;
	m_keyData[GLFW_MOUSE_BUTTON_LEFT].state			= glfwGetMouseButton(mp_window, GLFW_MOUSE_BUTTON_LEFT);
	m_keyData[GLFW_MOUSE_BUTTON_RIGHT].state		= glfwGetMouseButton(mp_window, GLFW_MOUSE_BUTTON_RIGHT);
	m_keyData[GLFW_MOUSE_BUTTON_MIDDLE].state		= glfwGetMouseButton(mp_window, GLFW_MOUSE_BUTTON_MIDDLE);
}

void Inputs::PollEvents()
{
	m_scrollState = 0;
	glfwPollEvents();
	StoreMousePosition();
	StoreMouseClicks();
}

bool Inputs::GetIsKeyPressed(unsigned int key)
{
	return m_keyData[key].state == GLFW_PRESS;
}

bool Inputs::GetIsKeyReleased(unsigned int key)
{
	return m_keyData[key].state == GLFW_RELEASE;
}

bool Inputs::GetIsKeyRepeated(unsigned int key)
{
	return m_keyData[key].state == GLFW_REPEAT;
}

void Inputs::BindWindow(GLFWwindow* window)
{
	mp_window = window;
	glfwSetKeyCallback(window, &Inputs::KeyCallBack);
}

void Inputs::ToggleMouseVisibility()
{
	m_mouseVisible ? glfwSetInputMode(mp_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN) : glfwSetInputMode(mp_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_mouseVisible = !m_mouseVisible;
}

void Inputs::ToggleMouseLock()
{
	m_mouseLocked = !m_mouseLocked;
}

void Inputs::CenterMouse()
{
	glfwSetCursorPos(mp_window, (float)INISettings::windowWidth / 2.f, (float)INISettings::windowHeight / 2.f);
}

vecs::Vec2& Inputs::GetMousePosition()
{
	return m_mousePos;
}

bool Inputs::HasMouseMoved()
{
	return m_oldMousePos != m_mousePos;
}

void Inputs::UpdateInputs()
{
	for (auto it = m_keyData.begin(), end = m_keyData.end(); it != end; ++it)
	{
		if (it->second.isToggle)
		{
			if (it->second.state == GLFW_RELEASE && it->second.oldState == GLFW_PRESS || it->second.oldState == GLFW_REPEAT)
			{
				if (it->second.func)
				{
					it->second.func();
					it->second.oldState = it->second.state;
				}
			}
		}
		else
		{
			it->second.elapsed += GameTime::Instance()->GetDeltaTime();
			if (it->second.elapsed >= it->second.cooldown)
			{
				if (it->second.state == GLFW_PRESS || it->second.state == GLFW_REPEAT)
				{
					it->second.elapsed = 0.f;
					if (it->second.func)
					{
						it->second.func();
					}
				}
			}
		}
	}
}

void Inputs::BindInput(const KEY_CODE key, const std::function<void()>& func, bool isToggleKey, float cooldown)
{
	m_keyData[key] = InputData(GLFW_RELEASE, GLFW_RELEASE, isToggleKey, func, cooldown);
}

void Inputs::ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	m_scrollState = (int)yoffset;
}