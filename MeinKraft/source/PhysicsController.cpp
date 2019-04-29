#include "PhysicsController.h"
#include "INISettings.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include "Ray.h"
#include "Entity.h"
#include "MathLib.h"
#include "GameObject.h"
#include "GameTime.h"

PhysicsController::PhysicsController()
{
	m_gravity			= INISettings::gravity;
	m_isGravityActive	= false;
	m_activeCollision	= true;
}

PhysicsController::~PhysicsController()
{
}

void PhysicsController::Init(world::World* world)
{
	m_world = world;
}

void PhysicsController::ApplyGravity(Entity& entity)
{
	if (!entity.IsGrounded(m_world))
	{
		if (mathlib::Magnitude(entity.GetVerticalVelocity() + m_gravity) > entity::DEFAULT_MAX_VELOCITY)
		{
			entity.SetVerticalVelocity(mathlib::Normalize(entity.GetVerticalVelocity() + m_gravity) * entity::DEFAULT_MAX_VELOCITY);
		}
		else
		{
			entity.AddVerticalVelocity(m_gravity);
		}
	}
}

void PhysicsController::ComputeCollisions(Entity& entity)
{
	entity.ApplyDeltaTimeToAcceleration();
	entity.ApplyAcceleration();

	if (m_activeCollision)
	{
		AABBCollider			aabb;
		SphereCollider			sphere(entity.GetTransform().GetPosition(), entity.GetCapsule().GetRadius());
		vecs::Vec3				capsulePosition;
		float					capsuleY = 0;
		std::vector<vecs::Vec3> blockPositions = m_world->GetBlocksAround(sphere.GetCenter());

		for (auto block : blockPositions)
		{
			aabb.SetCenter(block);

			capsuleY = aabb.GetCenter().y;
			capsuleY = mathlib::Clamp(entity.GetTransform().GetPosition().y, entity.GetTransform().GetPosition().y + entity.GetCapsule().GetHalfHeight() * 2.f, capsuleY);

			capsulePosition = vecs::Vec3(entity.GetTransform().GetPosition().x, capsuleY, entity.GetTransform().GetPosition().z);

			sphere.SetCenter(capsulePosition);

			vecs::Vec3 closestPoint = collisionutils::FindClosestPoint(sphere.GetCenter(), aabb);

			if (mathlib::Distance(closestPoint, sphere.GetCenter()) <= sphere.GetRadius())
			{
				vecs::Vec3 direction = sphere.GetCenter() - closestPoint;
				if (direction != vecs::Vec3::Zero())
				{
					direction = mathlib::Normalize(direction);
				}
				entity.GetTransform().SetPosition(closestPoint + (direction * sphere.GetRadius()) + vecs::Vec3(0.f, entity.GetTransform().GetPosition().y - capsuleY, 0.f));
			}
		}
		blockPositions.clear();
	}
}

void PhysicsController::ToggleGravity()
{
	m_isGravityActive = !m_isGravityActive;
}

void PhysicsController::ToggleCollisions()
{
	m_activeCollision = !m_activeCollision;
}