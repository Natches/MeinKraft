#pragma once
#include <vector>

#include "Vec3.h"
#include "World.h"
#include "CollisionUtils.h"

#define EPSILON 0.000001f

class Entity;
class AABBCollider;
class SphereCollider;

class PhysicsController
{
#pragma region Non-Static Attributes
private:
	vecs::Vec3		m_gravity;
	world::World*	m_world;
	bool			m_activeCollision;
	bool			m_isGravityActive;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	PhysicsController();
	~PhysicsController();

	void Init(world::World* world);

	void ApplyGravity(Entity& entity);
	void ComputeCollisions(Entity& entity);
	void ToggleGravity();
	void ToggleCollisions();
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
public:
	inline bool GetIsGravityActive() { return m_isGravityActive; }
#pragma endregion
};

