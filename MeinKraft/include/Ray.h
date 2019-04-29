#pragma once
#include "AABBCollider.h"
#include "Vec3.h"

class Ray
{
public:
	Ray() = delete;
	Ray(const vecs::Vec3& origin, const vecs::Vec3& direction);
	~Ray() = default;

	bool Intersects(const AABBCollider& aabb, float& t) const;

public:
	vecs::Vec3 m_origin;
	vecs::Vec3 m_direction;
};