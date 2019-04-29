#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider
{
#pragma region Non-Static Attributes
private:
	vecs::Vec3	m_center;
	float		m_halfHeight;
	float		m_radius;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	CapsuleCollider(const vecs::Vec3& center = vecs::Vec3::Zero(), const float halfHeight = 0.5f, const float radius = 1.f);
	~CapsuleCollider() = default;

	virtual bool Intersects(const Collider& collider, vecs::Vec3& closestPoint);

	void operator=(const CapsuleCollider& c);
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
public:
	inline const vecs::Vec3&	GetCenter()		const	{ return m_center; }
	inline const float			GetHalfHeight() const	{ return m_halfHeight; }
	inline const float			GetRadius()		const	{ return m_radius; }

	inline void SetCenter(const vecs::Vec3& center)		{ m_center = center; }
	inline void SetHalfHeight(const float halfHeight)	{ m_halfHeight = halfHeight; }
	inline void SetRadius(const float radius)			{ m_radius = radius; }
#pragma endregion
};