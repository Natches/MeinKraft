#pragma once
#include "Camera.h"
#include "Inputs.h"
#include "PhysicsController.h"
#include "Player.h"

#include "ShaderLib.h"
#include "BlockLibrary.h"
#include "World.h"

#include "GLFW\glfw3.h"
#include <thread>

#include "Menu.h"

struct GLFWwindow;
class GUI;
class Inventory;

class Game final
{
#pragma region Non-Static Attributes
private:
	GLFWwindow*				mp_window;
	Camera*					m_camera;
	Menu*					m_menu;
	world::World*			m_world;
	PhysicsController		m_physics;
	Player					m_player;
	std::vector<vecs::Vec3> m_cubeCollisionsPosition;

	bool					m_isWireFrame;
	bool					m_gen;

#pragma endregion
#pragma region Static Attributes
public:
	static Font*			font;
	static GUI*				gui;
#pragma endregion
#pragma region Non-Static Methods
public:
	Game(GLFWwindow* window);
	~Game();

	void Run();
	void ExitGame();

	void BindPlayerInput();
	void BreakBlock();
	void PlaceBlock();

private:
	void Init();
	void InitPlayer();
	void InitCamera();
	void InitInputs();
	void InitUI();
	void InitCollisions();

	void BindUIInput();
	void BindWorldInput();

	void Update();
	void Inputs();
	void Render();

	void ToggleWireFrame();

	void ComputePhysics();
	void UpdateWorld();
	void UpdateUI();

	void ToggleInventory();
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
public:
	inline Player& GetPlayer() { return m_player; }
#pragma endregion
};