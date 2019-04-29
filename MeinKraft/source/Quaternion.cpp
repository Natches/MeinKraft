#include "Quaternion.h"
#include "Mathlib.h"

namespace rotation
{
	Quaternion::Quaternion(const vecs::Vec3& Axis, const float AngleInDegree)
	{
		float theta = (AngleInDegree * mathlib::toRadian) * 0.5f;
		w = cos(theta);
		m_vector3D = Axis * mathlib::sin(theta);
	}

	Quaternion::Quaternion(const vecs::Vec3& EulerAngle)
	{
		vecs::Vec3 theta = (EulerAngle * mathlib::toRadian) * 0.5f;
		vecs::Vec3 cos = mathlib::cos(theta);
		vecs::Vec3 sin = mathlib::sin(theta);
		
		w = cos.x * cos.y * cos.z + sin.x * sin.y * sin.z;
		m_vector3D = {	sin.x * cos.y * cos.z - cos.x * sin.y * sin.z,
						cos.x * sin.y * cos.z + sin.x * cos.y * sin.z,
						cos.x * cos.y * sin.z - sin.x * sin.y * cos.z };
	}
	Quaternion::Quaternion(const float x, const float y, const float z, const float w)
		: m_vector3D(x, y, z), w(w)
	{
	}
	Quaternion Quaternion::Invert()const 
	{
		return Quaternion(-m_vector3D.x, -m_vector3D.y, -m_vector3D.z, w);
	}

	const Quaternion Quaternion::operator*(const Quaternion& q)const
	{
		Quaternion result;
		result.w = (w * q.w) - mathlib::Dot(m_vector3D, q.m_vector3D);
		result.m_vector3D = m_vector3D * q.w  + q.m_vector3D * w + mathlib::Cross(m_vector3D, q.m_vector3D);
		return result;
	}

	const vecs::Vec3 Quaternion::operator*(const vecs::Vec3& V)const
	{
		vecs::Vec3 uv = (mathlib::Cross(m_vector3D, V));
		vecs::Vec3 uuv = (mathlib::Cross(m_vector3D, uv));
		return V + ((uv * w) + uuv) * 2;
	}
	const Quaternion& Quaternion::Normalize()
	{
		float length = 1.f / mathlib::Magnitude(vecs::Vec4(m_vector3D, w));
		m_vector3D.x *= length, m_vector3D.y *= length, m_vector3D.z *= length, w *= length;
		return *this;
	}
	const matrix::Matrix4x4 Quaternion::AsMatrix()
	{
		Normalize();
		vecs::Vec3 powVec = m_vector3D * m_vector3D;
		float vXY = m_vector3D.x * m_vector3D.y;
		float vZW = m_vector3D.z * w;
		float vXZ = m_vector3D.x * m_vector3D.z;
		float vYW = m_vector3D.y * w;
		float vYZ = m_vector3D.y * m_vector3D.z;
		float vXW = m_vector3D.x * w;
		return matrix::Matrix4x4(
			1 - 2 * (powVec.y + powVec.z), 2 * (vXY + vZW), 2 * (vXZ - vYW), 0,
			2 * (vXY - vZW), 1 - 2 * (powVec.x + powVec.z), 2 * (vYZ + vXW), 0,
			2 * (vXZ + vYW), 2 * (vYZ - vXW), 1 - 2 * (powVec.x + powVec.y), 0,
			0, 0, 0, 1).Transpose();
	}
	Quaternion CreateRotationQuaternion(const vecs::Vec3& v)
	{
		return Quaternion(v);
	}

	Quaternion CreateXRotationQuaternion(const float angleInDegree)
	{
		return Quaternion({ 1,0,0 }, angleInDegree);
	}

	Quaternion CreateYRotationQuaternion(const float angleInDegree)
	{
		return Quaternion({ 0,1,0 }, angleInDegree);
	}

	Quaternion CreateZRotationQuaternion(const float angleInDegree)
	{
		return Quaternion({ 0,0,1 }, angleInDegree);
	}

	std::ostream& operator<<(std::ostream& out, const Quaternion& q)
	{
		return out << "w :" << q.w << ", Axis :" << " ( " << q.m_vector3D << ")";
	}
}