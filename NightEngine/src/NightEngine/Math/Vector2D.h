#pragma once

#include "MathHeader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Night
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
	/*!*************************************************************************
	 This is a container use to contain the each individual value of a constructed
	 2D vector
	 ****************************************************************************/
	typedef union vec2D
	{
		struct {
			float x;
			float y;
		};


		float m[2];
		/*!*************************************************************************
		This Function is a defualt constructor for the 2D vector
		 ****************************************************************************/
		vec2D();//default ctor
		/*!*************************************************************************
		This Function can be use to mannually construct a 3D Vector
		****************************************************************************/
		vec2D(float _x, float _y); //para_Ctor

		//operators
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform addition on the left
		hand vector using the right hand vector
		****************************************************************************/
		vec2D operator += (const vec2D& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform subtraction on the left
		hand vector using the right hand vector
		****************************************************************************/
		vec2D operator -= (const vec2D& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform multiplication on the left
		hand vector using the right hand vector
		****************************************************************************/
		vec2D operator *= (const vec2D& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform divison on the left
		hand vector using the right hand vector
		****************************************************************************/
		vec2D operator /= (const vec2D& rhs);
		/*!*************************************************************************
		This Function is used to copy the value of the right hand 3D Vector into
		the left hand 3D vevtor
		****************************************************************************/
		vec2D operator = (const vec2D& lhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform addition on the left
		hand vector using a float
		****************************************************************************/
		vec2D& operator += (float& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is to perform subtraction on the left
		hand vector using a float
		****************************************************************************/
		vec2D& operator -= (float& rhs);
		/*!*************************************************************************
		This Function is an aisgnment operator is used to convert the vector into a negative
		vector
		****************************************************************************/
		vec2D operator -() const;

	}Vector2D;
	/*!*************************************************************************
		This Function is an operator used to perform subtraction between 2 vector
		****************************************************************************/
	vec2D operator - (const vec2D& lhs, const vec2D& rhs);
	/*!*************************************************************************
		This Function is an operator used to perform addition between 2 vector
		****************************************************************************/
	vec2D operator + (const vec2D& lhs, const vec2D& rhs);
	/*!*************************************************************************
		This Function is an operator used to perform muliplication between a vector and a  right hand float
		****************************************************************************/
	vec2D operator * (const vec2D& lhs, float rhs);
	/*!*************************************************************************
		This Function is an operator used to perform muliplication between a vector and a
		left hand float
		****************************************************************************/
	vec2D operator * (float lhs, const vec2D& rhs);
	/*!*************************************************************************
		This Function is an operator used to perform divion between a vector and a float
		****************************************************************************/
	vec2D operator / (const vec2D& lhs, float rhs);
	/*!*************************************************************************
		This Function is used to normalize a vector
		****************************************************************************/
	void Normalize(vec2D& result, const vec2D Vec0); //normalize the vector
	/*!*************************************************************************
	   This Function is used to calculate and return the length of a vector
	   ****************************************************************************/
	float length(const vec2D& Vec0); // return the length of the vector 
	/*!*************************************************************************
			This Function is used to calculate and return the square length of a vector
			****************************************************************************/
	float squarelength(const vec2D& vec0);//return the length of the vector after squaring it
	/*!*************************************************************************
		This Function is used to calculate and return the distance between 2 point
		****************************************************************************/
	float distance(const vec2D& vec_p0, const vec2D& vec_p1);// find the distance 2 2D vector that are being treated as point 
	/*!*************************************************************************
		This Function is used to calculate and return the square distance between 2 point
		****************************************************************************/
	float squaredistance(const vec2D& vec_p0, const vec2D& vec_p1);
	/*!*************************************************************************
		This Function is used to calculate and return the dotproduct between 2 vrctor
		****************************************************************************/
	float dotProduct(const vec2D& Vec0, const vec2D& Vec1);
	/*!*************************************************************************
		This Function is used to calculate and return the crossproduct between 2 vrctor
		****************************************************************************/
	float crossproduct(const vec2D& Vec0, const vec2D& Vec1);

	/*!*************************************************************************
		This Function is used to perform orthogonal projection between 2 vector
		****************************************************************************/
	vec2D  ADV_OrthProj(const vec2D& Vec0, const vec2D& Vec1);
	/*!*************************************************************************
		This Function is used to convert this math liabary's vec2 into glm vec2
		****************************************************************************/
	glm::vec2 vec2_adapter(vec2D& rhs);
	/*!*************************************************************************
		This Function is used to convert glm vec2 into  this math liabary's vec2
		****************************************************************************/
	vec2D basevec2_adapter(const glm::vec2& rhs);
}