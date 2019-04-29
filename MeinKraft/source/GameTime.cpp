#include "GameTime.h"

GameTime::GameTime()
{
	m_deltaTime = 0;
	m_timeScale = 1;
	m_previousTime = std::chrono::steady_clock::now();
}

GameTime* GameTime::Instance()
{
	static GameTime* instance = new GameTime();
	return instance;
}

void GameTime::UpdateDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsed = currentTime - m_previousTime;

	m_deltaTime = (std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000000.f) * m_timeScale;

	m_previousTime = currentTime;
}