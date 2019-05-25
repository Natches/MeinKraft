#include "Player.h"
#include "Ray.h"
#include "Inventory.h"
#include "Font.h"
#include "Game.h"
#include "GUI.h"
#include <iostream>
#include <fstream>

const unsigned int Player::BREAK_RANGE	= 4u;
const float Player::PLACE_TIME			= 0.2f;

Player::Player()
	: Entity(entity::E_ENTITY_TYPE::PLAYER)
{
	m_capsule.SetRadius(0.4f);
	m_baseDamage = 5u;
}

Player::~Player()
{
	SaveData("playerData.txt");
	delete m_inventory;
}

void Player::Init()
{
	m_inventory = new Inventory(Game::font);
	Game::gui->SetInventory(m_inventory);
}

void Player::Update()
{
	m_inventory->Update();
}

void Player::SaveData(const char * saveFilePath)
{
	std::ofstream saveFile;
	saveFile.open(saveFilePath);
	std::string c = m_inventory->SaveData();
	saveFile << c;
	saveFile << m_transform.GetPosition().x;
	saveFile << '\n';
	saveFile << m_transform.GetPosition().y;
	saveFile << '\n';
	saveFile << m_transform.GetPosition().z;
	saveFile << '\n';
	saveFile << m_transform.GetRotation().x;
	saveFile << ' ';
	saveFile << m_transform.GetRotation().y;
	saveFile << ' ';
	saveFile << m_transform.GetRotation().z;
	saveFile << '\n';
	saveFile << m_currentLife;
}

void Player::LoadSave(const char * saveFilePath)
{
	std::ifstream saveFile(saveFilePath);
	if (saveFile.good())
	{
		std::string s;
		for (unsigned int i = 0; i < m_inventory->GetInventorySlot().size(); ++i)
		{
			std::getline(saveFile, s);
			m_inventory->LoadData(s, i);
		}
		std::getline(saveFile, s);
		vecs::Vec3 pos;
		pos.x = std::stof(s);
		std::getline(saveFile, s);
		pos.y = std::stof(s);
		std::getline(saveFile, s);
		pos.z = std::stof(s);
		m_transform.SetPosition(pos);
	}
}
