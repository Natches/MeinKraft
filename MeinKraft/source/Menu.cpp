#include "Menu.h"
#include "Inputs.h"
#include "Game.h"
#include "GUI.h"
#include "Inventory.h"
#include "INISettings.h"
#include "Text.h"

Menu::Menu(Font* font, Game* game, GUI* gui):
	m_game(game),
	m_gui(gui)
{
	m_isVisible = false;
	vecs::Vec2 buttonDimensions(500, 300);
	float QuitOffset = 80.f;
	m_buttons.push_back(new Button(std::bind(&Menu::CloseMenu, this), "Play", font, vecs::Vec2(0 - buttonDimensions.x / INISettings::windowWidth / 2, 0), buttonDimensions));
	m_buttons.push_back(new Button(std::bind(&Game::ExitGame, m_game), "Quit", font, vecs::Vec2(0 - buttonDimensions.x / INISettings::windowWidth / 2, 0 - buttonDimensions.y/INISettings::windowHeight - QuitOffset / INISettings::windowHeight), buttonDimensions));

	int x = 0, y = 0;
	Shader::LoadTexture(m_titleTexture, "Assets/title.png", &x, &y);
	m_gui->SetTitleTexture(m_titleTexture);

	vecs::Vec2 pos(-0.5f, 0.9f);
	vecs::Vec2 dimensions(800.f/INISettings::windowWidth * 2 , 350.f/INISettings::windowHeight * 2);

	m_TitleVertex.push_back(pos.x);
	m_TitleVertex.push_back(pos.y);

	m_TitleVertex.push_back(pos.x);
	m_TitleVertex.push_back(pos.y - dimensions.y);

	m_TitleVertex.push_back(pos.x + dimensions.x);
	m_TitleVertex.push_back(pos.y - dimensions.y);
	
	m_TitleVertex.push_back(pos.x + dimensions.x);
	m_TitleVertex.push_back(pos.y - dimensions.y);

	m_TitleVertex.push_back(pos.x + dimensions.x);
	m_TitleVertex.push_back(pos.y);

	m_TitleVertex.push_back(pos.x);
	m_TitleVertex.push_back(pos.y);

	m_titleUVs.push_back(0);
	m_titleUVs.push_back(0);

	m_titleUVs.push_back(0);
	m_titleUVs.push_back(1);

	m_titleUVs.push_back(1);
	m_titleUVs.push_back(1);

	m_titleUVs.push_back(1);
	m_titleUVs.push_back(1);

	m_titleUVs.push_back(1);
	m_titleUVs.push_back(0);

	m_titleUVs.push_back(0);
	m_titleUVs.push_back(0);
}


Menu::~Menu()
{
	for (unsigned int i = 0; i < m_buttons.size(); ++i)
	{
		delete m_buttons[i];
	}
}

void Menu::OpenMenu()
{
	if (!m_isVisible)
	{
		m_isVisible = true;
		Inputs::BindInput(GLFW_MOUSE_BUTTON_LEFT, std::bind(&Menu::ClickEvent, this), false, 0.1f);
		if (!m_game->GetPlayer().GetInventory()->IsVisible())
		{
			Inputs::ToggleMouseLock();
			Inputs::ToggleMouseVisibility();
		}
	}
}

void Menu::CloseMenu()
{
	m_isVisible = false;
	Inputs::BindInput(GLFW_MOUSE_BUTTON_LEFT, std::bind(&Game::BreakBlock, m_game), false);
	Inputs::ToggleMouseLock();
	Inputs::ToggleMouseVisibility();
	m_gui->TextIsDirty();
}

void Menu::Update()
{

}

void Menu::ClickEvent()
{
	vecs::Vec2 positionOfMouse = Inputs::GetMousePosition();
	positionOfMouse.x -= INISettings::windowWidth / 2.f;
	positionOfMouse.x /= INISettings::windowWidth / 2.f;
	positionOfMouse.y -= INISettings::windowHeight / 2.f;
	positionOfMouse.y /= INISettings::windowHeight / 2.f;
	positionOfMouse.y *= -1;
	for (unsigned int i = 0; i < m_buttons.size(); ++i)
	{
		if (m_buttons[i]->IsTouchingMouse(positionOfMouse))
		{
			m_buttons[i]->Activate();
			break;
		}
	}
}
