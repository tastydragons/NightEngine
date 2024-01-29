#include "hzpch.h"

#include "Matrix3x3.h"

namespace Night
{
	Mat3D::Mat3D() :
		m00{ 0 }, m01{ 0 }, m02{ 0 },
		m10{ 0 }, m11{ 0 }, m12{ 0 },
		m20{ 0 }, m21{ 0 }, m22{ 0 }
	{}

	Mat3D::Mat3D(const float* pArray) :
		m00{ pArray[0] }, m01{ pArray[1] }, m02{ pArray[2] },
		m10{ pArray[3] }, m11{ pArray[4] }, m12{ pArray[5] },
		m20{ pArray[6] }, m21{ pArray[7] }, m22{ pArray[8] }
	{}///initialized each data of the matrix 

	Mat3D::Mat3D(float _00, float _01, float _02,
		float _10, float _11, float _12,
		float _20, float _21, float _22) :
		m00{ _00 }, m01{ _01 }, m02{ _02 },
		m10{ _10 }, m11{ _11 }, m12{ _12 },
		m20{ _20 }, m21{ _21 }, m22{ _22 }

	{} ///initialized each data of the matrix to the respective float given

	Mat3D::Mat3D(Vec3 v1, Vec3 v2, Vec3 v3) :
		m00{ v1.x }, m01{ v2.x }, m02{ v3.x },
		m10{ v1.y }, m11{ v2.y }, m12{ v3.y },
		m20{ v1.z }, m21{ v2.z }, m22{ v3.z }
	{


	}

	Mat3D::Mat3D(float value) :
		m00{ value }, m01{ 0 }, m02{ 0 },
		m10{ 0 }, m11{ value }, m12{ 0 },
		m20{ 0 }, m21{ 0 }, m22{ value }
	{


	}

	Mat3D& Mat3D::operator=(const Mat3D& rhs) //copy ctor
	{
		m00 = rhs.m00;
		m01 = rhs.m01;
		m02 = rhs.m02;
		m10 = rhs.m10;
		m11 = rhs.m11;
		m12 = rhs.m12;
		m20 = rhs.m20;
		m21 = rhs.m21;
		m22 = rhs.m22;
		return *this;
	}

	Mat3D& Mat3D::operator*=(const Mat3D& rhs) ///multiply the matrix
	{
		Mat3D temp;
		temp.m00 = (m00 * rhs.m00) + (m01 * rhs.m10) + (m02 * rhs.m20);
		temp.m01 = (m00 * rhs.m01) + (m01 * rhs.m11) + (m02 * rhs.m21);
		temp.m02 = (m00 * rhs.m02) + (m01 * rhs.m12) + (m02 * rhs.m22);

		temp.m10 = (m10 * rhs.m00) + (m11 * rhs.m10) + (m12 * rhs.m20);
		temp.m11 = (m10 * rhs.m01) + (m11 * rhs.m11) + (m12 * rhs.m21);
		temp.m12 = (m10 * rhs.m02) + (m11 * rhs.m12) + (m12 * rhs.m22);

		temp.m20 = (m20 * rhs.m00) + (m21 * rhs.m10) + (m22 * rhs.m20);
		temp.m21 = (m20 * rhs.m01) + (m21 * rhs.m11) + (m22 * rhs.m21);
		temp.m22 = (m20 * rhs.m02) + (m21 * rhs.m12) + (m22 * rhs.m22);

		return *this = temp;
	}

	Mat3D operator*(const Mat3D& lhs, const Mat3D& rhs) ///multiply 2 matrix using the * operator
	{
		Mat3D temp;

		temp.m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20;
		temp.m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21;
		temp.m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22;

		temp.m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20;
		temp.m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21;
		temp.m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22;

		temp.m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20;
		temp.m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21;
		temp.m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22;

		return temp;
	}

	vec2D  operator * (const Mat3D& pMtx, const vec2D& rhs) ///multiply a matrix and a vector using the * operator
	{
		vec2D temp;
		temp.x = pMtx.m00 * rhs.x + pMtx.m01 * rhs.y + pMtx.m02; //find the X value and pass it into the x parameter of temp
		temp.y = pMtx.m10 * rhs.x + pMtx.m11 * rhs.y + pMtx.m12;  //find the y value and pass it into the y parameter of temp
		return temp;
	}

	void Identity(Mat3D& Result) /// turn current matrix into an identity matrix
	{
		Result.m00 = 1.0f;
		Result.m01 = 0.0f;
		Result.m02 = 0.0f;

		Result.m10 = 0.0f;
		Result.m11 = 1.0f;
		Result.m12 = 0.0f;

		Result.m20 = 0.0f;
		Result.m21 = 0.0f;
		Result.m22 = 1.0f;
	}

	void Translate(Mat3D& Result, float x, float y) /// turn current matrix into a translation matrix
	{
		Result.m00 = 1.0f;
		Result.m01 = 0.0f;
		Result.m02 = x;

		Result.m10 = 0.0f;
		Result.m11 = 1.0f;
		Result.m12 = y;

		Result.m20 = 0.0f;
		Result.m21 = 0.0f;
		Result.m22 = 1.0f;
	}

	void Scale(Mat3D& Result, float x, float y) /// turn current matrix into a scaling matrix
	{
		Result.m00 = x;
		Result.m01 = 0.0f;
		Result.m02 = 0.0f;

		Result.m10 = 0.0f;
		Result.m11 = y;
		Result.m12 = 0.0f;

		Result.m20 = 0.0f;
		Result.m21 = 0.0f;
		Result.m22 = 1.0f;
	}

	void RotRad(Mat3D& Result, float angle) /// turn current matrix into a rotation matrix by radian
	{
		Result.m00 = cosf(angle);
		Result.m01 = -sinf(angle);
		Result.m02 = 0.0f;

		Result.m10 = sinf(angle);
		Result.m11 = cosf(angle);
		Result.m12 = 0.0f;

		Result.m20 = 0.0f;
		Result.m21 = 0.0f;
		Result.m22 = 1.0f;
	}

	void RotDeg(Mat3D& Result, float angle) /// turn current matrix into a rotation matrix by degree
	{
		Result.m00 = cosf(angle * 3.14159265359f / 180);
		Result.m01 = -sinf(angle * 3.14159265359f / 180);
		Result.m02 = 0.0f;

		Result.m10 = sinf(angle * 3.14159265359f / 180);
		Result.m11 = cosf(angle * 3.14159265359f / 180);
		Result.m12 = 0.0f;

		Result.m20 = 0.0f;
		Result.m21 = 0.0f;
		Result.m22 = 1.0f;
	}

	void Transpose(Mat3D& Result, const Mat3D& pMtx) /// turn current matrix into a transpose matrix
	{
		Result.m00 = pMtx.m00;
		Result.m01 = pMtx.m10;
		Result.m02 = pMtx.m20;

		Result.m10 = pMtx.m01;
		Result.m11 = pMtx.m11;
		Result.m12 = pMtx.m12;

		Result.m20 = pMtx.m20;
		Result.m21 = pMtx.m21;
		Result.m22 = pMtx.m22;
	}


	void Mtx33Inverse(Mat3D* Result, float* determinant, const Mat3D& pMtx)///inverse the matrix 
	{
		Mat3D temp;

		*determinant = pMtx.m00 * (pMtx.m11 * pMtx.m22 - pMtx.m12 * pMtx.m21)
			- pMtx.m01 * (pMtx.m10 * pMtx.m22 - pMtx.m12 * pMtx.m20)
			+ pMtx.m02 * (pMtx.m10 * pMtx.m21 - pMtx.m11 * pMtx.m20);

		if (*determinant == 0.0f)
		{
			Result = NULL;
		}

		else
		{
			temp.m00 = (pMtx.m11 * pMtx.m22 - pMtx.m12 * pMtx.m21) / *determinant;
			temp.m01 = (pMtx.m21 * pMtx.m02 - pMtx.m22 * pMtx.m01) / *determinant;
			temp.m02 = (pMtx.m01 * pMtx.m12 - pMtx.m02 * pMtx.m11) / *determinant;

			temp.m10 = (pMtx.m20 * pMtx.m12 - pMtx.m10 * pMtx.m22) / *determinant;
			temp.m11 = (pMtx.m00 * pMtx.m22 - pMtx.m20 * pMtx.m02) / *determinant;
			temp.m12 = (pMtx.m10 * pMtx.m02 - pMtx.m00 * pMtx.m12) / *determinant;

			temp.m20 = (pMtx.m10 * pMtx.m21 - pMtx.m11 * pMtx.m20) / *determinant;
			temp.m21 = (pMtx.m20 * pMtx.m01 - pMtx.m00 * pMtx.m21) / *determinant;
			temp.m22 = (pMtx.m00 * pMtx.m11 - pMtx.m01 * pMtx.m10) / *determinant;

			*Result = temp;
		}
	}
}