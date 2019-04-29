#include <assert.h>
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "MathLib.h"

namespace vecs
{
#pragma region Constructors
	Vec3::Vec3(const Vec2& v)
	{
		x = v.x;
		y = v.y;
		z = 0;
	}

	Vec3::Vec3(const float a, const float b, const float c)
	{
		x = a;
		y = b;
		z = c;
	}

	Vec3::Vec3(const Vec4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vec3::Vec3(const Vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vec3::~Vec3()
	{

	}
#pragma endregion

#pragma region Operations
	std::ostream& operator<<(std::ostream& out, const Vec3& v)
	{
		return out << "(" << v.x << "," << v.y << "," << v.z << ")";
	}

	Vec3 Vec3::operator*(const float s) const
	{
		return Vec3(x * s, y * s, z * s);
	}

	Vec3& Vec3::operator*=(const float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vec3 Vec3::operator*(const Vec3& v) const
	{
		return Vec3(x * v.x, y * v.y, z * v.z);
	}

	Vec3& Vec3::operator*=(const Vec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vec3 Vec3::operator/(const float s) const
	{
		assert(s != 0);
		return Vec3(x / s, y / s, z / s);
	}

	Vec3& Vec3::operator/=(const float s)
	{
		assert(s != 0);
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	Vec3 Vec3::operator+(const Vec3& v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3& Vec3::operator+=(const Vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vec3 Vec3::operator-(const Vec3& v) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	Vec3& Vec3::operator-=(const Vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	bool Vec3::operator<(const Vec3& a) const
	{
		return true;
	}

	Vec3& Vec3::operator=(const Vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	bool Vec3::operator==(const Vec3& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}
	bool Vec3::operator!=(const Vec3 & v) const
	{
		return !(x == v.x && y == v.y && z == v.z);
	}
#pragma endregion

	void Vec3::ToArray(float array[3])const
	{
		array[0] = x;
		array[1] = y;
		array[2] = z;
	}
}