#pragma once

#include "Vec3.h"
#include "Matrix4x4.h"

namespace rotation
{
	class Quaternion final
	{
	public:
		Quaternion() = default;
		Quaternion(const vecs::Vec3& Axis, const float AngleInDegree);
		Quaternion(const vecs::Vec3& EulerAngle);
		Quaternion(const float x, const float y, const float z, const float w );
		~Quaternion() = default;

		Quaternion Invert() const;

		const Quaternion operator*(const Quaternion& q) const;
		const vecs::Vec3 operator*(const vecs::Vec3& v) const;
		const Quaternion& Normalize();
		const matrix::Matrix4x4 AsMatrix();

		vecs::Vec3 m_vector3D;
		float w;
	};
	Quaternion			CreateRotationQuaternion	(const vecs::Vec3& v);
	Quaternion			CreateXRotationQuaternion	(const float angleInDegree);
	Quaternion			CreateYRotationQuaternion	(const float angleInDegree);
	Quaternion			CreateZRotationQuaternion	(const float angleInDegree);
	std::ostream&		operator<<					(std::ostream& out, const Quaternion& v);
}