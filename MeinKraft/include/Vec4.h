#pragma once

namespace vecs
{
	class Vec3;

	class Vec4 final 
	{
	public:
		Vec4(const float x = 0, const float y = 0, const float z = 0, float w = 1);
		Vec4(const Vec3& v3, const float w);
		~Vec4() = default;

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

		union
		{
			float w;
			float a;
		};

		Vec4& operator=		(const Vec4& v);
		bool  operator==	(const Vec4& v) const;
		bool  operator!=	(const Vec4& v) const;
		Vec4  operator+		(const Vec4& v) const;
		Vec4& operator+=	(const Vec4& v);
		Vec4  operator-		(const Vec4& v) const;
		Vec4& operator-=	(const Vec4& v);
		Vec4  operator*		(const Vec4& v) const;
		Vec4& operator*=	(const Vec4& v);
		Vec4  operator*		(const float s) const;
		Vec4& operator*=	(const float s);
		Vec4  operator/		(const float s) const;
		Vec4& operator/=	(const float s);

		inline Vec4 operator-() const { return *this * -1; }

		void Homogenize();
		void ToArray(float array[4]);

		inline static const Vec4 Zero()		{ return Vec4(); }
		inline static const Vec4 Unit()		{ return Vec4(1, 1, 1); }
		inline static const Vec4 Up()		{ return Vec4(0, 1, 0); }
		inline static const Vec4 Right()	{ return Vec4(1, 0, 0); }
		inline static const Vec4 Forward()	{ return Vec4(0, 0, 1); }
	};

	std::ostream& operator<<(std::ostream& out, const Vec4& v);
}
//using Vec4 = vecs::Vec4;

