#include <iostream>
#include "Renderer.h"
#include "INISettings.h"

namespace Renderer
{
	GLFWwindow* InitGraphicLibraries()
	{
		GLFWwindow* window;

		if (!InitGLFW())
		{
			exit(EXIT_FAILURE);
		}
		if (!InitWindow(window))
		{
			exit(EXIT_FAILURE);
		}

		InitGLEW();

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		InitDepth();

		return window;
	}

	bool InitGLFW()
	{
		// Create a GL context then a window using GLFW
		if (!glfwInit()) {
			std::cout << "ERROR: could not start GLFW3" << std::endl;
			return false;
		}
		return true;
	}

	bool InitWindow(GLFWwindow*& window)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(INISettings::windowWidth, INISettings::windowHeight, "MeinKraft", INISettings::windowMode?NULL:glfwGetPrimaryMonitor(), NULL);
		//Fullscreen
		/*window = glfwCreateWindow(std::stoi(configData[ConfigTags::WINDOW_WIDTH]),
			std::stoi(configData[ConfigTags::WINDOW_HEIGHT]),
			configData[ConfigTags::WINDOW_TITLE].c_str(),
			glfwGetPrimaryMonitor(),
			NULL);*/

		if (!window)
		{
			std::cout << "ERROR: could not open window with GLFW3" << std::endl;
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);
		return true;
	}

	void InitGLEW()
	{
		const GLubyte* renderer;
		const GLubyte* version;

		// Start GLEW extension handler = load all OpenGL functions
		glewExperimental = GL_TRUE;
		glewInit();

		// Check if not running on old device
		renderer = glGetString(GL_RENDERER);
		version = glGetString(GL_VERSION);
		std::cout << "Renderer: " << renderer << std::endl;
		std::cout << "OpenGL version supported " << version << std::endl;
	}

	void InitDepth()
	{
		// Turning on Depth Buffer
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CCW);
		glDepthFunc(GL_LESS); // interprets a smaller value as "closer"
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);
	}

	void CleanGraphicLibraries()
	{
		glfwTerminate();
	}

	void BackgroundColor(const float* color)
	{
		glClearColor(color[0] / 255.f, color[1] / 255.f, color[2] / 255.f, 1.f);
	}
}