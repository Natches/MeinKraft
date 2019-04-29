#pragma once
#include "Vec3.h"

namespace collider
{
	enum E_COLLIDER_TYPE
	{
		RAY,
		SPHERE,
		TRIANGLE,
		PLAN,
		AABB,
		OBB,
		CAPSULE
	};
}

class Collider
{
#pragma region Non-Static Attributes
protected:
	const collider::E_COLLIDER_TYPE		m_type;
	bool								m_isTrigger;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	virtual void Update() {}

protected:
	Collider(const collider::E_COLLIDER_TYPE& type);
	~Collider();

	virtual bool Intersects(const Collider& collider, vecs::Vec3& closestPoint) = 0;
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
public:
	inline const collider::E_COLLIDER_TYPE& GetType() const { return m_type; }
#pragma endregion
};

