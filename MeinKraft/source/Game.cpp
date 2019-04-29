#include <iostream>
#include <string>
#include <string.h>
#include <windows.h>

#undef TRANSPARENT
#undef ERROR

#include "DefaultErrorShader.h"
#include "CubeShader.h"
#include "Player.h"
#include "Renderer.h"
#include "Game.h"
#include "GameTime.h"
#include "INISettings.h"
#include "Matrix4x4.h"
#include "BlockLibrary.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include "GUI.h"
#include "Font.h"
#include "Inventory.h"
#include "PoolThread.h"
#include "Text.h"
#include "ItemStack.h"

Font*	Game::font = nullptr;
GUI*	Game::gui = nullptr;

Game::Game(GLFWwindow* window)
	: mp_window(window), m_isWireFrame(false)
{
}

Game::~Game()
{
	delete GameTime::Instance();
	delete m_camera;
	delete m_world;
	delete font;
	delete gui;
	glfwDestroyWindow(mp_window);
}

void Game::Init()
{
	Inputs::BindWindow(mp_window);
	InitCamera();
	try
	{
		m_world = new world::World();
	}
	catch (std::bad_alloc& e)
	{
		std::cout << e.what() << std::endl;
	}
	InitUI();
	InitCollisions();
	InitPlayer();
	InitInputs();
	m_player.LoadSave("playerData.txt");
	m_gen = true;
}

void Game::InitPlayer()
{
	m_player.GetTransform().SetPosition(INISettings::cameraPosition);
	m_player.GetCapsule().SetHalfHeight(0.5f);
	m_player.GetCapsule().SetRadius(0.4f);
	m_player.Init();
}

void Game::InitInputs()
{
	BindPlayerInput();
	BindWorldInput();
	BindUIInput();

	glfwSetScrollCallback(mp_window, Inputs::ScrollCallBack);

	Inputs::ToggleMouseVisibility();
}

void Game::InitUI()
{
	font	= new Font("Assets/monospace.png");
	gui		= new GUI();
	gui->SetInvTexture(INISettings::itemTexturePath.c_str());
	gui->SetMenuTexture("Assets/menu.png");
	gui->SetFont(font);
	gui->SetCrosshair(new Text("+", font, 128.f/900.f * INISettings::windowHeight, vecs::Vec2(0 - (128.f / 900.f)/2, 0)));
	m_menu = new Menu(font, this, gui);
	gui->SetMenu(m_menu);
}

void Game::InitCamera()
{
	m_camera = new Camera(&m_player, {0.f, 1.f, 0.f});
	m_camera->Perspective(INISettings::cameraFOV, (float)INISettings::windowWidth / (float)INISettings::windowHeight, 0.1f, 1000.f);
	m_camera->GetTransform().SetPosition(INISettings::cameraPosition);
	m_camera->LookAt(INISettings::cameraPosition, INISettings::cameraTarget, INISettings::cameraUp);
}

void Game::InitCollisions()
{
	m_physics.Init(m_world);
}

void Game::BindUIInput()
{
	Inputs::BindInput(GLFW_KEY_1,			std::bind(&Inventory::		  SelectHotbar,	m_player.GetInventory(), 0), false);
	Inputs::BindInput(GLFW_KEY_2,			std::bind(&Inventory::		  SelectHotbar,	m_player.GetInventory(), 1), false);
	Inputs::BindInput(GLFW_KEY_3,			std::bind(&Inventory::		  SelectHotbar,	m_player.GetInventory(), 2), false);
	Inputs::BindInput(GLFW_KEY_4,			std::bind(&Inventory::		  SelectHotbar,	m_player.GetInventory(), 3), false);
	Inputs::BindInput(GLFW_KEY_5,			std::bind(&Inventory::		  SelectHotbar,	m_player.GetInventory(), 4), false);
	Inputs::BindInput(GLFW_KEY_6,			std::bind(&Inventory::		  SelectHotbar,	m_player.GetInventory(), 5), false);
	Inputs::BindInput(GLFW_KEY_7,			std::bind(&Inventory::		  SelectHotbar,	m_player.GetInventory(), 6), false);
	Inputs::BindInput(GLFW_KEY_8,			std::bind(&Inventory::		  SelectHotbar,	m_player.GetInventory(), 7), false);
	Inputs::BindInput(GLFW_KEY_9,			std::bind(&Inventory::		  SelectHotbar,	m_player.GetInventory(), 8), false);
}

void Game::BindPlayerInput()
{
	Inputs::BindInput(GLFW_KEY_W,				std::bind(&Camera::MoveForward, m_camera),		false);
	Inputs::BindInput(GLFW_KEY_S,				std::bind(&Camera::MoveBackward, m_camera),		false);
	Inputs::BindInput(GLFW_KEY_A,				std::bind(&Camera::StrafeLeft, m_camera),		false);
	Inputs::BindInput(GLFW_KEY_D,				std::bind(&Camera::StrafeRight, m_camera),		false);
	Inputs::BindInput(GLFW_KEY_E,				std::bind(&Camera::MoveUp, m_camera),			false);
	Inputs::BindInput(GLFW_KEY_Q,				std::bind(&Camera::MoveDown, m_camera),			false);
	Inputs::BindInput(GLFW_KEY_LEFT_SHIFT,		std::bind(&Camera::Sprint, m_camera),			false);
	Inputs::BindInput(GLFW_KEY_SPACE,			std::bind(&Camera::Jump, m_camera, m_world),	false);
	Inputs::BindInput(GLFW_KEY_I,				std::bind(&Game::ToggleInventory, this),		true);
	Inputs::BindInput(GLFW_MOUSE_BUTTON_LEFT,	std::bind(&Game::BreakBlock, this),				false);
	Inputs::BindInput(GLFW_MOUSE_BUTTON_RIGHT,	std::bind(&Game::PlaceBlock, this),	false, m_player.PLACE_TIME);
	Inputs::BindInput(GLFW_KEY_ESCAPE,			std::bind(&Menu::OpenMenu, m_menu));
}

void Game::BindWorldInput()
{
	Inputs::BindInput(GLFW_KEY_F1, std::bind(&PhysicsController::ToggleGravity, &m_physics));
	Inputs::BindInput(GLFW_KEY_F2, std::bind(&Game::ToggleWireFrame, this));
	Inputs::BindInput(GLFW_KEY_F3, std::bind(&PhysicsController::ToggleCollisions, &m_physics));
	Inputs::BindInput(GLFW_KEY_R, [this]() {m_world->ReloadShader(); });
}

void Game::Run()
{
	Init();
	float color[4];
	INISettings::backgroundColor.ToArray(color);
	INISettings::backgroundColor;
	m_menu->OpenMenu();
	//FreeConsole();
	while (glfwWindowShouldClose(mp_window) == 0)
	{
		Renderer::BackgroundColor(color);
		GameTime::Instance()->UpdateDeltaTime();
		Inputs();
		Update();
		Render();

#ifdef _DEBUG
		std::cout << GameTime::Instance()->GetDeltaTime() << std::endl;
#endif
	}
}

void Game::Update()
{
	//Pause the game if the focus is out of the window
	GameTime::Instance()->SetTimeScale((glfwGetWindowAttrib(mp_window, GLFW_FOCUSED) ? 1.f : 0.f));

	//Update the camera only if the inventory is not opened
	if(!m_player.GetInventory()->IsVisible())
		m_camera->Update();

	//Tests and replace entities for collisions
	ComputePhysics();

	//Replace the camera after the player collisions
	m_camera->CenterToTargetWithOffset();

	UpdateWorld();
	m_player.Update();
	UpdateUI();
}

void Game::Inputs()
{
	Inputs::PollEvents();
	Inputs::UpdateInputs();
}

void Game::Render()
{
	float ambient[3] = { 0.3f, 0.3f, 0.3f };
	m_camera->LookAt(m_camera->GetTransform().GetPosition(), m_camera->GetTransform().GetPosition() + m_camera->GetTransform().GetForward(), vecs::Vec3::Up());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	matrix::Matrix4x4 m = m_camera->GetPerspective() * m_camera->LookAt(m_camera->GetTransform().GetPosition(), m_camera->GetTransform().GetPosition() + m_camera->GetTransform().GetForward(), vecs::Vec3(0, 1, 0));;

	matrix::Matrix4x4 Test = matrix::CreatePositionMatrix(vecs::Vec3(25, 36, 9)) *
		rotation::CreateRotationQuaternion(vecs::Vec3(0, 0, 0)).AsMatrix() *
		matrix::CreateScaleMatrix(vecs::Vec3(1, 1, 1));

	ShaderParameter s;
	memcpy(s.mat, &m[0][0], sizeof(float) * 16);
	memcpy(s.ambient, ambient, sizeof(float) * 3);
	m_camera->GetPosition().ToArray(s.camPos);
	mathlib::Normalize(m_camera->GetTransform().GetForward()).ToArray(s.camDir);
	s.wireframe = m_isWireFrame;

	//Wireframe Checks
	if (m_isWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		s.color[0] = INISettings::wireframeColor.r / 255.f;
		s.color[1] = INISettings::wireframeColor.g / 255.f;
		s.color[2] = INISettings::wireframeColor.b / 255.f;
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if(!m_menu->IsVisible())
		m_world->RenderChunk(s);

	gui->Draw(m_world->GetShader("Shader2D"));

	glfwSwapBuffers(mp_window);
}

void Game::UpdateWorld()
{
	if (m_gen)
	{
		m_gen = false;
		auto func = [this]() {m_world->UpdateChunkLists(vecs::Vec2(m_player.GetTransform().GetPosition().x, m_player.GetTransform().GetPosition().z)); m_gen = true; };
		ThreadLib::PoolThread::getInstance().AddTask(new TaskThreadLib::TaskTemplated::TaskTemplate<>(func, false));
	}
}

void Game::UpdateUI()
{
}

void Game::ComputePhysics()
{
	m_camera->ApplyAccelerationToTarget();

	if (m_physics.GetIsGravityActive())
	{
		m_physics.ApplyGravity(m_player);
	}

	m_physics.ComputeCollisions(m_player);

	if (m_player.IsGrounded(m_world))
	{
		m_player.SetVerticalVelocity(vecs::Vec3::Zero());
	}
}

void Game::ToggleWireFrame()
{
	m_isWireFrame = !m_isWireFrame;
}

void Game::ToggleInventory()
{
	Inputs::ToggleMouseLock();
	Inputs::ToggleMouseVisibility();
	m_player.GetInventory()->ToggleInventory(gui);
	if (!m_player.GetInventory()->IsVisible())
	{
		Inputs::BindInput(GLFW_MOUSE_BUTTON_LEFT,	std::bind(&Game::BreakBlock, this), false);
		Inputs::BindInput(GLFW_MOUSE_BUTTON_RIGHT,	std::bind(&Game::PlaceBlock, this), false, m_player.PLACE_TIME);
	}
}

void Game::BreakBlock()
{
	Ray ray(m_camera->GetTransform().GetPosition(), m_camera->GetTransform().GetForward());
	uint16_t ID = m_world->BreakBlock(ray, m_camera->GetTarget()->GetBaseDamage());

	if(ID != block::AIR)
		m_player.GetInventory()->AddItem(new ItemStack(ID, 1));
}

void Game::PlaceBlock()
{
	ItemStack* s = m_player.GetInventory()->GetHotbar()[m_player.GetInventory()->GetSelectedHotbarSlotID()]->GetItemStack();
	Ray ray(m_camera->GetTransform().GetPosition(), m_camera->GetTransform().GetForward());

	if (s)
	{
		m_camera->GetTarget()->GetCapsule().SetCenter(m_camera->GetTarget()->GetTransform().GetPosition());
		if (m_world->PlaceBlock(ray, m_camera->GetTarget()->GetCapsule(), s->m_itemID))
		{
			m_player.GetInventory()->DropOneItemInSlot(m_player.GetInventory()->GetSelectedHotbarSlotID());
		}
	}
}

void Game::ExitGame()
{
	glfwSetWindowShouldClose(mp_window, GLFW_TRUE);
	m_world->GetSoundController().GetBackgroundMusic().stop();
}