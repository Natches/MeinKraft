#pragma once
#include "SphereCollider.h"
#include "AABBCollider.h"

namespace collisionutils
{
	struct Ray
	{
		vecs::Vec3 m_origin;
		vecs::Vec3 m_direction;

		Ray(const vecs::Vec3& origin, const vecs::Vec3& dir);
	};

	bool Intersects(const SphereCollider& c1, const SphereCollider& c2, vecs::Vec3& closestPoint);
	bool Intersects(const SphereCollider& c1, const AABBCollider& c2, vecs::Vec3& closestPoint);
	bool Intersects(const AABBCollider& c1, const AABBCollider& c2, vecs::Vec3& closestPoint);

	vecs::Vec3 FindClosestPoint(const vecs::Vec3& sphere, const AABBCollider& aabb);
};

