#pragma once
#include <iostream>
#include <unordered_map>

namespace vecs
{
	class Vec2;
	class Vec4;

	class Vec3 final
	{
	public: 
		Vec3(const float a = 0, const float b = 0, const float c = 0);
		Vec3(const Vec4& v);
		Vec3(const Vec3& v);
		Vec3(const Vec2& v);
		~Vec3();

		union
		{
			float x;
			float r;
		};

		union
		{
			float y;
			float g;
		};

		union
		{
			float z;
			float b;
		};
		Vec3& operator=		(const Vec3& v);
		bool operator==		(const Vec3& v) const;
		bool operator!=		(const Vec3& v) const;
		Vec3 operator*		(const Vec3& v) const;
		Vec3& operator*=	(const Vec3& v);
		Vec3 operator*		(const float s) const;
		Vec3& operator*=	(const float s);
		Vec3 operator/		(const float s) const;
		Vec3& operator/=	(const float s);
		Vec3 operator+		(const Vec3& a) const;
		Vec3& operator+=	(const Vec3& a);
		Vec3 operator-		(const Vec3& a) const;
		Vec3& operator-=	(const Vec3& a);
		bool operator<		(const Vec3& a) const;
		inline Vec3 operator-		()const { return *this * -1; }

		void ToArray(float array[3])const;

		inline static const Vec3 Zero()		{ return Vec3(); }
		inline static const Vec3 Unit()		{ return Vec3(1, 1, 1); }
		inline static const Vec3 Up()		{ return Vec3(0, 1, 0); }
		inline static const Vec3 Right()	{ return Vec3(1, 0, 0); }
		inline static const Vec3 Forward()	{ return Vec3(0, 0, 1); }
	};
	std::ostream& operator<<(std::ostream& out, const Vec3& v);
}