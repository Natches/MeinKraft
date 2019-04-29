    #pragma once
#include <iostream>

namespace vecs
{
	class Vec3;

	class Vec2 final
	{
	public:
		Vec2(const float a = 0, const float b = 0);
		Vec2(const Vec2& v);
		Vec2(const Vec3& v);
		~Vec2();

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

		Vec2& operator=		(const Vec2& v);
		bool operator==		(const Vec2& v) const;
		bool operator!=		(const Vec2& v) const;
		Vec2 operator+		(const Vec2& v) const;
		Vec2& operator+=	(const Vec2& v);
		Vec2 operator-		(const Vec2& v) const;
		Vec2& operator-=	(const Vec2& v);
		Vec2 operator*		(const float s) const;
		Vec2& operator*=	(const float s);
		Vec2 operator/		(const float s) const;
		Vec2& operator/=	(const float s);

		inline Vec2 operator-		() { return *this * -1; }
		void ToArray(float array[2]);

		inline static const Vec2 Zero() { return Vec2(); }
		inline static const Vec2 Unit() { return Vec2(1, 1); }
		inline static const Vec2 Up()	{ return Vec2(0, 1); }
		inline static const Vec2 Right(){ return Vec2(1, 0); }
	};
	std::ostream& operator<<(std::ostream& out, const vecs::Vec2& v);
}