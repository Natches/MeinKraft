#include <iostream>
#include "Game.h"
#include "INISettings.h"
#include "Renderer.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "PoolThread.h"

#ifdef _DEBUG
#include "vld.h"
#endif // DEBUG

int main()
{
	//system("pause");
	INISettings::LoadConfigFile("INIConfig.ini");

	GLFWwindow* window = Renderer::InitGraphicLibraries();

	if (window)
	{
		ThreadLib::PoolThread::getInstance().ChangeMaxWaitingQueueThread(2);
		Game game(window);
		game.Run();
		Renderer::CleanGraphicLibraries();
		ThreadLib::PoolThread::getInstance().FinishAllTaskThenContinue();
	}
	ThreadLib::PoolThread::getInstance().JoinAllThread();
	return EXIT_SUCCESS;
}