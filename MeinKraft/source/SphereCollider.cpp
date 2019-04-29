#include "SphereCollider.h"
#include "GameObject.h"
#include "CollisionUtils.h"
using namespace collider;

SphereCollider::SphereCollider(const Vec3& center, const float radius)
	: Collider(collider::E_COLLIDER_TYPE::SPHERE)
{
	m_center = center;
	m_radius = radius;
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::Intersects(const Collider& collider, vecs::Vec3& closestPoint)
{
	bool hasIntersected = false;
	switch (collider.GetType())
	{
	case E_COLLIDER_TYPE::SPHERE:
		hasIntersected = collisionutils::Intersects(*this, *static_cast<const SphereCollider*>(&collider), closestPoint);
		break;
	case E_COLLIDER_TYPE::AABB:
		hasIntersected = collisionutils::Intersects(*this, *static_cast<const AABBCollider*>(&collider), closestPoint);
		break;
	}
	return hasIntersected;
}