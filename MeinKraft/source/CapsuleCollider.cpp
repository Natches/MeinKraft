#include "CapsuleCollider.h"
#include "CollisionUtils.h"
#include "MathLib.h"

CapsuleCollider::CapsuleCollider(const vecs::Vec3& center, const float halfHeight, const float radius)
	: Collider(collider::E_COLLIDER_TYPE::CAPSULE), m_center(center), m_halfHeight(halfHeight), m_radius(radius)
{

}

bool CapsuleCollider::Intersects(const Collider & collider, vecs::Vec3 & closestPoint)
{
	SphereCollider	sphere;
	AABBCollider	aabb;
	float capsuleY = 0;
	aabb.SetCenter(static_cast<const AABBCollider*>(&collider)->GetCenter());

	capsuleY = aabb.GetCenter().y;
	capsuleY = mathlib::Clamp(GetCenter().y, GetCenter().y + GetHalfHeight() * 2.f, capsuleY);

	sphere.SetRadius(GetRadius());
	sphere.SetCenter(vecs::Vec3(GetCenter().x, capsuleY, GetCenter().z));

	return collisionutils::Intersects(sphere, aabb, closestPoint);
}

void CapsuleCollider::operator=(const CapsuleCollider& c)
{
	m_center		= c.GetCenter();
	m_halfHeight	= c.GetHalfHeight();
	m_radius		= c.GetRadius();
}
