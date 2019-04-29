#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>

namespace Renderer
{
	GLFWwindow*		InitGraphicLibraries();
	bool			InitGLFW();
	bool			InitWindow(GLFWwindow*& window);
	void			InitGLEW();
	void			InitDepth();
	void			CleanGraphicLibraries();
	void			BackgroundColor(const float* color);
}