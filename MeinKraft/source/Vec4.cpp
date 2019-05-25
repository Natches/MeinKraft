#include <xmemory>
#include <iostream>
#include <immintrin.h>
#include "Vec4.h"
#include "Vec3.h"
#include "MathLib.h"

namespace vecs
{
	Vec4::Vec4(const float x, const float y, const float z, float w) 
		: x(x), y(y), z(z), w(w)
	{}

	Vec4::Vec4(const Vec3& v3, const float w)
		: x(v3.x), y(v3.y), z(v3.z), w(w)
	{}

	Vec4& Vec4::operator=(const Vec4 & v)
	{
		x = v.x, y = v.y, z = v.z, w = v.w;
		return *this;
	}

	bool Vec4::operator==(const Vec4& v)const
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { v.x, v.y, v.z, v.w };
		__m128 i1 = _mm_xor_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2));
		__asm
		{
			emms;
		}
		return _mm_test_all_zeros(_mm_set1_epi32(0xFFFFFFFF), _mm_castps_si128(i1));
	}
	bool Vec4::operator!=(const Vec4& v)const
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { v.x, v.y, v.z, v.w };
		__m128 i1 = _mm_xor_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2));
		__asm
		{
			emms;
		}
		return !_mm_test_all_zeros(_mm_set1_epi32(0xFFFFFFFF), _mm_castps_si128(i1));
	}
	Vec4 Vec4::operator+(const Vec4& v)const
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { v.x, v.y, v.z, v.w };
		_mm_storeu_ps(tab, _mm_add_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2)));
		__asm
		{
			emms;
		}
		return Vec4(tab[0], tab[1], tab[2], tab[3]);
	}
	Vec4& Vec4::operator+=(const Vec4& v)
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { v.x, v.y, v.z, v.w };
		_mm_storeu_ps(tab, _mm_add_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2)));
		__asm
		{
			emms;
		}
		x = tab[0], y = tab[1], z = tab[2], w = tab[3];
		return *this;
	}
	Vec4  Vec4::operator-(const Vec4& v)const
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { v.x, v.y, v.z, v.w };
		_mm_storeu_ps(tab, _mm_sub_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2)));
		__asm
		{
			emms;
		}
		return Vec4(tab[0], tab[1], tab[2], tab[3]);
	}
	Vec4& Vec4::operator-=(const Vec4& v)
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { v.x, v.y, v.z, v.w };
		_mm_storeu_ps(tab, _mm_sub_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2)));
		__asm
		{
			emms;
		}
		x = tab[0], y = tab[1], z = tab[2], w = tab[3];
		return *this;
	}
	Vec4 Vec4::operator*(const Vec4 & v) const
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { v.x, v.y, v.z, v.w };
		_mm_storeu_ps(tab, _mm_mul_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2)));
		__asm
		{
			emms;
		}
		return Vec4(tab[0], tab[1], tab[2], tab[3]);
	}
	Vec4& Vec4::operator*=(const Vec4 & v)
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { v.x, v.y, v.z, v.w };
		_mm_storeu_ps(tab, _mm_mul_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2)));
		__asm
		{
			emms;
		}
		x = tab[0], y = tab[1], z = tab[2], w = tab[3];
		return *this;
	}
	Vec4 Vec4::operator*(const float s)const
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { s, s, s, s };
		_mm_storeu_ps(tab, _mm_mul_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2)));
		__asm
		{
			emms;
		}
		return Vec4(tab[0], tab[1], tab[2], tab[3]);
	}
	Vec4& Vec4::operator*=(const float s)
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { s, s, s, s };
		_mm_storeu_ps(tab, _mm_sub_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2)));
		__asm
		{
			emms;
		}
		x = tab[0], y = tab[1], z = tab[2], w = tab[3];
		return *this;
	}
	Vec4 Vec4::operator/(const float s)const
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { s, s, s, s };
		_mm_storeu_ps(tab, _mm_div_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2)));
		__asm
		{
			emms;
		}
		return Vec4(tab[0], tab[1], tab[2], tab[3]);
	}
	Vec4& Vec4::operator/=(const float s)
	{
		float tab[4] = { x, y, z, w }, tab2[4] = { s, s, s, s };
		_mm_storeu_ps(tab, _mm_div_ps(_mm_loadu_ps(tab), _mm_loadu_ps(tab2)));
		__asm
		{
			emms;
		}
		x = tab[0], y = tab[1], z = tab[2], w = tab[3];
		return *this;
	}
	void Vec4::Homogenize()
	{
		*this /= w;
	}
	void Vec4::ToArray(float array[4])
	{
		array[0] = x;
		array[1] = y;
		array[2] = z;
		array[3] = w;
	}
	std::ostream& operator<<(std::ostream& out, const Vec4& v)
	{
		return out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	}
}
