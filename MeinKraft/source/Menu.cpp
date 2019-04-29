#include "Menu.h"
#include "Inputs.h"
#include "Game.h"
#include "GUI.h"
#include "Inventory.h"

Menu::Menu(Font* font, Game* game, GUI* gui):
	m_game(game),
	m_gui(gui)
{
	m_isVisible = false;
	vecs::Vec2 buttonDimensions(500, 300);
	float QuitOffset = 80.f;
	m_buttons.push_back(new Button(std::bind(&Menu::CloseMenu, this), "Play", font, vecs::Vec2(0 - buttonDimensions.x / INISettings::windowWidth / 2, 0), buttonDimensions));
	m_buttons.push_back(new Button(std::bind(&Game::ExitGame, m_game), "Quit", font, vecs::Vec2(0 - buttonDimensions.x / INISettings::windowWidth / 2, 0 - buttonDimensions.y/INISettings::windowHeight - QuitOffset / INISettings::windowHeight), buttonDimensions));
	
	int x = 0, y = 0, n = 0;
	int force_channels = 4;
	unsigned char * image_data = SOIL_load_image("Assets/title.png", &x, &y, 0, SOIL_LOAD_RGBA);
	//unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		return;
	}
	glGenTextures(1, &m_titleTexture);
	glActiveTexture(GL_TEXTURE0 + m_titleTexture);
	glBindTexture(GL_TEXTURE_2D, m_titleTexture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
	);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
	SOIL_free_image_data(image_data);
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
