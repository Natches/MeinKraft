#include <assert.h>
#include "Vec2.h"
#include "Vec3.h"

namespace vecs
{
#pragma region Constructors
	Vec2::Vec2(const float a, const float b)
	{
		x = a;
		y = b;
	}

	Vec2::Vec2(const Vec2& v)
	{
		x = v.x;
		y = v.y;
	}

	Vec2::Vec2(const Vec3& v)
	{
		x = v.x;
		y = v.y;
	}

	Vec2::~Vec2()
	{

	}
#pragma endregion

#pragma region Operators
	std::ostream& operator<<(std::ostream& out, const vecs::Vec2& v)
	{ 
		return out << "(" << v.x << " , " << v.y << ")"; 
	}

	Vec2 Vec2::operator+(const Vec2& v) const
	{ 
		return Vec2(x + v.x, y + v.y); 
	}

	Vec2& Vec2::operator+=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vec2 Vec2::operator-(const Vec2& v) const
	{
		return Vec2(x - v.x, y - v.y);
	}

	Vec2& Vec2::operator-=(const Vec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vec2 Vec2::operator*(const float s) const
	{
		return Vec2(x * s, y * s);
	}

	Vec2& Vec2::operator*=(const float s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	Vec2 Vec2::operator/(const float s) const
	{
		assert(s != 0);
		return Vec2(x / s, y / s);
	}

	Vec2& Vec2::operator/=(const float s)
	{
		assert(s != 0);
		float m = 1.f / s;
		x *= m;
		y *= m;
		return *this;
	}

	Vec2& Vec2::operator=(const Vec2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	bool Vec2::operator==(const Vec2& v) const
	{
		return x == v.x && y == v.y;
	}

	bool Vec2::operator!=(const Vec2 & v) const
	{
		return x != v.x || y != v.y;
	}
#pragma endregion

	void Vec2::ToArray(float array[2])
	{
		array[0] = x;
		array[1] = y;
	}
}