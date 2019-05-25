#pragma once
#include <vector>
#include "Entity.h"

class Inventory;
class Font;
class Game;

class Player final : public Entity
{
#pragma region Non-Static Attributes
	Inventory*				m_inventory;
#pragma endregion
#pragma region Static Attributes
public:
	static const unsigned int	BREAK_RANGE;
	static const float			PLACE_TIME;
#pragma endregion
#pragma region Non-Static Methods
public:
	Player();
	~Player();

	void Init();

	void Update();
	void LoadSave(const char * saveFilePath);

private:
	void SaveData(const char * saveFilePath);
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
public:
	inline Inventory*	GetInventory() { return m_inventory; }
#pragma endregion
};

