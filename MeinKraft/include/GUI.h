#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include "Vec4.h"
#include "Menu.h"

class Inventory;
class Shader2D;
class Text;
class Font;
class Shader;

class GUI
{
#pragma region Non-Static Attributes
	std::vector<Text*>	m_text;
	std::vector<Text*>	m_menuText;
	vecs::Vec4			m_textColour;
	vecs::Vec4			m_invColour;
	Text*				m_Crosshair;
	Inventory*			m_inventory;
	Font*				m_font;
	Menu*				m_menu;
	int					m_textBufferSize;
	int					m_inventoryBufferSize;
	int					m_textTotalPoints;
	int					m_invTextTotalPoints;
	int					m_hotbarTextTotalPoints;
	int					m_hotbarTotalPoints;
	int					m_invTotalPoints;
	int					m_mouseTextPoints;
	int					m_menuPoints;
	GLuint				m_HBTextVAO;
	GLuint				m_HBVAO;
	GLuint				m_menuVAO;
	GLuint				m_titleVAO;
	GLuint				m_textVAO;
	GLuint				m_mouseTextVAO;
	GLuint				m_inventoryTextVAO;
	GLuint				m_invVAO;
	GLuint				m_PickedItemVAO;
	GLuint				m_invTexture;
	GLuint				m_menuTexture;
	GLuint				m_titleTexture;
	bool				m_HasText;
	bool				m_dirtyText;
#pragma endregion
#pragma region Non-Static Methods
	void BuildMouseTextVAO();
	void BuildTextVAO();
	void BuildInvTextVAO();
	void BuildInvVAO();
	void BuildPickedItemVAO();
	void BuildHotbarVAO();
	void BuildMenu();
public:
	GUI();
	~GUI();
	inline void SetCrosshair(Text* t) { m_Crosshair = t; m_dirtyText = true; }
	inline void AddText(Text* t) { m_text.push_back(t); m_dirtyText = true; }
	void Draw(Shader* shader);
#pragma endregion
#pragma region Getters & Setters
	void SetInvTexture(const char * path);
	void SetMenuTexture(const char * path);
	inline void TextIsDirty() { m_dirtyText = true; }
	inline void SetFont(Font* f) { m_font = f; }
	inline void SetInventory(Inventory * inv) { m_inventory = inv; }
	inline void SetMenu(Menu* menu) 
	{ 
		m_menu = menu; 
		for (unsigned int i = 0; i < m_menu->GetButtons().size(); ++i) 
		{ 
			m_menuText.push_back(m_menu->GetButtons()[i]->GetText()); 
		} 
		BuildMenu(); 
		BuildTextVAO(); 
	}
	inline void SetTitleTexture(GLuint ID) { m_titleTexture = ID; }
#pragma endregion
};

