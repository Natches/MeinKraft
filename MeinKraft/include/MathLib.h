#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Quaternion.h"

namespace mathlib
{
#define M_PI 3.141592653589793238462643383279502884197169399375105f
#define EPSILON 0.000001f

	using namespace vecs;
	using namespace rotation;

	enum E_AXIS
	{
		X,
		Y,
		Z
	};
	double Abs (double d);
	float			Cross			(const Vec2& a, const Vec2& b);
	Vec3			Cross			(const Vec3& a, const Vec3& b);
	float			Dot				(const Vec2& a, const Vec2& b);
	float			Dot				(const Vec3& a, const Vec3& b);
	float			Dot				(const Vec4& a, const Vec4& b);
	float			Angle			(const Vec2& a, const bool InRadian = true);
	float			Angle			(const Vec2& a, const Vec2& b, const bool InRadian = true);
	float			Angle			(const Vec3& a, const Vec3& b, const bool InRadian = true);
	Vec2			Rotate			(const Vec2& a, const float AngleInDegree);
	Vec3			Rotate			(const Vec3& a, const E_AXIS axis, const float AngleInDegree);
	inline bool	IsCollinear		(const Vec2& a, const Vec2& b)	{ return Dot(a, b) == 1; }
	inline bool	IsCollinear		(const Vec3& a, const Vec3& b)	{ return Dot(a, b) == 1; }
	inline bool	IsCollinear		(const Vec4& a, const Vec4& b)  { return Dot(a, b) == 1; }
	inline bool	IsOpposed		(const Vec2& a, const Vec2& b)	{ return Dot(a, b) == -1; }
	inline bool	IsOpposed		(const Vec3& a, const Vec3& b)	{ return Dot(a, b) == -1; }
	inline bool	IsOpposed		(const Vec4& a, const Vec4& b)  { return Dot(a, b) == -1; }
	inline bool	IsOrthogonal	(const Vec2& a, const Vec2& b)	{ return Dot(a, b) == 0; }
	inline bool	IsOrthogonal	(const Vec3& a, const Vec3& b)	{ return Dot(a, b) == 0; }
	inline bool	IsOrthogonal	(const Vec4& a, const Vec4& b)	{ return Dot(a, b) == 0; }
	inline bool	IsSameDirection	(const Vec2& a, const Vec2& b)	{ return Dot(a, b) > 0; }
	inline bool	IsSameDirection	(const Vec3& a, const Vec3& b)	{ return Dot(a, b) > 0; }
	inline bool	IsSameDirection	(const Vec4& a, const Vec4& b)  { return Dot(a, b) > 0; }
	float			Distance		(const Vec2& a, const Vec2& b);
	float			Distance		(const Vec3& a, const Vec3& b);
	float			Distance		(const Vec4& a, const Vec4& b);
	Vec2			Direction		(const Vec2& a, const Vec2& b);
	Vec3			Direction		(const Vec3& src, const Vec3& dest);
	Vec4			Direction		(const Vec4& src, const Vec4& dest);
	Vec2			Normalize		(const Vec2& v);
	Vec3			Normalize		(const Vec3& v);
	Vec4			Normalize		(const Vec4& v);
	Quaternion	Normalize		(const Quaternion& q);
	Vec4			Homogenize		(const Vec4& v);
	float			Magnitude		(const Vec2& v);
	float			Magnitude		(const Vec3& v);
	float			Magnitude		(const Vec4& v);
	float 		sqrt			(const float n);
	float 		sin				(const float n);
	float 		cos				(const float n);
	vecs::Vec3	sin				(const vecs::Vec3& v);
	vecs::Vec3 	cos				(const vecs::Vec3& v);
	float 		tan				(const float n);
	float 		aTan			(const float n);
	float 		aSin			(const float n);
	float 		aCos			(const float n);
	Vec3				ComputeMaxVector(const Vec3& v1, const Vec3& v2);
	Vec4				ComputeMaxVector(const Vec4& v1, const Vec4& v2);
	Vec3				ComputeMinVector(const Vec3& v1, const Vec3& v2);
	Vec4				ComputeMinVector(const Vec4& v1, const Vec4& v2);
	int					To1D			(const vecs::Vec3& v, const int Xmax, const int Ymax);
	int To1DYFirst(const vecs::Vec3& v, const int Xmax, const int Ymax);
	double				To1D			(const vecs::Vec3& v, const double Xmax, const double Ymax);
	void				To3D			(int idx, const int Xmax, const int Ymax, vecs::Vec3& v);
	void				To3DYFirst      (int idx, const int Xmax, const int Ymax, vecs::Vec3& v);
	void				To3D			(double idx, const double Xmax, const double Ymax, vecs::Vec3& v);
	bool				IsInside		(vecs::Vec3 pos, vecs::Vec3 PointMin, vecs::Vec3 PointMax);

	constexpr float		toRadian = (M_PI / 180.f);
	constexpr float		toDegree = (180.f / M_PI);
}

#include "MathLib_impl.h"
