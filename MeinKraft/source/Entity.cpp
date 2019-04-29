#include "Entity.h"
#include "mathlib.h"
#include "GameTime.h"
#include "Ray.h"
#include "World.h"

using namespace entity;

const unsigned short	entity::DEFAULT_MAX_LIFE			= 100;
const unsigned short	entity::DEFAULT_MIN_LIFE			= 0;
const float				entity::DEFAULT_MAX_VELOCITY		= 20.f;

Entity::Entity(const E_ENTITY_TYPE type)
	: GameObject(type)
{
	m_jumping		= false;
	m_speedModifier = 1;
}

Entity::~Entity()
{

}

void Entity::RemoveLife(const unsigned short damage)
{
	m_currentLife -= damage;
	m_currentLife = mathlib::Clamp((short)DEFAULT_MIN_LIFE, (short)DEFAULT_MAX_LIFE, m_currentLife);
}

void Entity::AddLife(const unsigned short heal)
{
	m_currentLife += heal;
	m_currentLife = mathlib::Clamp((short)DEFAULT_MIN_LIFE, (short)DEFAULT_MAX_LIFE, m_currentLife);
}

void Entity::ApplyAcceleration()
{
	m_transform.AddPosition(m_acceleration);
	m_acceleration = vecs::Vec3::Zero();
}

void Entity::NormalizeAccelerationWithSpeed()
{
	if (m_acceleration != vecs::Vec3::Zero())
	{
		m_acceleration = mathlib::Normalize(m_acceleration) * m_speed * m_speedModifier;
	}
}

void Entity::ApplyDeltaTimeToAcceleration()
{
	m_acceleration += m_verticalVelocity;
	m_acceleration *= GameTime::Instance()->GetDeltaTime();
}

bool Entity::IsGrounded(world::World* world)
{
	vecs::Vec3 newPos(m_transform.GetPosition() + m_acceleration * GameTime::Instance()->GetDeltaTime());

	newPos.y				= newPos.y - 1;
	block::Block* block		= world->GetBlock(newPos);

	return (block && block->GetID() != block::AIR);
}

void Entity::Jump(world::World* world)
{
	if (IsGrounded(world))
	{
		m_verticalVelocity += vecs::Vec3::Up() * m_jumpSpeed;
	}
}
