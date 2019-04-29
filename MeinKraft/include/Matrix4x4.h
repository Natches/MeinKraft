#pragma once

#include "Vec4.h"

namespace matrix
{
	using namespace vecs;
	class Matrix4x4 final
	{
	public:
		//n is the base scale, default is 1
		Matrix4x4(const float n = 1);
		Matrix4x4(const float f1, const float f2, const float f3, const float f4, 
			const float f5, const float f6, const float f7, const float f8, 
			const float f9, const float f10, const float f11, const float f12, 
			const float f13, const float f14, const float f15, const float f16);
		Matrix4x4(const float a[16]);
		~Matrix4x4() = default;
		
		Matrix4x4		Transpose()const;
		Matrix4x4&		operator=	(const Matrix4x4& m);
		bool			operator==	(const Matrix4x4& m)const;
		bool			operator!=	(const Matrix4x4& m)const;

		Matrix4x4		operator+	(const Matrix4x4& m)const;
		Matrix4x4&		operator+=	(const Matrix4x4& m);
		Matrix4x4		operator+	(const float s)const;
		Matrix4x4&		operator+=	(const float s);
		Matrix4x4		operator+	(const Vec4& v)const;
		Matrix4x4&		operator+=	(const Vec4& v);

		Matrix4x4		operator-	(const Matrix4x4& m)const;
		Matrix4x4&		operator-=	(const Matrix4x4& m);
		Matrix4x4		operator-	(const float s)const;
		Matrix4x4&		operator-=	(const float s);
		Matrix4x4		operator-	(const Vec4& v)const;
		Matrix4x4&		operator-=	(const Vec4& v);

		Matrix4x4		operator*	(const Matrix4x4& m)const;
		Matrix4x4&		operator*=	(const Matrix4x4& m);
		Matrix4x4		operator*	(const float s)const;
		Matrix4x4&		operator*=	(const float s);
		Vec4			operator*	(const Vec4& v)const;
		Matrix4x4&		operator*=	(const Vec4& v);

		Matrix4x4		operator/	(const Matrix4x4& m)const;
		Matrix4x4		operator/=	(const Matrix4x4& m);
		Matrix4x4		operator/	(const float s)const;
		Matrix4x4&		operator/=	(const float s);
		Matrix4x4		operator/	(const Vec4& v)const;
		Matrix4x4		operator/=	(const Vec4& v);

		float*	operator[]	(const unsigned int n);

		float matrix[4][4];
	};
	Matrix4x4 CreatePositionMatrix(const Vec3& v);
	Matrix4x4 CreatePositionMatrix(const Vec4& v);
	Matrix4x4 CreateRotationMatrix(const vecs::Vec3& v);
	Matrix4x4 CreateXRotationMatrix(const float angleInDegree);
	Matrix4x4 CreateYRotationMatrix(const float angleInDegree);
	Matrix4x4 CreateZRotationMatrix(const float angleInDegree);
	Matrix4x4 CreateScaleMatrix(const Vec3& v);
	Matrix4x4 CreateScaleMatrix(const Vec4& v);
	Matrix4x4 CreateGlobalMatrix(const Vec3& pos, const Vec3& rot, const Vec3& scale);

	std::ostream&	operator<<(std::ostream& out, const Matrix4x4& Matrix4x4);
}

