#pragma once

#include "MathHeader.h"

#pragma warning( disable : 4201 )

namespace Night
{
	/*!*************************************************************************
	 This is a container use to contain the each individual value of a constructed
	 4D vector
	 ****************************************************************************/

	union Vec4
	{
		struct {
			float x;
			float y;
			float z;
			float w;

		};



		float m[4];
		/*!*************************************************************************
		This Function is a defualt constructor for the 4D vector
		 ****************************************************************************/
		Vec4();//default ctor
		/*!*************************************************************************
		This Function can be use to mannually construct a 4D Vector
		****************************************************************************/
		Vec4(float _x, float _y, float _z, float _w); //para_Ctor

		//operators
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform addition on the left
		hand vector using the right hand vector
		****************************************************************************/
		Vec4 operator += (const Vec4& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform subtraction on the left
		hand vector using the right hand vector
		****************************************************************************/
		Vec4 operator -= (const Vec4& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform multiplication on the left
		hand vector using the right hand vector
		****************************************************************************/
		Vec4 operator *= (const Vec4& rhs);
		/*!*************************************************************************
	This Function is an aisgnment operator is to perform divison on the left
	hand vector using the right hand vector
	****************************************************************************/
		Vec4 operator /= (const Vec4& rhs);
		/*!*************************************************************************
		This Function is used to copy the value of the right hand 3D Vector into
		the left hand 3D vevtor
		****************************************************************************/
		Vec4 operator = (const Vec4& lhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform addition on the left
		hand vector using a float
		****************************************************************************/
		Vec4& operator += (float& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform subtraction on the left
		hand vector using a float
		****************************************************************************/
		Vec4& operator -= (float& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is used to convert the vector into a negative
		vector
		****************************************************************************/
		Vec4 operator -() const;
		/*!*************************************************************************
		This Function is used to set/change the value of existing vector
		****************************************************************************/
		void Vec4Set(Vec4& curr, float x_, float y_, float z_, float w_);
	};
	/*!*************************************************************************
		This Function is an operator used to perform subtraction between 2 vector
		****************************************************************************/
	Vec4 operator - (const Vec4& lhs, const Vec4& rhs);
	/*!*************************************************************************
			This Function is an operator used to perform addition between 2 vector
			****************************************************************************/
	Vec4 operator + (const Vec4& lhs, const Vec4& rhs);
	/*!*************************************************************************
		This Function is an operator used to perform muliplication between a vector and a  right hand float
		****************************************************************************/
	Vec4 operator * (const Vec4& lhs, float rhs);
	/*!*************************************************************************
		This Function is an operator used to perform muliplication between a vector and a
		left hand float
		****************************************************************************/
	Vec4 operator * (float lhs, const Vec4& rhs);
	/*!*************************************************************************
		This Function is an operator used to perform divion between a vector and a float
		****************************************************************************/
	Vec4 operator / (const Vec4& lhs, float rhs);
}