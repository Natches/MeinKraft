#include "CollisionUtils.h"
#include "MathLib.h"

namespace collisionutils
{
	Ray::Ray(const vecs::Vec3& origin, const vecs::Vec3& dir)
	{
		m_origin	= origin;
		m_direction = dir;   
	}

	bool Intersects(const SphereCollider& c1, const SphereCollider& c2, vecs::Vec3& closestPoint)
	{
		vecs::Vec3 d = c2.GetCenter() - c1.GetCenter();
		float vDistance = mathlib::Magnitude(d), rDistance = c1.GetRadius() + c2.GetRadius();
		if (vDistance <= rDistance)
		{
			float t = c1.GetRadius() - (rDistance - vDistance);
			closestPoint = c1.GetCenter() + mathlib::Normalize(d) * t;
			return true;
		}
		return false;
	}

	bool Intersects(const SphereCollider& c1, const AABBCollider& c2, vecs::Vec3& closestPoint)
	{
		closestPoint = FindClosestPoint(c1.GetCenter(), c2);

		return mathlib::Distance(closestPoint, c1.GetCenter()) <= c1.GetRadius();
	}

	bool Intersects(const AABBCollider& c1, const AABBCollider& c2, vecs::Vec3& closestPoint)
	{
		bool x = abs(c1.GetCenter().x - c2.GetCenter().x) <= (c1.GetHalfWidths().x + c2.GetHalfWidths().x);
		bool y = abs(c1.GetCenter().y - c2.GetCenter().y) <= (c1.GetHalfWidths().y + c2.GetHalfWidths().y);
		bool z = abs(c1.GetCenter().z - c2.GetCenter().z) <= (c1.GetHalfWidths().z + c2.GetHalfWidths().z);

		return x && y && z;
	}

	vecs::Vec3 FindClosestPoint(const vecs::Vec3& sphere, const AABBCollider& aabb)
	{
		vecs::Vec3 result;
		result.x = mathlib::Clamp(aabb.GetPointMin().x, aabb.GetPointMax().x, sphere.x);
		result.y = mathlib::Clamp(aabb.GetPointMin().y, aabb.GetPointMax().y, sphere.y);
		result.z = mathlib::Clamp(aabb.GetPointMin().z, aabb.GetPointMax().z, sphere.z);
		return result;
	}
}