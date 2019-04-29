#pragma once
#include "Collider.h"

class GameObject;

class SphereCollider final : public Collider
{
#pragma region Non-Static Attributes
private:
	vecs::Vec3	m_center;
	float		m_radius;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	SphereCollider(const vecs::Vec3& center = vecs::Vec3(), const float radius = 1.f);
	~SphereCollider();

	virtual void Update() {}
	virtual bool Intersects(const Collider& collider, vecs::Vec3& closestPoint);
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
	inline void					SetCenter(const vecs::Vec3& center) { m_center = center; }
	inline void					SetRadius(const float radius)		{ m_radius = radius; }
	inline const vecs::Vec3&	GetCenter() const					{ return m_center; }
	inline const float&			GetRadius() const					{ return m_radius; }
#pragma endregion
};

