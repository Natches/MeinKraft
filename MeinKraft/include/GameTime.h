#pragma once
#include <atomic>
#include <chrono>

class GameTime final
{
#pragma region Non-Static Attributes
private:
	std::atomic<float> m_deltaTime;
	std::atomic<float> m_timeScale;
	std::chrono::time_point<std::chrono::steady_clock> m_previousTime;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	~GameTime() = default;

	void UpdateDeltaTime();

private:
	GameTime();
#pragma endregion
#pragma region Static Methods
public:
	static GameTime* Instance();
#pragma endregion
#pragma region Getters & Setters
public:
	inline const float GetDeltaTime() { return m_deltaTime; }
	inline const float GetTimeScale() { return m_timeScale; }

	inline void SetTimeScale(const float timeScale) { m_timeScale = timeScale; }
#pragma endregion
};