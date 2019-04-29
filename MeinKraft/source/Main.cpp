#include <iostream>
#include <chrono>
#include "Game.h"
#include "INISettings.h"
#include "Renderer.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "PoolThread.h"

#ifdef _DEBUG
#include "vld.h"
#endif // DEBUG

static std::random_device rd;

void add() {
	for (int i = 0; i < 100000; ++i);
}

int main(int argv, char** argc)
{
	//system("pause");
	INISettings::LoadConfigFile("INIConfig.ini");

	GLFWwindow* window = Renderer::InitGraphicLibraries();


	if (window)
	{
		Game game(window);
		game.Run();
		Renderer::CleanGraphicLibraries();
		ThreadLib::PoolThread::getInstance().FinishAllTaskThenContinue();
	}
	ThreadLib::PoolThread::getInstance().JoinAllThread();
	system("pause");
	return EXIT_SUCCESS;
}