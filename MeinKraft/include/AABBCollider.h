#pragma once
#include "Vec3.h"
#include "Collider.h"

class GameObject;

class AABBCollider final : public Collider
{
#pragma region Non-Static Attributes
private:
	vecs::Vec3 m_center;
	vecs::Vec3 m_widths;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	AABBCollider(const vecs::Vec3& center = vecs::Vec3(), const vecs::Vec3& widths = vecs::Vec3(1.f, 1.f, 1.f));
	~AABBCollider();

	virtual void Update() {}
	virtual bool Intersects(const Collider& collider, vecs::Vec3& closestPoint);
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
public:
	inline void					SetWidths(const vecs::Vec3& widths) { m_widths = widths; }
	inline void					SetCenter(const vecs::Vec3& center)	{ m_center = center; }
	inline const vecs::Vec3		GetPointMax()	const				{ return m_center + GetHalfWidths(); }
	inline const vecs::Vec3		GetPointMin()	const				{ return m_center - GetHalfWidths(); }
	inline const vecs::Vec3&	GetCenter()		const				{ return m_center; }
	inline const vecs::Vec3		GetHalfWidths()	const				{ return m_widths / 2.f; }
#pragma endregion
};

