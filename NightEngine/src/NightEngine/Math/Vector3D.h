#pragma once

#include "MathHeader.h"

#include "Vector4D.h"

#pragma warning( disable : 4201 )

namespace Night
{
	/*!*************************************************************************
	 This is a container use to contain the each individual value of a constructed
	 3D vector
	 ****************************************************************************/

	union Vec3
	{
		struct {
			float x;
			float y;
			float z;
		};



		float m[3]{ 0,0,0 };
		/*!*************************************************************************
		This Function is a defualt constructor for the 3D vector
		 ****************************************************************************/
		Vec3();
		/*!*************************************************************************
		This Function can be use to mannually construct a 3D Vector
		****************************************************************************/
		Vec3(float _x, float _y, float _z); //para_Ctor

		/*!*************************************************************************
		This Function is an aisgnment operator is to perform addition on the left
		hand vector using the right hand vector
		****************************************************************************/
		Vec3 operator += (const Vec3& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform subtraction on the left
		hand vector using the right hand vector
		****************************************************************************/
		Vec3 operator -= (const Vec3& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform multiplication on the left
		hand vector using the right hand vector
		****************************************************************************/
		Vec3 operator *= (const Vec3& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform divison on the left
		hand vector using the right hand vector
		****************************************************************************/
		Vec3 operator /= (const Vec3& rhs);
		/*!*************************************************************************
		This Function is used to copy the value of the right hand 3D Vector into
		the left hand 3D vevtor
		****************************************************************************/
		Vec3 operator = (const Vec3& lhs);
		/*!*************************************************************************
		This Function is used to copy the value of the right hand 4D Vector into
		the left hand 3D vevtor
		****************************************************************************/
		Vec3 operator = (const Vec4& lhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform addition on the left
		hand vector using a float
		****************************************************************************/
		Vec3& operator += (float& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform subtraction on the left
		hand vector using a float
		****************************************************************************/
		Vec3& operator -= (float& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is used to convert the vector into a negative
		vector
		****************************************************************************/
		Vec3 operator -() const;
	};

	/*!*************************************************************************
		This Function is an operator used to perform subtraction between 2 vector
		****************************************************************************/
	Vec3 operator - (const Vec3& lhs, const Vec3& rhs);
	/*!*************************************************************************
		This Function is an operator used to perform addition between 2 vector
		****************************************************************************/
	Vec3 operator + (const Vec3& lhs, const Vec3& rhs);
	/*!*************************************************************************
		This Function is an operator used to perform muliplication between a vector and a  right hand float
		****************************************************************************/
	Vec3 operator * (const Vec3& lhs, float rhs);
	/*!*************************************************************************
		This Function is an operator used to perform muliplication between a vector and a
		left hand float
		****************************************************************************/
	Vec3 operator * (float lhs, const Vec3& rhs);
	/*!*************************************************************************
		This Function is an operator used to perform divion between a vector and a float
		****************************************************************************/
	Vec3 operator / (const Vec3& lhs, float rhs);

	/*!*************************************************************************
		This Function is used to normalize a vector
		****************************************************************************/
	void Normalize(Vec3& result, const Vec3 Vec0); //normalize the vector

	/*!*************************************************************************
		This Function is used to calculate and return the length of a vector
		****************************************************************************/
	float length(const Vec3& Vec0); // return the length of the vector 
	/*!*************************************************************************
		This Function is used to calculate and return the square length of a vector
		****************************************************************************/
	float squarelength(const Vec3& Vec0);//return the length of the vector after squaring it
	/*!*************************************************************************
		This Function is used to calculate and return the distance between 2 point
		****************************************************************************/
	float distance(const Vec3& vec_p0, const Vec3& vec_p1);// find the distance 2 3D vector that are being treated as point 
	/*!*************************************************************************
		This Function is used to calculate and return the square distance between 2 point
		****************************************************************************/
	float squaredistance(const Vec3& vec_p0, const Vec3& vec_p1);
	/*!*************************************************************************
		This Function is used to calculate and return the dotproduct between 2 vrctor
		****************************************************************************/
	float dotProduct(const Vec3& Vec0, const Vec3& Vec1);
	/*!*************************************************************************
		This Function is used to calculate and return the crossproduct between 2 vrctor
		****************************************************************************/
	Vec3 crossproduct(const Vec3& Vec0, const Vec3& Vec1);
}