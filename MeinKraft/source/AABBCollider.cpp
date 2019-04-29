#include "AABBCollider.h"
#include "CollisionUtils.h"
#include "GameObject.h"
using namespace collider;

AABBCollider::AABBCollider(const vecs::Vec3& center, const vecs::Vec3& widths)
	: Collider(E_COLLIDER_TYPE::AABB)
{
	m_center = center;
	m_widths = widths;
}

AABBCollider::~AABBCollider()
{
}

bool AABBCollider::Intersects(const Collider& collider, vecs::Vec3& closestPoint)
{
	bool hasIntersected = false;
	switch (collider.GetType())
	{
	case E_COLLIDER_TYPE::SPHERE:
		hasIntersected = collisionutils::Intersects(*dynamic_cast<const SphereCollider*>(&collider), *this, closestPoint);
		break;
	case E_COLLIDER_TYPE::AABB:
		hasIntersected = collisionutils::Intersects(*dynamic_cast<const AABBCollider*>(&collider), *this, closestPoint);
		break;
	}
	return hasIntersected;
}