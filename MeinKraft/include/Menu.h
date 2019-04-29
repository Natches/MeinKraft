#pragma once
#include "Button.h"
#include <vector>
class Game;
class GUI;
class Menu
{
	GLuint	m_titleTexture;
	std::vector<GLfloat> m_TitleVertex;
	std::vector<GLfloat> m_titleUVs;
	std::vector<Button*> m_buttons;
	bool m_isVisible;
	Game* m_game;
	GUI* m_gui;
public:
	Menu(Font* font, Game* game, GUI* gui);
	~Menu();
	inline bool IsVisible() { return m_isVisible; }
	void OpenMenu();
	void CloseMenu();
	inline std::vector<Button*>& GetButtons() { return m_buttons; }
	void Update();
	void ClickEvent();
	inline std::vector<GLfloat>& GetTitleVertex() { return m_TitleVertex; }
	inline std::vector<GLfloat>& GetTitleUvs() { return m_titleUVs; }
};

