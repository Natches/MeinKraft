#include "MathLib.h"
#include "Matrix4x4.h"
#include "assert.h"

namespace mathlib
{
	const float Dot(const Vec2& a, const Vec2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	const float Dot(const Vec3& a, const Vec3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	const float Dot(const Vec4 & a, const Vec4 & b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	const float Angle(const Vec2& a, const bool InRadian)
	{
		float res = 0.f;
		if (a.y > EPSILON || a.y < -EPSILON)
		{
			if (a.y > 0.f)
				InRadian ? res = (float)(aCos(a.x / Magnitude(a))) : res = (float)aCos(a.x / Magnitude(a)) * 180.f / (float)M_PI;
			else if (a.y < 0.f)
				InRadian ? res = (float)-(aCos(a.x / Magnitude(a))) : res = (float)-(aCos(a.x / Magnitude(a)) * 180.f / (float)M_PI);
		}
		else
			return 0.f;
		return res;
	}

	const float Angle(const Vec2& a, const Vec2& b, const bool InRadian)
	{
		return InRadian ? (b.y < a.y ? -aCos((Dot(a, b) / Distance(a, b)))  : aCos((Dot(a, b) / Distance(a, b)))) : (b.y < a.y ? -aCos((Dot(a, b) / Distance(a, b))) * 180.f / (float)M_PI : aCos((Dot(a, b) / Distance(a, b))) * 180.f / (float)M_PI);
	}

	const float Angle(const Vec3& a, const Vec3& b, const bool InRadian)
	{
		return InRadian ? (b.y < a.y ? -aCos((Dot(a, b) / Distance(a, b))) : aCos((Dot(a, b) / Distance(a, b)))) : (b.y < a.y ? -aCos((Dot(a, b) / Distance(a, b))) * 180.f / (float)M_PI : aCos((Dot(a, b) / Distance(a, b))) * 180.f / (float)M_PI);
	}

	const Vec2 Rotate(const Vec2& a, const float angle)
	{
		return Vec2(a.x * cos(angle) - a.y * sin(angle), a.x * sin(angle) + a.y * cos(angle));
	}

	const Vec3 Rotate(const Vec3& a, const E_AXIS axis, const float AngleInDegree)
	{
		switch (axis)
		{
		case X:
			return Vec3(matrix::CreateXRotationMatrix(AngleInDegree) * Vec4(a, 1));
			break;
		case Y:
			return Vec3(matrix::CreateYRotationMatrix(AngleInDegree) * Vec4(a, 1));
			break;
		case Z:
			return Vec3(matrix::CreateZRotationMatrix(AngleInDegree) * Vec4(a, 1));
			break;
		default:
			return Vec3(0);
		}
	}

	const Vec3 Cross(const Vec3& a, const Vec3& b)
	{
		Vec3 r = Vec3();
		r.x = a.y * b.z - a.z * b.y;
		r.y = a.z * b.x - a.x * b.z;
		r.z = a.x * b.y - a.y * b.x;
		return r;
	}

	const Vec4 Cross(const Vec4 & a, const Vec4 & b)
	{
		//not implemented
		assert(false);
		return Vec4();
	}

	const double Abs(double d)
	{
		*((unsigned long long*)(&d)) &= 0x7fffffffffffffff;
		return d;
	}

	const float Cross(const Vec2& a, const Vec2& b)
	{
		Vec3 v1(a), v2(b);

		return Cross(v1, v2).z;
	}

	const float Distance(const Vec2& a, const Vec2& b)
	{
		return Magnitude(Direction(a, b));
	}

	const float Distance(const Vec3& a, const Vec3& b)
	{
		return Magnitude(Direction(a, b));
	}

	const float Distance(const Vec4& a, const Vec4& b)
	{
		return Magnitude(Direction(a, b));
	}

	const Vec2 Direction(const Vec2& src, const Vec2& dest)
	{
		return dest - src;
	}

	const Vec3 Direction(const Vec3& src, const Vec3& dest)
	{
		return dest - src;
	}

	const Vec4 Direction(const Vec4 & src, const Vec4 & dest)
	{
		return dest - src;
	}
	
	const Vec2 Normalize(const Vec2& v)
	{
		float m = mathlib::Magnitude(v);
		return (v / m);
	}

	const Vec3 Normalize(const Vec3& v)
	{
		float m = mathlib::Magnitude(v);
		return (v / m);
	}

	const Vec4 Normalize(const Vec4& v)
	{
		float m = mathlib::Magnitude(v);
		return (v / m);
	}

	const Quaternion Normalize(const Quaternion& q)
	{
		float length = 1.f / Magnitude(Vec4(q.m_vector3D, q.w)) ;
		return Quaternion(q.m_vector3D.x * length, q.m_vector3D.y * length, q.m_vector3D.z * length, q.w * length);
	}

	const Vec4 Homogenize(const Vec4& v)
	{
		return (v / v.w);
	}

	const float Magnitude(const Vec2& v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}

	const float Magnitude(const Vec3& v)
	{
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	const float Magnitude(const Vec4& v)
	{
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	}

	const float	sqrt(const float n)
	{
		_asm
		{
			fld  n;
			fsqrt;
			fstp n;
		}
		return n;
	}

	const float sin(const float n)
	{
		_asm
		{
			fld  n;
			fsin;
			fstp n;
		}
		return n;
	}

	const float cos(const float n)
	{
		_asm
		{
			fld  n;
			fcos;
			fstp n;
		}
		return n;
	}

	const vecs::Vec3 sin(const vecs::Vec3& v)
	{
		return vecs::Vec3(sin(v.x), sin(v.y), sin(v.z));
	}

	const vecs::Vec3 cos(const vecs::Vec3& v)
	{
		return vecs::Vec3(cos(v.x), cos(v.y), cos(v.z));
	}

	const float tan(const float n)
	{
		_asm
		{
			fld  n;
			fsincos;
			fdiv;
			fstp n;
		}
		return n;
	}

	const float aTan(const float n)
	{
		_asm
		{
			fld  n;
			fld1;
			fpatan;
			fstp n;
		}
		return n;
	}

	const float aSin(float n)
	{
		n = sqrt(n*n / (1 - n*n));
		n = aTan(n);
		return n;
	}

	const float aCos(float n)
	{
		n = sqrt((1 - n*n) / (n*n));
		n = aTan(n);
		return n;
	}

	Vec3 ComputeMaxVector(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(
			std::max(v1.x, v2.x),
			std::max(v1.y, v2.y),
			std::max(v1.z, v2.z)
		);
	}

	Vec4 ComputeMaxVector(const Vec4& v1, const Vec4& v2)
	{
		return Vec4(
			std::max(v1.x, v2.x),
			std::max(v1.y, v2.y),
			std::max(v1.z, v2.z),
			std::max(v1.w, v2.w)
		);
	}

	Vec3 ComputeMinVector(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(
			std::min(v1.x, v2.x),
			std::min(v1.y, v2.y),
			std::min(v1.z, v2.z)
		);
	}

	Vec4 ComputeMinVector(const Vec4& v1, const Vec4& v2)
	{
		return Vec4(
			std::min(v1.x, v2.x),
			std::min(v1.y, v2.y),
			std::min(v1.z, v2.z),
			std::min(v1.w, v2.w)
		);
	}

	int To1D(const vecs::Vec3& v, const int Xmax, const int Ymax)
	{
		return ((int)v.z * Xmax * Ymax) +((int)v.y * Xmax ) + (int)v.x;
	}

	int To1DYFirst(const vecs::Vec3& v, const int Xmax, const int Ymax)
	{
		return ((int)v.z * Xmax * Ymax) + ((int)v.x * Xmax) + (int)v.y;
	}

	double To1D(const vecs::Vec3& v, const double Xmax, const double Ymax)
	{
		return (double(v.z) * Xmax * Ymax) + (double(v.y) * Xmax) + double(v.x);
	}

	void To3D(int idx, const int Xmax, const int Ymax, vecs::Vec3& v)
	{
		v.z = float(idx / (Xmax * Ymax));
		idx -= ((int)v.z * Xmax * Ymax);
		v.y = float(idx / Xmax);
		v.x = float(idx % Xmax);
	}

	void To3DYFirst(int idx, const int Xmax, const int Ymax, vecs::Vec3 & v)
	{
		v.z = float(idx / (Xmax * Ymax));
		idx -= ((int)v.z * Xmax * Ymax);
		v.x = float(idx / Xmax);
		v.y = fmod(idx, Ymax);
	}

	void To3D(double idx, const double Xmax, const double Ymax, vecs::Vec3 & v)
	{
		v.z = float(idx / (Xmax * Ymax));
		idx -= ((int)v.z * Xmax * Ymax);
		v.y = float(idx / Xmax);
		v.x = fmod(idx, Xmax);
	}

	bool IsInside(vecs::Vec3 pos, vecs::Vec3 PointMin, vecs::Vec3 PointMax)
	{
		return (pos.x <= PointMax.x &&  pos.x >= PointMin.x) &&
			(pos.y <= PointMax.y &&  pos.y >= PointMin.y) &&
			(pos.z <= PointMax.z &&  pos.z >= PointMin.z);
	}
}