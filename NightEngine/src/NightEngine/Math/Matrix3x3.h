#pragma once

#include "Vector2D.h"
#include "Vector3D.h"

namespace Night
{
	/*!*************************************************************************
	 This is a container use to contain the each individual value of a constructed
	 3 by 3 matrix
	 ****************************************************************************/
	typedef union Mat3D
	{
		struct
		{
			float m00, m01, m02;
			float m10, m11, m12;
			float m20, m21, m22;
		};

		float m[3][3];

		/*!*************************************************************************
		 This Function is a defualt constructor for the 3x3 matrix
		 ****************************************************************************/
		Mat3D();
		/*!*************************************************************************
		This Function use an array to construct a 3x3 matrix
		****************************************************************************/
		Mat3D(const float* pArray);
		/*!*************************************************************************
		This Function can be use to mannually construct a 3x3 matrix
		****************************************************************************/
		Mat3D(float _00, float _01, float _02,
			float _10, float _11, float _12,
			float _20, float _21, float _22);// mannual para ctor
		/*!*************************************************************************
		This Function use 3 seperate vector to construct a 3x3 matrix
		****************************************************************************/
		Mat3D(Vec3 v1, Vec3 v2, Vec3 v3);// vector para ctor
		/*!*************************************************************************
		This Function construct an identity 3x3 matrix using the given float value
		****************************************************************************/
		Mat3D(float value);// identity ctor


		/*!*************************************************************************
		This Function is a copy operator used to copy the data from the right matrix
		into the left matrix
		****************************************************************************/
		Mat3D& operator=(const Mat3D& rhs); // copy ctor

		/*!*************************************************************************
		This Function is an aisgnment operator is to perform matrix multiplication on the left
		hand matrix using the right hand matrix
		****************************************************************************/
		Mat3D& operator *= (const Mat3D& rhs);//assignment operator
	}Mat3D, Mtx33, MTX;

	//operator 
	/*!*************************************************************************
		This Function is a multiplication operator is to perform matrix multiplication
		between the 2 matrice
		****************************************************************************/
	Mat3D operator * (const Mat3D& lhs, const Mat3D& rhs);

	/*!*************************************************************************
		This Function is a multiplication operator is to perform matrix multiplication
		between a vector and a matrix
		****************************************************************************/
	vec2D  operator * (const Mat3D& pMtx, const vec2D& rhs);

	/*!*************************************************************************
		This Function is used to convert the current matrix into an indentity matrix
		****************************************************************************/
	void Identity(Mat3D& pResult);
	/*!*************************************************************************
		This Function is used to convert the current matrix into a Translation matrix
		****************************************************************************/
	void Translate(Mat3D& pResult, float x, float y);
	/*!*************************************************************************
		This Function is used to convert the current matrix into a scaling matrix
		****************************************************************************/
	void Scale(Mat3D& pResult, float x, float y);
	/*!*************************************************************************
		This Function is used to rotate matrix with the given radian value
		****************************************************************************/
	void RotRad(Mat3D& pResult, float angle);
	/*!*************************************************************************
		This Function is used to rotate matrix with the given degree value
		****************************************************************************/
	void RotDeg(Mat3D& pResult, float angle);
	/*!*************************************************************************
		This Function is used to convert the current matrix into a transpose matrix
		****************************************************************************/
	void Transpose(Mat3D& pResult, const Mat3D& pMtx);
	/*!*************************************************************************
		This Function is used to convert the current matrix into an inverse 3x3 matrix
		****************************************************************************/
	void Mtx33Inverse(Mat3D* pResult, float* determinant, const Mat3D& pMtx);
}