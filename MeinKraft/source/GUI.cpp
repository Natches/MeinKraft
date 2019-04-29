#include "GUI.h"
#include "Shader2D.h"
#include "Inventory.h"
#include "InventorySlot.h"
#include "Text.h"
#include "Menu.h"


void GUI::BuildMouseTextVAO()
{
	int size = 0;
	m_mouseTextPoints = 0;

	size += m_inventory->GetItemDescription()->GetVertex().size();
	m_mouseTextPoints += m_inventory->GetItemDescription()->GetPoints();
	GLuint m_mouseTextVBO, m_mouseTextUVBO;
	glBindVertexArray(m_mouseTextVAO);
	glCreateBuffers(1, &m_mouseTextVBO);
	glCreateBuffers(1, &m_mouseTextUVBO);


	std::vector<GLfloat> vboData;
	vboData.reserve(size);
	std::vector<GLfloat> uvboData;
	uvboData.reserve(size);
	for (unsigned int j = 0; j <m_inventory->GetItemDescription()->GetVertex().size(); ++j)
	{
		vboData.push_back(m_inventory->GetItemDescription()->GetVertex()[j]);
		uvboData.push_back(m_inventory->GetItemDescription()->GetUVData()[j]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_mouseTextVBO);
	glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(GLfloat), vboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_mouseTextUVBO);
	glBufferData(GL_ARRAY_BUFFER, uvboData.size() * sizeof(GLfloat), uvboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

void GUI::BuildTextVAO()
{
	int size = 0;
	m_textTotalPoints = 0;

	for (unsigned int i = 0; i < m_text.size(); ++i)
	{
		size += m_text[i]->GetVertex().size();
		m_textTotalPoints += m_text[i]->GetPoints();
	}
	if (!m_menu->IsVisible())
	{
		size += m_Crosshair->GetVertex().size();
		m_textTotalPoints += 6;
	}
	else
	{
		for (unsigned int i = 0; i < m_menuText.size(); ++i)
		{
			size += m_menuText[i]->GetVertex().size();
			m_textTotalPoints += m_menuText[i]->GetPoints();
		}
	}

	GLuint m_textVBO, m_textUVBO;
	glBindVertexArray(m_textVAO);
	glCreateBuffers(1, &m_textVBO);
	glCreateBuffers(1, &m_textUVBO);


	std::vector<GLfloat> vboData;
	vboData.reserve(size);
	std::vector<GLfloat> uvboData;
	uvboData.reserve(size);
	for (unsigned int i = 0; i < m_text.size(); ++i)
	{
		for (unsigned int j = 0; j < m_text[i]->GetVertex().size(); ++j)
		{
			vboData.push_back(m_text[i]->GetVertex()[j]);
			uvboData.push_back(m_text[i]->GetUVData()[j]);
		}
	}
	if (!m_menu->IsVisible())
	{
		for (unsigned int i = 0; i < m_Crosshair->GetVertex().size(); ++i)
		{
			vboData.push_back(m_Crosshair->GetVertex()[i]);
			uvboData.push_back(m_Crosshair->GetUVData()[i]);
		}
	}
	else
	{
		for (unsigned int i = 0; i < m_menuText.size(); ++i)
		{
			for (unsigned int j = 0; j < m_menuText[i]->GetVertex().size(); ++j)
			{
				vboData.push_back(m_menuText[i]->GetVertex()[j]);
				uvboData.push_back(m_menuText[i]->GetUVData()[j]);
			}
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
	glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(GLfloat), vboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_textUVBO);
	glBufferData(GL_ARRAY_BUFFER, uvboData.size() * sizeof(GLfloat), uvboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

void GUI::BuildInvTextVAO()
{
	int size = 0;
	m_invTextTotalPoints = 0;
	for (unsigned int i = 0; i < m_inventory->GetInventorySlot().size(); ++i)
	{
		size += m_inventory->GetInventorySlot()[i]->GetText()->GetVertex().size();
		m_invTextTotalPoints += m_inventory->GetInventorySlot()[i]->GetText()->GetPoints();
		m_HasText = true;
	}
	GLuint m_inventoryTextVBO, m_inventoryTextUVBO;
	glBindVertexArray(m_inventoryTextVAO);
	glCreateBuffers(1, &m_inventoryTextVBO);
	glCreateBuffers(1, &m_inventoryTextUVBO);


	std::vector<GLfloat> vboData;
	vboData.reserve(size);
	std::vector<GLfloat> uvboData;
	uvboData.reserve(size);
	for (unsigned int i = 0; i < m_inventory->GetInventorySlot().size(); ++i)
	{
		for (unsigned int j = 0; j < m_inventory->GetInventorySlot()[i]->GetText()->GetVertex().size(); ++j)
		{
			vboData.push_back(m_inventory->GetInventorySlot()[i]->GetText()->GetVertex()[j]);
			uvboData.push_back(m_inventory->GetInventorySlot()[i]->GetText()->GetUVData()[j]);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_inventoryTextVBO);
	glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(GLfloat), vboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_inventoryTextUVBO);
	glBufferData(GL_ARRAY_BUFFER, uvboData.size() * sizeof(GLfloat), uvboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

void GUI::BuildInvVAO()
{
	int size = 0;
	m_invTotalPoints = 0;
	for (unsigned int i = 0; i < m_inventory->GetInventorySlot().size(); ++i)
	{
		size += m_inventory->GetInventorySlot()[i]->GetVertex().size();
		m_invTotalPoints += m_inventory->GetInventorySlot()[i]->GetPoints();
		if (m_inventory->GetInventorySlot()[i]->GetItemStack())
		{
			size += m_inventory->GetInventorySlot()[i]->GetItemVertex().size();
			m_invTotalPoints += 6;
		}
	}
	size += m_inventory->GetInventorySlot()[0]->GetVertex().size();
	m_invTotalPoints += m_inventory->GetInventorySlot()[0]->GetPoints();

	GLuint m_invVBO, m_invUVBO;
	glBindVertexArray(m_invVAO);
	glCreateBuffers(1, &m_invVBO);
	glCreateBuffers(1, &m_invUVBO);

	std::vector<GLfloat> vboData;
	vboData.reserve(size);
	std::vector<GLfloat> uvboData;
	uvboData.reserve(size);
	for (unsigned int i = 0; i < m_inventory->GetInventorySlot().size(); ++i)
	{
		for (unsigned int j = 0; j < m_inventory->GetInventorySlot()[i]->GetVertex().size(); ++j)
		{
			vboData.push_back(m_inventory->GetInventorySlot()[i]->GetVertex()[j]);
			uvboData.push_back(m_inventory->GetInventorySlot()[i]->GetUVData()[j]);
		}
		if (m_inventory->GetInventorySlot()[i]->GetItemStack())
		{
			for (unsigned int j = 0; j < m_inventory->GetInventorySlot()[i]->GetVertex().size(); ++j)
			{
				vboData.push_back(m_inventory->GetInventorySlot()[i]->GetItemVertex()[j]);
				uvboData.push_back(m_inventory->GetInventorySlot()[i]->GetItemUVData()[j]);
			}
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_invVBO);
	glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(GLfloat), vboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_invUVBO);
	glBufferData(GL_ARRAY_BUFFER, uvboData.size() * sizeof(GLfloat), uvboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	m_inventory->SetIsDirty(false);
}

void GUI::BuildPickedItemVAO()
{
	int size = 0;
	size += m_inventory->GetPickUpItemVerticies().size();
	GLuint m_PickedItemVBO, m_PickedItemUVBO;
	glBindVertexArray(m_PickedItemVAO);
	glCreateBuffers(1, &m_PickedItemVBO);
	glCreateBuffers(1, &m_PickedItemUVBO);

	std::vector<GLfloat> vboData;
	vboData.reserve(size);
	std::vector<GLfloat> uvboData;
	uvboData.reserve(size);
	for (unsigned int i = 0; i <m_inventory->GetPickUpItemVerticies().size(); ++i)
	{
		vboData.push_back(m_inventory->GetPickUpItemVerticies()[i]);
		uvboData.push_back(m_inventory->GetPickUpItemUVs()[i]);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_PickedItemVBO);
	glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(GLfloat), vboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_PickedItemUVBO);
	glBufferData(GL_ARRAY_BUFFER, uvboData.size() * sizeof(GLfloat), uvboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	m_inventory->SetIsDirtyPickedItem(false);
}

void GUI::BuildHotbarVAO()
{
	int size = 0;
	int textSize = 0;
	m_hotbarTextTotalPoints = 0;
	m_hotbarTotalPoints = 0;
	for (unsigned int i = 0; i < m_inventory->GetHotbar().size(); ++i)
	{
		size += m_inventory->GetHotbar()[i]->GetVertex().size();
		m_hotbarTotalPoints += m_inventory->GetHotbar()[i]->GetPoints();
		if (m_inventory->GetHotbar()[i]->GetItemStack())
		{
			size += m_inventory->GetHotbar()[i]->GetItemVertex().size();
			m_hotbarTotalPoints += 6;
			textSize += m_inventory->GetHotbar()[i]->GetText()->GetVertex().size();
			m_hotbarTextTotalPoints += m_inventory->GetHotbar()[i]->GetText()->GetPoints();
		}
	}
	//Add size for selected slot
	size += m_inventory->GetHotbar()[0]->GetVertex().size();
	m_hotbarTotalPoints += m_inventory->GetHotbar()[0]->GetPoints();

	GLuint m_HBVBO, m_HBUVBO;
	glBindVertexArray(m_HBVAO);
	glCreateBuffers(1, &m_HBVBO);
	glCreateBuffers(1, &m_HBUVBO);

	std::vector<GLfloat> vboData;
	vboData.reserve(size);
	std::vector<GLfloat> uvboData;
	uvboData.reserve(size);

	std::vector<GLfloat> TextVboData;
	TextVboData.reserve(textSize);
	std::vector<GLfloat> TextUvboData;
	TextUvboData.reserve(textSize);

	for (unsigned int i = 0; i <m_inventory->GetHotbar().size(); ++i)
	{
		for (unsigned int j = 0; j < m_inventory->GetHotbar()[i]->GetVertex().size(); ++j)
		{
			vboData.push_back(m_inventory->GetHotbar()[i]->GetVertex()[j]);
			uvboData.push_back(m_inventory->GetHotbar()[i]->GetUVData()[j]);
		}
		if (m_inventory->GetHotbar()[i]->GetItemStack())
		{
			for (unsigned int j = 0; j < m_inventory->GetHotbar()[i]->GetVertex().size(); ++j)
			{
				vboData.push_back(m_inventory->GetHotbar()[i]->GetItemVertex()[j]);
				uvboData.push_back(m_inventory->GetHotbar()[i]->GetItemUVData()[j]);
			}
			for (unsigned int j = 0; j < m_inventory->GetHotbar()[i]->GetText()->GetVertex().size(); ++j)
			{
				TextVboData.push_back(m_inventory->GetHotbar()[i]->GetText()->GetVertex()[j]);
				TextUvboData.push_back(m_inventory->GetHotbar()[i]->GetText()->GetUVData()[j]);
			}
		}
		if (m_inventory->GetSelectedHotbarSlotID() == i)
		{
			for (unsigned int j = 0; j < m_inventory->GetHotbar()[i]->GetVertex().size(); ++j)
			{
				vboData.push_back(m_inventory->GetSelectedVerticies()[j]);
				uvboData.push_back(m_inventory->GetSelectedUVs()[j]);
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_HBVBO);
	glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(GLfloat), vboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_HBUVBO);
	glBufferData(GL_ARRAY_BUFFER, uvboData.size() * sizeof(GLfloat), uvboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint m_HBTextVBO, m_HBTextUVBO;
	glBindVertexArray(m_HBTextVAO);
	glCreateBuffers(1, &m_HBTextVBO);
	glCreateBuffers(1, &m_HBTextUVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_HBTextVBO);
	glBufferData(GL_ARRAY_BUFFER, TextVboData.size() * sizeof(GLfloat), TextVboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_HBTextUVBO);
	glBufferData(GL_ARRAY_BUFFER, TextUvboData.size() * sizeof(GLfloat), TextUvboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

void GUI::BuildMenu()
{
	int size = 0;
	int titleSize = 0;
	m_menuPoints = 0;
	for (unsigned int i = 0; i < m_menu->GetButtons().size(); ++i)
	{
		size += m_menu->GetButtons()[i]->GetVertexData().size();
		m_menuPoints += 6;
	}
	titleSize += m_menu->GetTitleVertex().size();

	std::vector<GLfloat> vboData;
	vboData.reserve(size);
	std::vector<GLfloat> uvboData;
	uvboData.reserve(size);

	std::vector<GLfloat> titlevboData;
	vboData.reserve(titleSize);
	std::vector<GLfloat> titleuvboData;
	uvboData.reserve(titleSize);
	for (unsigned int i = 0; i < m_menu->GetButtons().size(); ++i)
	{
		for (unsigned int j = 0; j < m_menu->GetButtons()[i]->GetVertexData().size(); ++j)
		{
			vboData.push_back(m_menu->GetButtons()[i]->GetVertexData()[j]);
			uvboData.push_back(m_menu->GetButtons()[i]->GetUVData()[j]);
		}
	}
	for (unsigned int i = 0; i < m_menu->GetTitleVertex().size(); ++i)
	{
		titlevboData.push_back(m_menu->GetTitleVertex()[i]);
		titleuvboData.push_back(m_menu->GetTitleUvs()[i]);
	}
	GLuint m_menuVBO, m_menuUVBO;
	glBindVertexArray(m_menuVAO);
	glCreateBuffers(1, &m_menuVBO);
	glCreateBuffers(1, &m_menuUVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_menuVBO);
	glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(GLfloat), vboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_menuUVBO);
	glBufferData(GL_ARRAY_BUFFER, uvboData.size() * sizeof(GLfloat), uvboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint m_titleVBO, m_titleUVBO;
	glBindVertexArray(m_titleVAO);
	glCreateBuffers(1, &m_titleVBO);
	glCreateBuffers(1, &m_titleUVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_titleVBO);
	glBufferData(GL_ARRAY_BUFFER, titlevboData.size() * sizeof(GLfloat), titlevboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, m_titleUVBO);
	glBufferData(GL_ARRAY_BUFFER, titleuvboData.size() * sizeof(GLfloat), titleuvboData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

}

GUI::GUI()
{
	m_textColour = vecs::Vec4(1.f, 1.f, 1.f, 1.f);
	m_invColour = vecs::Vec4(1.f, 1.f, 1.f, 1.f);;
	m_textBufferSize = 0;
	m_inventoryBufferSize = 0;
	m_invTextTotalPoints = 0;
	m_textTotalPoints = 0;
	m_invTotalPoints = 0;
	m_hotbarTextTotalPoints = 0;
	m_hotbarTotalPoints = 0;
	m_mouseTextPoints = 0;
	m_menuPoints = 0;
	m_HasText = false;
	glCreateVertexArrays(1, &m_textVAO);
	glCreateVertexArrays(1, &m_inventoryTextVAO);
	glCreateVertexArrays(1, &m_invVAO);
	glCreateVertexArrays(1, &m_mouseTextVAO);
	glCreateVertexArrays(1, &m_PickedItemVAO);
	glCreateVertexArrays(1, &m_HBTextVAO);
	glCreateVertexArrays(1, &m_HBVAO);
	glCreateVertexArrays(1, &m_menuVAO);
	glCreateVertexArrays(1, &m_titleVAO);
}

GUI::~GUI()
{
}

void GUI::Draw(Shader* shader)
{
	ShaderParameter sP;
	shader->Update(sP);
	glDisable(GL_DEPTH_TEST);

	if (m_menu->IsVisible())
	{
		glBindVertexArray(m_menuVAO);
		glActiveTexture(GL_TEXTURE0 + m_menuTexture);
		glBindTexture(GL_TEXTURE_2D, m_menuTexture);
		glUniform1i(((Shader2D*)shader)->GetUniformTextureLocation(), m_menuTexture);
		glUniform4f(((Shader2D*)shader)->GetUniformColourLocation(), m_invColour.x, m_invColour.y, m_invColour.z, m_invColour.w);
		glDrawArrays(GL_TRIANGLES, 0, m_menuPoints);

		glBindVertexArray(m_titleVAO);
		glActiveTexture(GL_TEXTURE0 + m_titleTexture);
		glBindTexture(GL_TEXTURE_2D, m_titleTexture);
		glUniform1i(((Shader2D*)shader)->GetUniformTextureLocation(), m_titleTexture);
		glUniform4f(((Shader2D*)shader)->GetUniformColourLocation(), m_invColour.x, m_invColour.y, m_invColour.z, m_invColour.w);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else
	{
		if (m_inventory->GetInventorySlot().size() > 0)
		{
			if (m_inventory->IsDirty())
			{
				BuildInvVAO();
				BuildInvTextVAO();
				BuildHotbarVAO();
			}
			if (m_inventory->IsVisible())
			{
				glBindVertexArray(m_invVAO);
				glActiveTexture(GL_TEXTURE0 + m_invTexture);
				glBindTexture(GL_TEXTURE_2D, m_invTexture);
				glUniform1i(((Shader2D*)shader)->GetUniformTextureLocation(), m_invTexture);
				glUniform4f(((Shader2D*)shader)->GetUniformColourLocation(), m_invColour.x, m_invColour.y, m_invColour.z, m_invColour.w);
				glDrawArrays(GL_TRIANGLES, 0, m_invTotalPoints);

				glBindVertexArray(m_inventoryTextVAO);
				glActiveTexture(GL_TEXTURE0 + m_font->GetTexture());
				glBindTexture(GL_TEXTURE_2D, m_font->GetTexture());
				glUniform1i(((Shader2D*)shader)->GetUniformTextureLocation(), m_font->GetTexture());
				glUniform4f(((Shader2D*)shader)->GetUniformColourLocation(), m_textColour.x, m_textColour.y, m_textColour.z, m_textColour.w);
				glDrawArrays(GL_TRIANGLES, 0, m_invTextTotalPoints);
			}
			else
			{
				glBindVertexArray(m_HBVAO);
				glActiveTexture(GL_TEXTURE0 + m_invTexture);
				glBindTexture(GL_TEXTURE_2D, m_invTexture);
				glUniform1i(((Shader2D*)shader)->GetUniformTextureLocation(), m_invTexture);
				glUniform4f(((Shader2D*)shader)->GetUniformColourLocation(), m_invColour.x, m_invColour.y, m_invColour.z, m_invColour.w);
				glDrawArrays(GL_TRIANGLES, 0, m_hotbarTotalPoints);

				glBindVertexArray(m_HBTextVAO);
				glActiveTexture(GL_TEXTURE0 + m_font->GetTexture());
				glBindTexture(GL_TEXTURE_2D, m_font->GetTexture());
				glUniform1i(((Shader2D*)shader)->GetUniformTextureLocation(), m_font->GetTexture());
				glUniform4f(((Shader2D*)shader)->GetUniformColourLocation(), m_textColour.x, m_textColour.y, m_textColour.z, m_textColour.w);
				glDrawArrays(GL_TRIANGLES, 0, m_hotbarTextTotalPoints);
			}
		}

		if (m_inventory->GetItemDescription() != nullptr)
		{
			if (m_inventory->IsVisible())
			{
				if (m_inventory->IsMouseTouchingItem())
				{
					if (m_inventory->IsItemDescriptionDirty())
					{
						BuildMouseTextVAO();
						m_inventory->SetIsItemDescriptionDirty(false);
					}
					glBindVertexArray(m_mouseTextVAO);
					glActiveTexture(GL_TEXTURE0 + m_font->GetTexture());
					glBindTexture(GL_TEXTURE_2D, m_font->GetTexture());
					glUniform1i(((Shader2D*)shader)->GetUniformTextureLocation(), m_font->GetTexture());
					glUniform4f(((Shader2D*)shader)->GetUniformColourLocation(), m_textColour.x, m_textColour.y, m_textColour.z, m_textColour.w);
					glDrawArrays(GL_TRIANGLES, 0, m_mouseTextPoints);
				}
			}
		}
		if (m_inventory->GetPickedUpStack() != nullptr)
		{
			if (m_inventory->IsVisible())
			{
				if (m_inventory->IsPickedItemImageDirty())
					BuildPickedItemVAO();

				glBindVertexArray(m_PickedItemVAO);
				glActiveTexture(GL_TEXTURE0 + m_invTexture);
				glBindTexture(GL_TEXTURE_2D, m_invTexture);
				glUniform1i(((Shader2D*)shader)->GetUniformTextureLocation(), m_invTexture);
				glUniform4f(((Shader2D*)shader)->GetUniformColourLocation(), m_invColour.x, m_invColour.y, m_invColour.z, m_invColour.w);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
	}
	if (m_text.size() > 0 || m_menuText.size() > 0)
	{
		if (m_dirtyText)
			BuildTextVAO();
		glBindVertexArray(m_textVAO);
		glActiveTexture(GL_TEXTURE0 + m_font->GetTexture());
		glBindTexture(GL_TEXTURE_2D, m_font->GetTexture());
		glUniform1i(((Shader2D*)shader)->GetUniformTextureLocation(), m_font->GetTexture());
		if(m_menu->IsVisible())
			glUniform4f(((Shader2D*)shader)->GetUniformColourLocation(), 0,0,0, m_textColour.w);
		else
			glUniform4f(((Shader2D*)shader)->GetUniformColourLocation(), m_textColour.x, m_textColour.y, m_textColour.z, m_textColour.w);
		glDrawArrays(GL_TRIANGLES, 0, m_textTotalPoints);
	}

	glEnable(GL_DEPTH_TEST);
}

void GUI::SetInvTexture(const char * path)
{
	int x = 0, y = 0, n = 0;
	int force_channels = 4;
	unsigned char * image_data = SOIL_load_image(path, &x, &y, 0, SOIL_LOAD_RGBA);
	//unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		return;
	}
	glGenTextures(1, &m_invTexture);
	glActiveTexture(GL_TEXTURE0 + m_invTexture);
	glBindTexture(GL_TEXTURE_2D, m_invTexture);
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
}

void GUI::SetMenuTexture(char * path)
{
	int x = 0, y = 0, n = 0;
	int force_channels = 4;
	unsigned char * image_data = SOIL_load_image(path, &x, &y, 0, SOIL_LOAD_RGBA);
	//unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		return;
	}
	glGenTextures(1, &m_menuTexture);
	glActiveTexture(GL_TEXTURE0 + m_menuTexture);
	glBindTexture(GL_TEXTURE_2D, m_menuTexture);
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
}
