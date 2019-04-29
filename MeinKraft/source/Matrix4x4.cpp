#include <xmemory>
#include <iostream>
#include <emmintrin.h>
#include <assert.h>
#include "Matrix4x4.h"
#include "Vec3.h"
#include "mathlib.h"


namespace matrix
{
	Matrix4x4::Matrix4x4(const float n)
	{
		memset(&matrix[0][0], 0, sizeof(float) * 16);
		matrix[0][0] = n;
		matrix[1][1] = n;
		matrix[2][2] = n;
		matrix[3][3] = n;
	}

	Matrix4x4::Matrix4x4(const float a[16])
	{
		memcpy(&matrix[0][0], &a[0], sizeof(float) * 16);
	}

	Matrix4x4::Matrix4x4(const float f1, const float f2, const float f3, const float f4, const float f5, const float f6, const float f7, const float f8, const float f9, const float f10, const float f11, const float f12, const float f13, const float f14, const float f15, const float f16)
	{
		matrix[0][0] = f1;
		matrix[1][0] = f2;
		matrix[2][0] = f3;
		matrix[3][0] = f4;
		matrix[0][1] = f5;
		matrix[1][1] = f6;
		matrix[2][1] = f7;
		matrix[3][1] = f8;
		matrix[0][2] = f9;
		matrix[1][2] = f10;
		matrix[2][2] = f11;
		matrix[3][2] = f12;
		matrix[0][3] = f13;
		matrix[1][3] = f14;
		matrix[2][3] = f15;
		matrix[3][3] = f16;
	}

	Matrix4x4 CreateRotationMatrix(const vecs::Vec3& v)
	{
		return CreateYRotationMatrix(v.y) * CreateXRotationMatrix(v.x) * CreateZRotationMatrix(v.z);
	}

	Matrix4x4 CreateRotationMatrix(const vecs::Vec4& v)
	{
		return CreateYRotationMatrix(v.y) * CreateXRotationMatrix(v.x) * CreateZRotationMatrix(v.z);
	}

	Matrix4x4 CreateXRotationMatrix(const float angleInDegree)
	{
		float AngleRad = angleInDegree * mathlib::toRadian;
		float AngleRadCos = mathlib::cos(AngleRad);
		float AngleRadSin = mathlib::sin(AngleRad);
		Matrix4x4 Mat;
		Mat[1][2] = -AngleRadSin;
		Mat[2][1] = AngleRadSin;
		Mat[1][1] = AngleRadCos;
		Mat[2][2] = AngleRadCos;
		return Mat;
	}

	Matrix4x4 CreateYRotationMatrix(const float angleInDegree)
	{
		float AngleRad = angleInDegree * mathlib::toRadian;
		float AngleRadCos = mathlib::cos(AngleRad);
		float AngleRadSin = mathlib::sin(AngleRad);
		Matrix4x4 Mat;
		Mat[0][2] = AngleRadSin;
		Mat[2][0] = -AngleRadSin;
		Mat[0][0] = AngleRadCos;
		Mat[2][2] = AngleRadCos;
		return Mat;
	}

	Matrix4x4 CreateZRotationMatrix(const float angleInDegree)
	{
		float AngleRad = angleInDegree * mathlib::toRadian;
		float AngleRadCos = mathlib::cos(AngleRad);
		float AngleRadSin = mathlib::sin(AngleRad);
		Matrix4x4 Mat;
		Mat[0][1] = -AngleRadSin;
		Mat[1][0] = AngleRadSin;
		Mat[0][0] = AngleRadCos;
		Mat[1][1] = AngleRadCos;
		return Mat;
	}

	std::ostream& operator<<(std::ostream& out, const Matrix4x4& Matrix4x4)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			out << i << " : (";
			for (unsigned int j = 0; j < 4; ++j)
			{
				out << " " << Matrix4x4.matrix[i][j];
			}
			out << ")" << std::endl;
		}
		return out;
	}

	Matrix4x4 Matrix4x4::Transpose()const
	{
		__m128 row0 = _mm_loadu_ps(&matrix[0][0]);
		__m128 row1 = _mm_loadu_ps(&matrix[1][0]);
		__m128 row2 = _mm_loadu_ps(&matrix[2][0]);
		__m128 row3 = _mm_loadu_ps(&matrix[3][0]);
		_MM_TRANSPOSE4_PS(row0, row1, row2, row3);
		Matrix4x4 mat;
		_mm_storeu_ps(&mat.matrix[0][0], row0);
		_mm_storeu_ps(&mat.matrix[1][0], row1);
		_mm_storeu_ps(&mat.matrix[2][0], row2);
		_mm_storeu_ps(&mat.matrix[3][0], row3);
		return mat;

	}

	Matrix4x4& Matrix4x4::operator=(const Matrix4x4& m)
	{
		memcpy(matrix, m.matrix, sizeof(float) * 16);
		return *this;
	}

	bool Matrix4x4::operator==(const Matrix4x4& m)const
	{
		bool* i1 = (bool*)(&_mm_cmpeq_ps(_mm_loadu_ps(&matrix[0][0]), _mm_loadu_ps(&m.matrix[0][0])));
		bool* i2 = (bool*)(&_mm_cmpeq_ps(_mm_loadu_ps(&matrix[1][0]), _mm_loadu_ps(&m.matrix[1][0])));
		bool* i3 = (bool*)(&_mm_cmpeq_ps(_mm_loadu_ps(&matrix[2][0]), _mm_loadu_ps(&m.matrix[2][0])));
		bool* i4 = (bool*)(&_mm_cmpeq_ps(_mm_loadu_ps(&matrix[3][0]), _mm_loadu_ps(&m.matrix[3][0])));
		__asm
		{
			emms;
		}
		return (i1[0] && i1[1] && i1[2] && i1[3] &&
				i2[0] && i2[1] && i2[2] && i2[3] &&
				i3[0] && i3[1] && i3[2] && i3[3] &&
				i4[0] && i4[1] && i4[2] && i4[3]);
	}
	bool Matrix4x4::operator!=(const Matrix4x4& m)const
	{
		bool* i1 = (bool*)(&_mm_cmpneq_ps(_mm_loadu_ps(&matrix[0][0]), _mm_loadu_ps(&m.matrix[0][0])));
		bool* i2 = (bool*)(&_mm_cmpneq_ps(_mm_loadu_ps(&matrix[1][0]), _mm_loadu_ps(&m.matrix[1][0])));
		bool* i3 = (bool*)(&_mm_cmpneq_ps(_mm_loadu_ps(&matrix[2][0]), _mm_loadu_ps(&m.matrix[2][0])));
		bool* i4 = (bool*)(&_mm_cmpneq_ps(_mm_loadu_ps(&matrix[3][0]), _mm_loadu_ps(&m.matrix[3][0])));
		__asm
		{
			emms;
		}
		return (i1[0] && i1[1] && i1[2] && i1[3] &&
				i2[0] && i2[1] && i2[2] && i2[3] && 
				i3[0] && i3[1] && i3[2] && i3[3] && 
				i4[0] && i4[1] && i4[2] && i4[3]);
	}

	Matrix4x4 Matrix4x4::operator+(const Matrix4x4& m)const
	{
		Matrix4x4 mat;
		_mm_storeu_ps(&mat.matrix[0][0], _mm_add_ps(_mm_loadu_ps(&matrix[0][0]), _mm_loadu_ps(&m.matrix[0][0])));
 		_mm_storeu_ps(&mat.matrix[1][0], _mm_add_ps(_mm_loadu_ps(&matrix[1][0]), _mm_loadu_ps(&m.matrix[1][0])));
		_mm_storeu_ps(&mat.matrix[2][0], _mm_add_ps(_mm_loadu_ps(&matrix[2][0]), _mm_loadu_ps(&m.matrix[2][0])));
		_mm_storeu_ps(&mat.matrix[3][0], _mm_add_ps(_mm_loadu_ps(&matrix[3][0]), _mm_loadu_ps(&m.matrix[3][0])));
		__asm
		{
			emms;
		}
		return mat;
	}
	Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& m)
	{
		_mm_storeu_ps(&matrix[0][0], _mm_add_ps(_mm_loadu_ps(&matrix[0][0]), _mm_loadu_ps(&m.matrix[0][0])));
		_mm_storeu_ps(&matrix[1][0], _mm_add_ps(_mm_loadu_ps(&matrix[1][0]), _mm_loadu_ps(&m.matrix[1][0])));
		_mm_storeu_ps(&matrix[2][0], _mm_add_ps(_mm_loadu_ps(&matrix[2][0]), _mm_loadu_ps(&m.matrix[2][0])));
		_mm_storeu_ps(&matrix[3][0], _mm_add_ps(_mm_loadu_ps(&matrix[3][0]), _mm_loadu_ps(&m.matrix[3][0])));
		__asm
		{
			emms;
		}
		return *this;
	}
	Matrix4x4 Matrix4x4::operator+(const float m)const
	{
		float tab[4] = { m, m, m, m };
		__m128 tab128 = _mm_loadu_ps(tab);
		Matrix4x4 mat;
		_mm_storeu_ps(&mat.matrix[0][0], _mm_add_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&mat.matrix[1][0], _mm_add_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&mat.matrix[2][0], _mm_add_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&mat.matrix[3][0], _mm_add_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return mat;
	}
	Matrix4x4& Matrix4x4::operator+=(const float m)
	{
		float tab[4] = { m, m, m, m };
		__m128 tab128 = _mm_loadu_ps(tab);
		_mm_storeu_ps(&matrix[0][0], _mm_add_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&matrix[1][0], _mm_add_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&matrix[2][0], _mm_add_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&matrix[3][0], _mm_add_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return *this;
	}
	Matrix4x4 Matrix4x4::operator+(const Vec4& v)const
	{
		float tab[4] = { v.x, v.y, v.z, v.w };
		__m128 tab128 = _mm_loadu_ps(tab);
		Matrix4x4 mat;
		_mm_storeu_ps(&mat.matrix[0][0], _mm_add_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&mat.matrix[1][0], _mm_add_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&mat.matrix[2][0], _mm_add_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&mat.matrix[3][0], _mm_add_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return mat;
	}
	Matrix4x4& Matrix4x4::operator+=(const Vec4& v)
	{
		float tab[4] = { v.x, v.y, v.z, v.w };
		__m128 tab128 = _mm_loadu_ps(tab);
		_mm_storeu_ps(&matrix[0][0], _mm_add_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&matrix[1][0], _mm_add_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&matrix[2][0], _mm_add_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&matrix[3][0], _mm_add_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return *this;
	}

	Matrix4x4 Matrix4x4::operator-(const Matrix4x4& m)const
	{
		Matrix4x4 mat;
		_mm_storeu_ps(&mat.matrix[0][0], _mm_sub_ps(_mm_loadu_ps(&matrix[0][0]), _mm_loadu_ps(&m.matrix[0][0])));
		_mm_storeu_ps(&mat.matrix[1][0], _mm_sub_ps(_mm_loadu_ps(&matrix[1][0]), _mm_loadu_ps(&m.matrix[1][0])));
		_mm_storeu_ps(&mat.matrix[2][0], _mm_sub_ps(_mm_loadu_ps(&matrix[2][0]), _mm_loadu_ps(&m.matrix[2][0])));
		_mm_storeu_ps(&mat.matrix[3][0], _mm_sub_ps(_mm_loadu_ps(&matrix[3][0]), _mm_loadu_ps(&m.matrix[3][0])));
		__asm
		{
			emms;
		}
		return mat;
	}
	Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& m)
	{
		_mm_storeu_ps(&matrix[0][0], _mm_sub_ps(_mm_loadu_ps(&matrix[0][0]), _mm_loadu_ps(&m.matrix[0][0])));
		_mm_storeu_ps(&matrix[1][0], _mm_sub_ps(_mm_loadu_ps(&matrix[1][0]), _mm_loadu_ps(&m.matrix[1][0])));
		_mm_storeu_ps(&matrix[2][0], _mm_sub_ps(_mm_loadu_ps(&matrix[2][0]), _mm_loadu_ps(&m.matrix[2][0])));
		_mm_storeu_ps(&matrix[3][0], _mm_sub_ps(_mm_loadu_ps(&matrix[3][0]), _mm_loadu_ps(&m.matrix[3][0])));
		__asm
		{
			emms;
		}
		return *this;
	}
	Matrix4x4 Matrix4x4::operator-(const float m)const
	{
		float tab[4] = { m, m, m, m };
		__m128 tab128 = _mm_loadu_ps(tab);
		Matrix4x4 mat;
		_mm_storeu_ps(&mat.matrix[0][0], _mm_sub_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&mat.matrix[1][0], _mm_sub_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&mat.matrix[2][0], _mm_sub_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&mat.matrix[3][0], _mm_sub_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return mat;
	}
	Matrix4x4& Matrix4x4::operator-=(const float m)
	{
		float tab[4] = { m, m, m, m };
		__m128 tab128 = _mm_loadu_ps(tab);
		_mm_storeu_ps(&matrix[0][0], _mm_sub_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&matrix[1][0], _mm_sub_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&matrix[2][0], _mm_sub_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&matrix[3][0], _mm_sub_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return *this;
	}
	Matrix4x4 Matrix4x4::operator-(const Vec4& v)const
	{
		float tab[4] = { v.x, v.y, v.z, v.w };
		__m128 tab128 = _mm_loadu_ps(tab);
		Matrix4x4 mat;
		_mm_storeu_ps(&mat.matrix[0][0], _mm_sub_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&mat.matrix[1][0], _mm_sub_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&mat.matrix[2][0], _mm_sub_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&mat.matrix[3][0], _mm_sub_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return mat;
	}
	Matrix4x4& Matrix4x4::operator-=(const Vec4& v)
	{
		float tab[4] = { v.x, v.y, v.z, v.w };
		__m128 tab128 = _mm_loadu_ps(tab);
		_mm_storeu_ps(&matrix[0][0], _mm_sub_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&matrix[1][0], _mm_sub_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&matrix[2][0], _mm_sub_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&matrix[3][0], _mm_sub_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return *this;
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& m)const
	{
		Matrix4x4 result;
		float temp1[4] = { matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3] };
		float temp2[4] = { matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3] };
		float temp3[4] = { matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3] };
		float temp4[4] = { matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3] };

		__m128 row1M128 = _mm_loadu_ps(temp1);
		__m128 row2M128 = _mm_loadu_ps(temp2);
		__m128 row3M128 = _mm_loadu_ps(temp3);
		__m128 row4M128 = _mm_loadu_ps(temp4);

		__m128 col1M128 = _mm_loadu_ps(&m.matrix[0][0]);
		__m128 col2M128 = _mm_loadu_ps(&m.matrix[1][0]);
		__m128 col3M128 = _mm_loadu_ps(&m.matrix[2][0]);
		__m128 col4M128 = _mm_loadu_ps(&m.matrix[3][0]);

		__m128 tempM128C1 = _mm_mul_ps(row1M128, _mm_shuffle_ps(col1M128, col1M128, _MM_SHUFFLE(0, 0, 0, 0)));
		__m128 tempM128C2 = _mm_mul_ps(row2M128, _mm_shuffle_ps(col1M128, col1M128, _MM_SHUFFLE(1, 1, 1, 1)));
		__m128 tempM128C3 = _mm_mul_ps(row3M128, _mm_shuffle_ps(col1M128, col1M128, _MM_SHUFFLE(2, 2, 2, 2)));
		__m128 tempM128C4 = _mm_mul_ps(row4M128, _mm_shuffle_ps(col1M128, col1M128, _MM_SHUFFLE(3, 3, 3, 3)));
		__m128 a0 = _mm_add_ps(tempM128C1, tempM128C2);
		__m128 a1 = _mm_add_ps(tempM128C3, tempM128C4);

		__m128 tot = _mm_add_ps(a0, a1);

		_mm_storeu_ps(temp1, tot);

		result[0][0] = temp1[0];
		result[0][1] = temp1[1];
		result[0][2] = temp1[2];
		result[0][3] = temp1[3];

		tempM128C1 = _mm_mul_ps(row1M128, _mm_shuffle_ps(col2M128, col2M128, _MM_SHUFFLE(0, 0, 0, 0)));
		tempM128C2 = _mm_mul_ps(row2M128, _mm_shuffle_ps(col2M128, col2M128, _MM_SHUFFLE(1, 1, 1, 1)));
		tempM128C3 = _mm_mul_ps(row3M128, _mm_shuffle_ps(col2M128, col2M128, _MM_SHUFFLE(2, 2, 2, 2)));
		tempM128C4 = _mm_mul_ps(row4M128, _mm_shuffle_ps(col2M128, col2M128, _MM_SHUFFLE(3, 3, 3, 3)));
		a0 = _mm_add_ps(tempM128C1, tempM128C2);
		a1 = _mm_add_ps(tempM128C3, tempM128C4);

		tot = _mm_add_ps(a0, a1);

		_mm_storeu_ps(temp1, tot);

		result[1][0] = temp1[0];
		result[1][1] = temp1[1];
		result[1][2] = temp1[2];
		result[1][3] = temp1[3];

		tempM128C1 = _mm_mul_ps(row1M128, _mm_shuffle_ps(col3M128, col3M128, _MM_SHUFFLE(0, 0, 0, 0)));
		tempM128C2 = _mm_mul_ps(row2M128, _mm_shuffle_ps(col3M128, col3M128, _MM_SHUFFLE(1, 1, 1, 1)));
		tempM128C3 = _mm_mul_ps(row3M128, _mm_shuffle_ps(col3M128, col3M128, _MM_SHUFFLE(2, 2, 2, 2)));
		tempM128C4 = _mm_mul_ps(row4M128, _mm_shuffle_ps(col3M128, col3M128, _MM_SHUFFLE(3, 3, 3, 3)));
		a0 = _mm_add_ps(tempM128C1, tempM128C2);
		a1 = _mm_add_ps(tempM128C3, tempM128C4);

		tot = _mm_add_ps(a0, a1);

		_mm_storeu_ps(temp1, tot);

		result[2][0] = temp1[0];
		result[2][1] = temp1[1];
		result[2][2] = temp1[2];
		result[2][3] = temp1[3];

		tempM128C1 = _mm_mul_ps(row1M128, _mm_shuffle_ps(col4M128, col4M128, _MM_SHUFFLE(0, 0, 0, 0)));
		tempM128C2 = _mm_mul_ps(row2M128, _mm_shuffle_ps(col4M128, col4M128, _MM_SHUFFLE(1, 1, 1, 1)));
		tempM128C3 = _mm_mul_ps(row3M128, _mm_shuffle_ps(col4M128, col4M128, _MM_SHUFFLE(2, 2, 2, 2)));
		tempM128C4 = _mm_mul_ps(row4M128, _mm_shuffle_ps(col4M128, col4M128, _MM_SHUFFLE(3, 3, 3, 3)));
		a0 = _mm_add_ps(tempM128C1, tempM128C2);
		a1 = _mm_add_ps(tempM128C3, tempM128C4);

		tot = _mm_add_ps(a0, a1);

		_mm_storeu_ps(temp1, tot);

		result[3][0] = temp1[0];
		result[3][1] = temp1[1];
		result[3][2] = temp1[2];
		result[3][3] = temp1[3];

		__asm
		{
			emms;
		}
		return result;
	}
	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& m)
	{
		float temp1[4] = { matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3] };
		float temp2[4] = { matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3] };
		float temp3[4] = { matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3] };
		float temp4[4] = { matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3] };

		__m128 row1M128 = _mm_loadu_ps(temp1);
		__m128 row2M128 = _mm_loadu_ps(temp2);
		__m128 row3M128 = _mm_loadu_ps(temp3);
		__m128 row4M128 = _mm_loadu_ps(temp4);

		__m128 col1M128 = _mm_loadu_ps(&m.matrix[0][0]);
		__m128 col2M128 = _mm_loadu_ps(&m.matrix[1][0]);
		__m128 col3M128 = _mm_loadu_ps(&m.matrix[2][0]);
		__m128 col4M128 = _mm_loadu_ps(&m.matrix[3][0]);

		__m128 tempM128C1 = _mm_mul_ps(row1M128, _mm_shuffle_ps(col1M128, col1M128, _MM_SHUFFLE(0, 0, 0, 0)));
		__m128 tempM128C2 = _mm_mul_ps(row2M128, _mm_shuffle_ps(col1M128, col1M128, _MM_SHUFFLE(1, 1, 1, 1)));
		__m128 tempM128C3 = _mm_mul_ps(row3M128, _mm_shuffle_ps(col1M128, col1M128, _MM_SHUFFLE(2, 2, 2, 2)));
		__m128 tempM128C4 = _mm_mul_ps(row4M128, _mm_shuffle_ps(col1M128, col1M128, _MM_SHUFFLE(3, 3, 3, 3)));
		__m128 a0 = _mm_add_ps(tempM128C1, tempM128C2);
		__m128 a1 = _mm_add_ps(tempM128C3, tempM128C4);

		__m128 tot = _mm_add_ps(a0, a1);

		_mm_storeu_ps(temp1, tot);

		*this[0][0] = temp1[0];
		*this[0][1] = temp1[1];
		*this[0][2] = temp1[2];
		*this[0][3] = temp1[3];

		tempM128C1 = _mm_mul_ps(row1M128, _mm_shuffle_ps(col2M128, col2M128, _MM_SHUFFLE(0, 0, 0, 0)));
		tempM128C2 = _mm_mul_ps(row2M128, _mm_shuffle_ps(col2M128, col2M128, _MM_SHUFFLE(1, 1, 1, 1)));
		tempM128C3 = _mm_mul_ps(row3M128, _mm_shuffle_ps(col2M128, col2M128, _MM_SHUFFLE(2, 2, 2, 2)));
		tempM128C4 = _mm_mul_ps(row4M128, _mm_shuffle_ps(col2M128, col2M128, _MM_SHUFFLE(3, 3, 3, 3)));
		a0 = _mm_add_ps(tempM128C1, tempM128C2);
		a1 = _mm_add_ps(tempM128C3, tempM128C4);

		tot = _mm_add_ps(a0, a1);

		_mm_storeu_ps(temp1, tot);

		*this[1][0] = temp1[0];
		*this[1][1] = temp1[1];
		*this[1][2] = temp1[2];
		*this[1][3] = temp1[3];

		tempM128C1 = _mm_mul_ps(row1M128, _mm_shuffle_ps(col3M128, col3M128, _MM_SHUFFLE(0, 0, 0, 0)));
		tempM128C2 = _mm_mul_ps(row2M128, _mm_shuffle_ps(col3M128, col3M128, _MM_SHUFFLE(1, 1, 1, 1)));
		tempM128C3 = _mm_mul_ps(row3M128, _mm_shuffle_ps(col3M128, col3M128, _MM_SHUFFLE(2, 2, 2, 2)));
		tempM128C4 = _mm_mul_ps(row4M128, _mm_shuffle_ps(col3M128, col3M128, _MM_SHUFFLE(3, 3, 3, 3)));
		a0 = _mm_add_ps(tempM128C1, tempM128C2);
		a1 = _mm_add_ps(tempM128C3, tempM128C4);

		tot = _mm_add_ps(a0, a1);

		_mm_storeu_ps(temp1, tot);

		*this[2][0] = temp1[0];
		*this[2][1] = temp1[1];
		*this[2][2] = temp1[2];
		*this[2][3] = temp1[3];

		tempM128C1 = _mm_mul_ps(row1M128, _mm_shuffle_ps(col4M128, col4M128, _MM_SHUFFLE(0, 0, 0, 0)));
		tempM128C2 = _mm_mul_ps(row2M128, _mm_shuffle_ps(col4M128, col4M128, _MM_SHUFFLE(1, 1, 1, 1)));
		tempM128C3 = _mm_mul_ps(row3M128, _mm_shuffle_ps(col4M128, col4M128, _MM_SHUFFLE(2, 2, 2, 2)));
		tempM128C4 = _mm_mul_ps(row4M128, _mm_shuffle_ps(col4M128, col4M128, _MM_SHUFFLE(3, 3, 3, 3)));
		a0 = _mm_add_ps(tempM128C1, tempM128C2);
		a1 = _mm_add_ps(tempM128C3, tempM128C4);

		tot = _mm_add_ps(a0, a1);

		_mm_storeu_ps(temp1, tot);

		*this[3][0] = temp1[0];
		*this[3][1] = temp1[1];
		*this[3][2] = temp1[2];
		*this[3][3] = temp1[3];
		__asm
		{
			emms;
		}
		return *this;
	}
	Matrix4x4 Matrix4x4::operator*(const float m)const
	{
		float tab[4] = { m, m, m, m };
		__m128 tab128 = _mm_loadu_ps(tab);
		Matrix4x4 mat;
		_mm_storeu_ps(&mat.matrix[0][0], _mm_mul_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&mat.matrix[1][0], _mm_mul_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&mat.matrix[2][0], _mm_mul_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&mat.matrix[3][0], _mm_mul_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return mat;
	}
	Matrix4x4& Matrix4x4::operator*=(const float m)
	{
		float tab[4] = { m, m, m, m };
		__m128 tab128 = _mm_loadu_ps(tab);
		_mm_storeu_ps(&matrix[0][0], _mm_mul_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&matrix[1][0], _mm_mul_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&matrix[2][0], _mm_mul_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&matrix[3][0], _mm_mul_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return *this;
	}
	Vec4 Matrix4x4::operator*(const Vec4& v)const
	{
		float tab[4] = { v.x, v.y, v.z, v.w };

		__m128 tab128 = _mm_loadu_ps(tab);
		
		__m128 v0 = _mm_shuffle_ps(tab128, tab128, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 v1 = _mm_shuffle_ps(tab128, tab128, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 v2 = _mm_shuffle_ps(tab128, tab128, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 v3 = _mm_shuffle_ps(tab128, tab128, _MM_SHUFFLE(3, 3, 3, 3));

		__m128 m0 = _mm_mul_ps(_mm_loadu_ps(&matrix[0][0]), v0);
		__m128 m1 = _mm_mul_ps(_mm_loadu_ps(&matrix[1][0]), v1);
		__m128 m2 = _mm_mul_ps(_mm_loadu_ps(&matrix[2][0]), v2);
		__m128 m3 = _mm_mul_ps(_mm_loadu_ps(&matrix[3][0]), v3);
		__m128 a0 = _mm_add_ps(m0, m1);
		__m128 a1 = _mm_add_ps(m2, m3);

		_mm_storeu_ps(&tab[0], _mm_add_ps(a0, a1));

		__asm
		{
			emms;
		}

		return Vec4(tab[0],
					tab[1],
					tab[2], 
					tab[3]);
	}

	Matrix4x4& Matrix4x4::operator*=(const Vec4& v)
	{
		float tab[4] = { v.x, v.y, v.z, v.w };
		__m128 tab128 = _mm_loadu_ps(tab);
		_mm_storeu_ps(&matrix[0][0], _mm_mul_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&matrix[1][0], _mm_mul_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&matrix[2][0], _mm_mul_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&matrix[3][0], _mm_mul_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return *this;
	}

	Matrix4x4 Matrix4x4::operator/(const Matrix4x4& m)const
	{
		//not implemented;
		assert(false);
		return Matrix4x4(0.f);
	}
	Matrix4x4 Matrix4x4::operator/=(const Matrix4x4 & m)
	{
		//not implemented;
		assert(false);
		return Matrix4x4(0.f);
	}
	Matrix4x4 Matrix4x4::operator/(const float m)const
	{
		float tab[4] = { m, m, m, m };
		__m128 tab128 = _mm_loadu_ps(tab);
		Matrix4x4 mat;
		_mm_storeu_ps(&mat.matrix[0][0], _mm_div_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&mat.matrix[1][0], _mm_div_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&mat.matrix[2][0], _mm_div_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&mat.matrix[3][0], _mm_div_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return mat;
	}
	Matrix4x4& Matrix4x4::operator/=(const float m)
	{
		float tab[4] = { m, m, m, m };
		__m128 tab128 = _mm_loadu_ps(tab);
		_mm_storeu_ps(&matrix[0][0], _mm_div_ps(_mm_loadu_ps(&matrix[0][0]), tab128));
		_mm_storeu_ps(&matrix[1][0], _mm_div_ps(_mm_loadu_ps(&matrix[1][0]), tab128));
		_mm_storeu_ps(&matrix[2][0], _mm_div_ps(_mm_loadu_ps(&matrix[2][0]), tab128));
		_mm_storeu_ps(&matrix[3][0], _mm_div_ps(_mm_loadu_ps(&matrix[3][0]), tab128));
		__asm
		{
			emms;
		}
		return *this;
	}
	Matrix4x4 Matrix4x4::operator/(const Vec4& m)const
	{
		//not implemented;
		assert(false);
		return Matrix4x4(0.f);
	}
	Matrix4x4 Matrix4x4::operator/=(const Vec4& m)
	{
		//not implemented;
		assert(false);
		return Matrix4x4(0.f);
	}

	float* Matrix4x4::operator[](const unsigned int n)
	{
		if (n > 3)throw std::out_of_range("iterator is superior than 3");
		return matrix[n];
	}

	Matrix4x4 CreatePositionMatrix(const Vec3& v)
	{
		return Matrix4x4({  1.f, 0.f, 0.f, v.x, 
							0.f, 1.f, 0.f, v.y, 
							0.f, 0.f, 1.f, v.z, 
							0.f, 0.f, 0.f, 1.f });
	}

	Matrix4x4 CreatePositionMatrix(const Vec4& v)
	{
		return Matrix4x4({	1.f, 0.f, 0.f, v.x,
							0.f, 1.f, 0.f, v.y,
							0.f, 0.f, 1.f, v.z,
							0.f, 0.f, 0.f, v.w });
	}

	Matrix4x4 CreateScaleMatrix(const Vec3& v)
	{
		return Matrix4x4({	v.x, 0.f, 0.f, 0.f, 
							0.f, v.y, 0.f, 0.f, 
							0.f, 0.f, v.z, 0.f, 
							0.f, 0.f, 0.f, 1.f });
	}

	Matrix4x4 CreateScaleMatrix(const Vec4& v)
	{
		return Matrix4x4({	v.x, 0.f, 0.f, 0.f,
							0.f, v.y, 0.f, 0.f,
							0.f, 0.f, v.z, 0.f,
							0.f, 0.f, 0.f, v.w });
	}
}