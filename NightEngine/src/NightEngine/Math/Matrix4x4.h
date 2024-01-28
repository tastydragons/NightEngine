#pragma once

#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

#include "Matrix3x3.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Night
{
	/*!*************************************************************************
	 This is a container use to contain the each individual value of a constructed
	 4 by 4 matrix
	 ****************************************************************************/
	typedef union Mat4x4
	{
		struct
		{

			float m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33;
		};

		float m[4][4];

		/*!*************************************************************************
		 This Function is a defualt constructor for the 4x4 matrix
		 ****************************************************************************/
		Mat4x4();
		/*!*************************************************************************
		This Function can be use to mannually construct a 4x4 matrix
		****************************************************************************/
		Mat4x4(float _00, float _01, float _02, float _03,
			float _10, float _11, float _12, float _13,
			float _20, float _21, float _22, float _23,
			float _30, float _31, float _32, float _33);
		/*!*************************************************************************
		This Function is used to convert a 3x3 matrix into a 4x4 matrix
		****************************************************************************/
		Mat4x4(Mat3D mat);
		/*!*************************************************************************
		This Function is used to convert a glm 4x4 matrix into this  math liabary matrix
		****************************************************************************/
		Mat4x4(glm::mat4& rhs);
		/*!*************************************************************************
		This Function construct an identity 4x4 matrix using the given float value
		****************************************************************************/
		Mat4x4(float value);
		/*!*************************************************************************
		This Function is a copy operator used to copy the data from the right matrix
		into the left matrix
		****************************************************************************/
		Mat4x4& operator=(const Mat4x4& rhs);
		/*!*************************************************************************
		This Function is a copy operator used to copy the data from a glm 4x4matrix
		into the left matrix
		****************************************************************************/
		Mat4x4& operator=(glm::mat4& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform matrix multiplication on the left
		hand matrix using the right hand matrix
		****************************************************************************/
		Mat4x4& operator*= (const Mat4x4& rhs);



	}Matrix4x4;
	/*!*************************************************************************
	This Function is used to convert the current matrix into an indentity matrix
	****************************************************************************/
	void Identity4x4(Mat4x4& pResult);
	/*!*************************************************************************
	This Function is used to convert the current matrix into a manual Translation matrix
	****************************************************************************/
	Matrix4x4 Translate4x4(Mat4x4& pResult, float x, float y, float z);
	/*!*************************************************************************
	   This Function is used to convert the current matrix into a manual scaling matrix
	   ****************************************************************************/
	Matrix4x4 Scale4x4(Mat4x4& pResult, float x, float y, float z);

	/*!*************************************************************************
	   This Function is used to convert a 3 value vector into a scaling matrix
	   ****************************************************************************/
	void Scale4x4(Mat4x4& pResult, Vec3 temp);

	/*!*************************************************************************
	This helper Function is used to rotate matrix with the given radian value by the X axis
	****************************************************************************/
	void XRotRad4x4(Mat4x4& Result, float angle);
	/*!*************************************************************************
	This helper Function is used to rotate matrix with the given radian value By the Y axis
	****************************************************************************/
	void YRotRad4x4(Mat4x4& Result, float angle);
	/*!*************************************************************************
	This helper Function is used to rotate matrix with the given radian value By the Z axis
	****************************************************************************/
	void ZRotRad4x4(Mat4x4& Result, float angle);

	/*!*************************************************************************
	This Function is used to rotate matrix with the given radian value using vector
	to determine which axis it is rotating by
	****************************************************************************/
	void RotRad4x4(Mat4x4& Result, float angle, Vec3 dir);
	/*!*************************************************************************
	This Function is used to rotate matrix with the given degree value
	using the vector to determin with axis it is rotating by
	****************************************************************************/
	void RotDeg4x4(Mat4x4& Result, float angle, Vec3 dir);
	/*!*************************************************************************
	This helper Function is used to rotate matrix with the given degree value By the X axis
	****************************************************************************/
	void XRotDeg4x4(Mat4x4& Result, float angle);
	/*!*************************************************************************
	This helper Function is used to rotate matrix with the given degree value By the Y axis
	****************************************************************************/
	void YRotDeg4x4(Mat4x4& Result, float angle);
	/*!*************************************************************************
	This helper Function is used to rotate matrix with the given degree value By the Z axis
	****************************************************************************/
	void ZRotDeg4x4(Mat4x4& Result, float angle);
	/*!*************************************************************************
		This Function is used to convert the current matrix into a transpose matrix
		****************************************************************************/
	void Transpose4x4(Mat4x4& pResult, const Mat4x4& pMtx);
	/*!*************************************************************************
		This Function is used to convert the current matrix into an inverse 4x4 matrix
		****************************************************************************/
	void Mtx33Inverse4x4(Mat4x4* pResult, float* determinant, const Mat4x4& pMtx);

	/*!*************************************************************************
	This Function is a multiplication operator is to perform matrix multiplication
	between the 2 matrice
	****************************************************************************/
	Mat4x4 operator * (const Mat4x4& lhs, const Mat4x4& rhs);
	/*!*************************************************************************
	This Function is a multiplication operator is to perform matrix multiplication
	between a 3D vector and a matrix
	****************************************************************************/
	Vec3 operator * (const Mat4x4& pMtx, const Vec3& rhs);
	/*!*************************************************************************
	This Function is a multiplication operator is to perform matrix multiplication
	between a 4D vector and a matrix
	****************************************************************************/
	Vec4 operator * (const Mat4x4& pMtx, const Vec4& rhs);

	/*!*************************************************************************
	This Function is used to convert this liabary matrix into a GLM 4x4 matrix
	****************************************************************************/
	glm::mat4 mtx_adapter(Mat4x4& rhs);
	/*!*************************************************************************
	This Function is used to convert a const of version this liabary matrix
	into a GLM 4x4 matrix
	****************************************************************************/
	glm::mat4 mtx_adapter(const Mat4x4& rhs);
	/*!*************************************************************************
	This Function is used to convert a GLM 4x4 matrix into this liabary  4x4 matrix
	****************************************************************************/
	Mat4x4 basemtx_adapterC(const glm::mat4& rhs);
}