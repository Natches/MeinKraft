#include "Ray.h"
#include "MathLib.h"

Ray::Ray(const vecs::Vec3& origin, const vecs::Vec3& direction)
	: m_origin(origin), m_direction(mathlib::Normalize(direction))
{

}

bool Ray::Intersects(const AABBCollider& aabb, float& t) const
{
	float invaRayDirX = 1.f / m_direction.x;
	float invaRayDirY = 1.f / m_direction.y;
	float invaRayDirZ = 1.f / m_direction.z;

	float Length1 = (aabb.GetPointMin().x - m_origin.x) * invaRayDirX;
	float Length2 = (aabb.GetPointMax().x - m_origin.x) * invaRayDirX;
	float Length3 = (aabb.GetPointMin().y - m_origin.y) * invaRayDirY;
	float Length4 = (aabb.GetPointMax().y - m_origin.y) * invaRayDirY;
	float Length5 = (aabb.GetPointMin().z - m_origin.z) * invaRayDirZ;
	float Length6 = (aabb.GetPointMax().z - m_origin.z) * invaRayDirZ;

	float LengthMin = std::max(std::max(std::min(Length1, Length2), std::min(Length3, Length4)), std::min(Length5, Length6));
	float LengthMax = std::min(std::min(std::max(Length1, Length2), std::max(Length3, Length4)), std::max(Length5, Length6));

	if (LengthMax <= 0.f)
	{
		return false;
	}
	else if (LengthMin > LengthMax)
	{
		return false;
	}

	t = LengthMin;
	return true;
}