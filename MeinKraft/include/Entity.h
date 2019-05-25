#pragma once
#include "GameObject.h"
#include "CapsuleCollider.h"
#include "World.h"

namespace entity
{
	enum E_ENTITY_TYPE : __int16
	{
		CAMERA,
		PLAYER,
		ZOMBIE
	};

	extern const unsigned short DEFAULT_MAX_LIFE;
	extern const unsigned short DEFAULT_MIN_LIFE;
	extern const float			DEFAULT_MAX_VELOCITY;
}

class Entity : public GameObject
{
#pragma region Non-Static Attributes
protected:
	CapsuleCollider m_capsule;
	vecs::Vec3		m_acceleration;
	vecs::Vec3		m_verticalVelocity;
	short			m_currentLife;
	short			m_maxLife;
	short			m_lifeRegeneration;
	short			m_armor;
	short			m_baseDamage;
	float			m_speed;
	float			m_speedModifier;
	float			m_jumpSpeed;
	bool			m_jumping;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	virtual void RemoveLife(const unsigned short damage);
	virtual void AddLife(const unsigned short heal);
	virtual void ApplyAcceleration();
	virtual void NormalizeAccelerationWithSpeed();
	virtual void ApplyDeltaTimeToAcceleration();
	virtual bool IsGrounded(world::World* world);
	virtual void Jump(world::World* world);

protected:
	Entity() = delete;
	Entity(const entity::E_ENTITY_TYPE type);
	~Entity();
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
public:
	inline CapsuleCollider&			GetCapsule()									{ return m_capsule; }
	inline vecs::Vec3			GetVerticalVelocity() const noexcept			{ return m_verticalVelocity; }
	inline vecs::Vec3			GetAcceleration() const noexcept				{ return m_acceleration; }
	inline short				GetCurrentLife() const noexcept					{ return m_currentLife; }
	inline short				GetMaxLife() const noexcept						{ return m_maxLife; }
	inline short				GetLifeRegeneration() const noexcept			{ return m_lifeRegeneration; }
	inline short				GetArmor() const noexcept						{ return m_armor; }
	inline short				GetBaseDamage() const noexcept					{ return m_baseDamage; }
	inline float				GetSpeed() const noexcept						{ return m_speed; }
	inline void						SetCapsule(CapsuleCollider& capsule)			{ m_capsule = capsule; }
	inline void						AddAcceleration(const vecs::Vec3& acceleration) { m_acceleration += acceleration; }
	inline void						SetAcceleration(const vecs::Vec3& acceleration) { m_acceleration = acceleration; }
	inline void						SetVerticalVelocity(const vecs::Vec3& velocity) { m_verticalVelocity = velocity; }
	inline void						AddVerticalVelocity(const vecs::Vec3& velocity) { m_verticalVelocity += velocity; }
#pragma endregion
};