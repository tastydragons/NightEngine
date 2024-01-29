#include "hzpch.h"

#include "Platform/Windows/Window.h"

#include "Matrix4x4.h"

namespace Night
{
	Mat4x4::Mat4x4() :
		m00{ 0 }, m01{ 0 }, m02{ 0 }, m03{ 0 },
		m10{ 0 }, m11{ 0 }, m12{ 0 }, m13{ 0 },
		m20{ 0 }, m21{ 0 }, m22{ 0 }, m23{ 0 },
		m30{ 0 }, m31{ 0 }, m32{ 0 }, m33{ 0 }
	{}

	Mat4x4::Mat4x4(float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33) :
		m00{ _00 }, m01{ _01 }, m02{ _02 }, m03{ _03 },
		m10{ _10 }, m11{ _11 }, m12{ _12 }, m13{ _13 },
		m20{ _20 }, m21{ _21 }, m22{ _22 }, m23{ _23 },
		m30{ _30 }, m31{ _31 }, m32{ _32 }, m33{ _33 }
	{}

	Mat4x4::Mat4x4(float value) :
		m00{ value }, m01{ 0 }, m02{ 0 }, m03{ 0 },
		m10{ 0 }, m11{ value }, m12{ 0 }, m13{ 0 },
		m20{ 0 }, m21{ 0 }, m22{ value }, m23{ 0 },
		m30{ 0 }, m31{ 0 }, m32{ 0 }, m33{ value }
	{

	}

	Mat4x4::Mat4x4(Mat3D mat) :
		m00{ mat.m00 }, m01{ mat.m01 }, m02{ mat.m02 }, m03{ 0 },
		m10{ mat.m10 }, m11{ mat.m11 }, m12{ mat.m12 }, m13{ 0 },
		m20{ mat.m20 }, m21{ mat.m21 }, m22{ mat.m22 }, m23{ 0 },
		m30{ 0 }, m31{ 0 }, m32{ 0 }, m33{ 1 }
	{

	}
	Mat4x4::Mat4x4(glm::mat4& rhs) :
		m00{ rhs[0][0] }, m01{ rhs[0][1] }, m02{ rhs[0][2] }, m03{ rhs[0][3] },
		m10{ rhs[1][0] }, m11{ rhs[1][1] }, m12{ rhs[1][2] }, m13{ rhs[1][3] },
		m20{ rhs[2][0] }, m21{ rhs[2][1] }, m22{ rhs[2][2] }, m23{ rhs[2][3] },
		m30{ rhs[3][0] }, m31{ rhs[3][1] }, m32{ rhs[3][2] }, m33{ rhs[3][3] }
	{}




	Mat4x4& Mat4x4::operator=(const Mat4x4& rhs)
	{
		m00 = rhs.m00;
		m01 = rhs.m01;
		m02 = rhs.m02;
		m03 = rhs.m03;
		m10 = rhs.m10;
		m11 = rhs.m11;
		m12 = rhs.m12;
		m13 = rhs.m13;
		m20 = rhs.m20;
		m21 = rhs.m21;
		m22 = rhs.m22;
		m23 = rhs.m23;
		m30 = rhs.m30;
		m31 = rhs.m31;
		m32 = rhs.m32;
		m33 = rhs.m33;
		return *this;
	}

	Mat4x4& Mat4x4::operator*=(const Mat4x4& rhs) ///multiply the matrix
	{
		Mat4x4 temp;
		temp.m00 = (m00 * rhs.m00) + (m01 * rhs.m10) + (m02 * rhs.m20) + (m03 * m30);
		temp.m01 = (m00 * rhs.m01) + (m01 * rhs.m11) + (m02 * rhs.m21) + (m03 * m31);
		temp.m02 = (m00 * rhs.m02) + (m01 * rhs.m12) + (m02 * rhs.m22) + (m03 * m32);
		temp.m03 = (m00 * rhs.m03) + (m01 * rhs.m13) + (m02 * rhs.m23) + (m03 * m33);

		temp.m10 = (m10 * rhs.m00) + (m11 * rhs.m10) + (m12 * rhs.m20) + (m13 * m30);
		temp.m11 = (m10 * rhs.m01) + (m11 * rhs.m11) + (m12 * rhs.m21) + (m13 * m31);
		temp.m12 = (m10 * rhs.m02) + (m11 * rhs.m12) + (m12 * rhs.m22) + (m13 * m32);
		temp.m13 = (m10 * rhs.m03) + (m11 * rhs.m13) + (m12 * rhs.m23) + (m13 * m33);

		temp.m20 = (m20 * rhs.m00) + (m21 * rhs.m10) + (m22 * rhs.m20) + (m23 * m30);
		temp.m21 = (m20 * rhs.m01) + (m21 * rhs.m11) + (m22 * rhs.m21) + (m23 * m30);
		temp.m22 = (m20 * rhs.m02) + (m21 * rhs.m12) + (m22 * rhs.m22) + (m23 * m30);
		temp.m23 = (m20 * rhs.m03) + (m21 * rhs.m13) + (m22 * rhs.m23) + (m23 * m33);

		return *this = temp;
	}

	void Identity4x4(Mat4x4& Result)
	{
		Result.m00 = 1.0f;
		Result.m01 = 0.0f;
		Result.m02 = 0.0f;
		Result.m03 = 0.0f;

		Result.m10 = 0.0f;
		Result.m11 = 1.0f;
		Result.m12 = 0.0f;
		Result.m13 = 0.0f;

		Result.m20 = 0.0f;
		Result.m21 = 0.0f;
		Result.m22 = 1.0f;
		Result.m23 = 0.0f;

		Result.m30 = 0.0f;
		Result.m31 = 0.0f;
		Result.m32 = 0.0f;
		Result.m33 = 1.0f;
	}

	Night::Matrix4x4 Night::Translate4x4(Mat4x4& Result, float x, float y, float z) /// turn current matrix into a translation matrix
	{
		Result.m00 = 1.0f;
		Result.m01 = 0.0f;
		Result.m02 = 0.0f;
		Result.m03 = x;

		Result.m10 = 0.0f;
		Result.m11 = 1.0f;
		Result.m12 = 0.0f;
		Result.m13 = y;

		Result.m20 = 0.0f;
		Result.m21 = 0.0f;
		Result.m22 = 1.0f;
		Result.m23 = z;

		Result.m30 = 0.0f;
		Result.m31 = 0.0f;
		Result.m32 = 0.0f;
		Result.m33 = 1.0f;

		return Result;
	}

	Night::Matrix4x4 Night::Scale4x4(Mat4x4& Result, float x, float y, float z) /// turn current matrix into a scaling matrix
	{
		Result.m00 = x;
		Result.m01 = 0.0f;
		Result.m02 = 0.0f;
		Result.m03 = 0.0f;

		Result.m10 = 0.0f;
		Result.m11 = y;
		Result.m12 = 0.0f;
		Result.m13 = 0.0f;

		Result.m20 = 0.0f;
		Result.m21 = 0.0f;
		Result.m22 = z;
		Result.m23 = 0.0f;

		Result.m30 = 0.0f;
		Result.m31 = 0.0f;
		Result.m32 = 0.0f;
		Result.m33 = 1.0f;

		return Result;
	}

	void Scale4x4(Mat4x4& Result, Vec3 temp)
	{
		Result.m00 = temp.x;
		Result.m01 = 0.0f;
		Result.m02 = 0.0f;
		Result.m03 = 0.0f;

		Result.m10 = 0.0f;
		Result.m11 = temp.y;
		Result.m12 = 0.0f;
		Result.m13 = 0.0f;

		Result.m20 = 0.0f;
		Result.m21 = 0.0f;
		Result.m22 = temp.z;
		Result.m23 = 0.0f;

		Result.m30 = 0.0f;
		Result.m31 = 0.0f;
		Result.m32 = 0.0f;
		Result.m33 = 1.0f;
	}

	void XRotRad4x4(Mat4x4& Result, float angle) /// turn current matrix into a rotation matrix by radian
	{
		Result.m00 = 1.0f;
		Result.m01 = 0.0f;
		Result.m02 = 0.0f;
		Result.m02 = 0.0f;

		Result.m10 = 0.0f;
		Result.m11 = cosf(angle);
		Result.m12 = -sinf(angle);
		Result.m12 = 0.0f;

		Result.m20 = 0.0f;
		Result.m21 = sinf(angle);
		Result.m22 = cosf(angle);
		Result.m23 = 0.0f;

		Result.m30 = 0.0f;
		Result.m31 = 0.0f;
		Result.m32 = 0.0f;
		Result.m33 = 1.0f;
	}

	void YRotRad4x4(Mat4x4& Result, float angle) /// turn current matrix into a rotation matrix by radian
	{
		Result.m00 = cosf(angle);
		Result.m01 = 0.0f;
		Result.m02 = sinf(angle);;
		Result.m02 = 0.0f;

		Result.m10 = 0.0f;
		Result.m11 = 1.0f;
		Result.m12 = 0.0f;
		Result.m12 = 0.0f;

		Result.m20 = -sinf(angle);
		Result.m21 = 0.0f;
		Result.m22 = cosf(angle);
		Result.m23 = 0.0f;

		Result.m30 = 0.0f;
		Result.m31 = 0.0f;
		Result.m32 = 0.0f;
		Result.m33 = 1.0f;
	}

	void ZRotRad4x4(Mat4x4& Result, float angle) /// turn current matrix into a rotation matrix by radian
	{
		Result.m00 = cosf(angle * 3.14159265359f / 180);
		Result.m01 = -sinf(angle * 3.14159265359f / 180);
		Result.m02 = 0.0f;
		Result.m02 = 0.0f;

		Result.m10 = sinf(angle * 3.14159265359f / 180);
		Result.m11 = cosf(angle * 3.14159265359f / 180);
		Result.m12 = 0.0f;
		Result.m12 = 0.0f;

		Result.m20 = 0.0f;
		Result.m21 = 0.0f;
		Result.m22 = 1.0f;
		Result.m23 = 0.0f;

		Result.m30 = 0.0f;
		Result.m31 = 0.0f;
		Result.m32 = 0.0f;
		Result.m33 = 1.0f;
	}

	void RotRad4x4(Mat4x4& Result, float angle, Vec3 Dir)
	{
		if (Dir.x != 0)
		{

			XRotRad4x4(Result, angle);
		}


		if (Dir.y != 0)
		{

			YRotRad4x4(Result, angle);
		}


		if (Dir.z != 0)
		{

			ZRotRad4x4(Result, angle);
		}


	}

	void RotDeg4x4(Mat4x4& Result, float angle, Vec3 Dir)
	{
		if (Dir.x >= 0.0f)
		{

			XRotDeg4x4(Result, angle);
		}


		if (Dir.y >= 0.0f)
		{

			YRotDeg4x4(Result, angle);
		}


		if (Dir.z >= 0.0f)
		{

			ZRotDeg4x4(Result, angle);
		}
	}




	void XRotDeg4x4(Mat4x4& Result, float angle) /// turn current matrix into a rotation matrix by radian
	{
		Result.m00 = 1.0f;
		Result.m01 = 0.0f;
		Result.m02 = 0.0f;
		Result.m02 = 0.0f;

		Result.m10 = 0.0f;
		Result.m11 = cosf(angle);
		Result.m12 = -sinf(angle);
		Result.m12 = 0.0f;

		Result.m20 = 0.0f;
		Result.m21 = sinf(angle);
		Result.m22 = cosf(angle);
		Result.m23 = 0.0f;

		Result.m30 = 0.0f;
		Result.m31 = 0.0f;
		Result.m32 = 0.0f;
		Result.m33 = 1.0f;
	}

	void YRotDeg4x4(Mat4x4& Result, float angle) /// turn current matrix into a rotation matrix by radian
	{
		Result.m00 = cosf(angle);
		Result.m01 = 0.0f;
		Result.m02 = sinf(angle);
		Result.m02 = 0.0f;

		Result.m10 = 0.0f;
		Result.m11 = 1.0f;
		Result.m12 = 0.0f;
		Result.m12 = 0.0f;

		Result.m20 = -sinf(angle);
		Result.m21 = 0.0f;
		Result.m22 = cosf(angle);
		Result.m23 = 0.0f;

		Result.m30 = 0.0f;
		Result.m31 = 0.0f;
		Result.m32 = 0.0f;
		Result.m33 = 1.0f;
	}


	void ZRotDeg4x4(Mat4x4& Result, float angle)/// turn current matrix into a rotation matrix by radian
	{
		Result.m00 = cosf(angle);
		Result.m01 = -sinf(angle);
		Result.m02 = 0.0f;
		Result.m02 = 0.0f;

		Result.m10 = sinf(angle);
		Result.m11 = cosf(angle);
		Result.m12 = 0.0f;
		Result.m12 = 0.0f;

		Result.m20 = 0.0f;
		Result.m21 = 0.0f;
		Result.m22 = 1.0f;
		Result.m23 = 0.0f;

		Result.m30 = 0.0f;
		Result.m31 = 0.0f;
		Result.m32 = 0.0f;
		Result.m33 = 1.0f;
	}

	void Transpose4x4(Mat4x4& Result, const Mat4x4& pMtx) /// turn current matrix into a transpose matrix
	{
		Mat4x4 temp = pMtx;
		Result.m00 = temp.m00;
		Result.m01 = temp.m10;
		Result.m02 = temp.m20;
		Result.m03 = temp.m30;

		Result.m10 = temp.m01;
		Result.m11 = temp.m11;
		Result.m12 = temp.m21;
		Result.m13 = temp.m31;

		Result.m20 = temp.m02;
		Result.m21 = temp.m12;
		Result.m22 = temp.m22;
		Result.m23 = temp.m32;

		Result.m30 = temp.m03;
		Result.m31 = temp.m13;
		Result.m32 = temp.m23;
		Result.m33 = temp.m33;
	}


	Mat4x4 operator*(const Mat4x4& lhs, const Mat4x4& rhs) ///multiply 2 matrix using the * operator
	{
		Mat4x4 temp;

		temp.m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30;
		temp.m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21 + lhs.m03 * rhs.m31;
		temp.m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22 + lhs.m03 * rhs.m32;
		temp.m03 = lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23 + lhs.m03 * rhs.m33;

		temp.m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20 + lhs.m13 * rhs.m30;
		temp.m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31;
		temp.m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32;
		temp.m13 = lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33;

		temp.m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20 + lhs.m23 * rhs.m30;
		temp.m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31;
		temp.m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32;
		temp.m23 = lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33;

		temp.m30 = lhs.m30 * rhs.m00 + lhs.m31 * rhs.m10 + lhs.m32 * rhs.m20 + lhs.m33 * rhs.m30;
		temp.m31 = lhs.m30 * rhs.m01 + lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31;
		temp.m32 = lhs.m30 * rhs.m02 + lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32;
		temp.m33 = lhs.m30 * rhs.m03 + lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33;

		return temp;
	}



	Vec3 operator * (const Mat4x4& pMtx, const Vec3& rhs)
	{
		Vec3 temp;
		temp.x = pMtx.m00 * rhs.x + pMtx.m01 * rhs.y + pMtx.m02 * rhs.z + pMtx.m03; //find the X value and pass it into the x parameter of temp
		temp.y = pMtx.m10 * rhs.x + pMtx.m11 * rhs.y + pMtx.m12 * rhs.z + pMtx.m13;  //find the y value and pass it into the y parameter of temp
		temp.z = pMtx.m20 * rhs.x + pMtx.m21 * rhs.y + pMtx.m22 * rhs.z + pMtx.m23;  //find the z value and pass it into the z parameter of temp
		return temp;
	}
	Vec4 operator * (const Mat4x4& pMtx, const Vec4& rhs)
	{
		Vec4 temp;
		temp.x = pMtx.m00 * rhs.x + pMtx.m01 * rhs.y + pMtx.m02 * rhs.z + pMtx.m03 * rhs.w; //find the X value and pass it into the x parameter of temp
		temp.y = pMtx.m10 * rhs.x + pMtx.m11 * rhs.y + pMtx.m12 * rhs.z + pMtx.m13 * rhs.w;  //find the y value and pass it into the y parameter of temp
		temp.z = pMtx.m20 * rhs.x + pMtx.m21 * rhs.y + pMtx.m22 * rhs.z + pMtx.m23 * rhs.w;
		temp.w = pMtx.m30 * rhs.x + pMtx.m31 * rhs.y + pMtx.m32 * rhs.z + pMtx.m33 * rhs.w; //find the w value and pass it into the z parameter of temp
		return temp;
	}





	void Mtx33Inverse4x4(Mat4x4* pResult, float* determinant, const Mat4x4& pMtx)
	{
		Mat4x4 temp;

		*determinant = pMtx.m00 * (pMtx.m11 * pMtx.m22 * pMtx.m33 + pMtx.m12 * pMtx.m23 * pMtx.m31 + pMtx.m13 * pMtx.m21 * pMtx.m32
			- pMtx.m13 * pMtx.m22 * pMtx.m31 - pMtx.m12 * pMtx.m21 * pMtx.m33 - pMtx.m11 * pMtx.m23 * pMtx.m32)
			- pMtx.m10 * (pMtx.m01 * pMtx.m22 * pMtx.m33 + pMtx.m02 * pMtx.m23 * pMtx.m31 + pMtx.m03 * pMtx.m21 * pMtx.m32
				- pMtx.m03 * pMtx.m22 * pMtx.m31 - pMtx.m02 * pMtx.m21 * pMtx.m33 - pMtx.m01 * pMtx.m23 * pMtx.m32)
			+ pMtx.m20 * (pMtx.m01 * pMtx.m12 * pMtx.m33 + pMtx.m02 * pMtx.m13 * pMtx.m31 + pMtx.m03 * pMtx.m11 * pMtx.m32
				- pMtx.m03 * pMtx.m12 * pMtx.m31 - pMtx.m02 * pMtx.m11 * pMtx.m33 - pMtx.m01 * pMtx.m13 * pMtx.m32)
			- pMtx.m30 * (pMtx.m01 * pMtx.m12 * pMtx.m23 + pMtx.m02 * pMtx.m13 * pMtx.m21 + pMtx.m03 * pMtx.m11 * pMtx.m22
				- pMtx.m03 * pMtx.m12 * pMtx.m21 - pMtx.m02 * pMtx.m11 * pMtx.m23 - pMtx.m01 * pMtx.m13 * pMtx.m22);

		float inveDet = 1 / (*determinant);

		float ptemp00 = pMtx.m11 * (pMtx.m22 * pMtx.m33 - pMtx.m23 * pMtx.m32)
			- pMtx.m12 * (pMtx.m21 * pMtx.m33 - pMtx.m23 * pMtx.m31)
			+ pMtx.m13 * (pMtx.m21 * pMtx.m32 - pMtx.m22 * pMtx.m31);

		float ptemp01 = pMtx.m01 * (pMtx.m22 * pMtx.m33 - pMtx.m23 * pMtx.m32)
			- pMtx.m02 * (pMtx.m21 * pMtx.m33 - pMtx.m23 * pMtx.m31)
			+ pMtx.m03 * (pMtx.m21 * pMtx.m32 - pMtx.m22 * pMtx.m31);

		float ptemp02 = pMtx.m01 * (pMtx.m12 * pMtx.m33 - pMtx.m13 * pMtx.m32)
			- pMtx.m02 * (pMtx.m11 * pMtx.m33 - pMtx.m13 * pMtx.m31)
			+ pMtx.m03 * (pMtx.m11 * pMtx.m32 - pMtx.m12 * pMtx.m31);

		float ptemp03 = pMtx.m11 * (pMtx.m12 * pMtx.m23 - pMtx.m13 * pMtx.m22)
			- pMtx.m12 * (pMtx.m11 * pMtx.m23 - pMtx.m13 * pMtx.m21)
			+ pMtx.m13 * (pMtx.m11 * pMtx.m22 - pMtx.m12 * pMtx.m21);

		//.............................................................................
		float ptemp10 = pMtx.m10 * (pMtx.m22 * pMtx.m33 - pMtx.m23 * pMtx.m32)
			- pMtx.m12 * (pMtx.m20 * pMtx.m33 - pMtx.m23 * pMtx.m30)
			+ pMtx.m13 * (pMtx.m20 * pMtx.m32 - pMtx.m22 * pMtx.m30);

		float ptemp11 = pMtx.m00 * (pMtx.m22 * pMtx.m33 - pMtx.m23 * pMtx.m32)
			- pMtx.m02 * (pMtx.m20 * pMtx.m33 - pMtx.m23 * pMtx.m30)
			+ pMtx.m03 * (pMtx.m20 * pMtx.m32 - pMtx.m22 * pMtx.m30);

		float ptemp12 = pMtx.m00 * (pMtx.m12 * pMtx.m33 - pMtx.m13 * pMtx.m32)
			- pMtx.m02 * (pMtx.m10 * pMtx.m33 - pMtx.m13 * pMtx.m30)
			+ pMtx.m03 * (pMtx.m10 * pMtx.m32 - pMtx.m12 * pMtx.m30);

		float ptemp13 = pMtx.m00 * (pMtx.m12 * pMtx.m23 - pMtx.m13 * pMtx.m22)
			- pMtx.m02 * (pMtx.m10 * pMtx.m23 - pMtx.m13 * pMtx.m20)
			+ pMtx.m03 * (pMtx.m10 * pMtx.m22 - pMtx.m12 * pMtx.m20);

		//.............................................................................
		float ptemp20 = pMtx.m10 * (pMtx.m21 * pMtx.m33 - pMtx.m23 * pMtx.m31)
			- pMtx.m11 * (pMtx.m20 * pMtx.m33 - pMtx.m23 * pMtx.m30)
			+ pMtx.m13 * (pMtx.m20 * pMtx.m31 - pMtx.m21 * pMtx.m30);

		float ptemp21 = pMtx.m00 * (pMtx.m21 * pMtx.m33 - pMtx.m23 * pMtx.m31)
			- pMtx.m01 * (pMtx.m20 * pMtx.m33 - pMtx.m23 * pMtx.m30)
			+ pMtx.m03 * (pMtx.m20 * pMtx.m31 - pMtx.m21 * pMtx.m30);

		float ptemp22 = pMtx.m00 * (pMtx.m11 * pMtx.m33 - pMtx.m13 * pMtx.m31)
			- pMtx.m01 * (pMtx.m10 * pMtx.m33 - pMtx.m13 * pMtx.m30)
			+ pMtx.m03 * (pMtx.m10 * pMtx.m31 - pMtx.m11 * pMtx.m30);

		float ptemp23 = pMtx.m00 * (pMtx.m11 * pMtx.m23 - pMtx.m13 * pMtx.m21)
			- pMtx.m01 * (pMtx.m10 * pMtx.m23 - pMtx.m13 * pMtx.m20)
			+ pMtx.m03 * (pMtx.m10 * pMtx.m21 - pMtx.m11 * pMtx.m20);

		//.............................................................................

		float ptemp30 = pMtx.m10 * (pMtx.m21 * pMtx.m32 - pMtx.m22 * pMtx.m31)
			- pMtx.m11 * (pMtx.m20 * pMtx.m32 - pMtx.m22 * pMtx.m30)
			+ pMtx.m12 * (pMtx.m20 * pMtx.m31 - pMtx.m21 * pMtx.m30);

		float ptemp31 = pMtx.m00 * (pMtx.m21 * pMtx.m32 - pMtx.m22 * pMtx.m31)
			- pMtx.m01 * (pMtx.m20 * pMtx.m32 - pMtx.m22 * pMtx.m30)
			+ pMtx.m02 * (pMtx.m20 * pMtx.m31 - pMtx.m21 * pMtx.m30);

		float ptemp32 = pMtx.m00 * (pMtx.m11 * pMtx.m32 - pMtx.m12 * pMtx.m31)
			- pMtx.m01 * (pMtx.m10 * pMtx.m32 - pMtx.m12 * pMtx.m30)
			+ pMtx.m02 * (pMtx.m10 * pMtx.m31 - pMtx.m11 * pMtx.m30);

		float ptemp33 = pMtx.m00 * (pMtx.m11 * pMtx.m22 - pMtx.m12 * pMtx.m21)
			- pMtx.m01 * (pMtx.m10 * pMtx.m22 - pMtx.m12 * pMtx.m20)
			+ pMtx.m02 * (pMtx.m10 * pMtx.m21 - pMtx.m11 * pMtx.m20);

		temp.m00 = inveDet * ptemp00;
		temp.m01 = inveDet * -ptemp01;
		temp.m02 = inveDet * ptemp02;
		temp.m03 = inveDet * -ptemp03;

		temp.m10 = inveDet * -ptemp10;
		temp.m11 = inveDet * ptemp11;
		temp.m12 = inveDet * -ptemp12;
		temp.m13 = inveDet * ptemp13;

		temp.m20 = inveDet * ptemp20;
		temp.m21 = inveDet * -ptemp21;
		temp.m22 = inveDet * ptemp22;
		temp.m23 = inveDet * -ptemp23;

		temp.m30 = inveDet * -ptemp30;
		temp.m31 = inveDet * ptemp31;
		temp.m32 = inveDet * -ptemp32;
		temp.m33 = inveDet * ptemp33;


		*pResult = temp;


	}


	glm::mat4 mtx_adapter(const Mat4x4& rhs)
	{

		return (glm::mat4{ rhs.m00, rhs.m01, rhs.m02, rhs.m03,
							 rhs.m10, rhs.m11, rhs.m12, rhs.m13,
							 rhs.m20, rhs.m21, rhs.m22, rhs.m23,
							 rhs.m30, rhs.m31, rhs.m32, rhs.m33 });

	}

	glm::mat4 mtx_adapter(Mat4x4& rhs)
	{
		return (glm::mat4{ rhs.m00, rhs.m01, rhs.m02, rhs.m03,
							 rhs.m10, rhs.m11, rhs.m12, rhs.m13,
							 rhs.m20, rhs.m21, rhs.m22, rhs.m23,
							 rhs.m30, rhs.m31, rhs.m32, rhs.m33 });
	}

	Mat4x4 basemtx_adapterC(const glm::mat4& rhs)
	{
		Mat4x4 temp{ rhs[0][0], rhs[0][1], rhs[0][2], rhs[0][3],
						rhs[1][0], rhs[1][1], rhs[1][2], rhs[1][3],
						rhs[2][0], rhs[2][1], rhs[2][2], rhs[2][3],
						rhs[3][0], rhs[3][1], rhs[3][2], rhs[3][3] };
		return temp;
	}

	Mat4x4& Mat4x4::operator=(glm::mat4& rhs)
	{
		m00 = rhs[0][0];
		m01 = rhs[0][1];
		m02 = rhs[0][2];
		m03 = rhs[0][3];
		m10 = rhs[1][0];
		m11 = rhs[1][1];
		m12 = rhs[1][2];
		m13 = rhs[1][3];
		m20 = rhs[2][0];
		m21 = rhs[2][1];
		m22 = rhs[2][2];
		m23 = rhs[2][3];
		m30 = rhs[3][0];
		m31 = rhs[3][1];
		m32 = rhs[3][2];
		m33 = rhs[3][3];
		return *this;
	}
}