#include "hzpch.h"

#include "Vector3D.h"

namespace Night
{
	Vec3::Vec3()
	{


	}

	Vec3::Vec3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;

	}

	Vec3 Vec3::operator += (const Vec3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}
	Vec3 Vec3::operator -= (const Vec3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}
	Vec3 Vec3::operator *= (const Vec3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;

		return *this;
	}
	Vec3 Vec3::operator /= (const Vec3& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;

		return *this;
	}
	Vec3 Vec3::operator = (const Vec3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		return *this;
	}

	Vec3 Vec3::operator = (const Vec4& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		return *this;
	}
	Vec3& Vec3::operator += (float& rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;

		return *this;
	}
	Vec3& Vec3::operator -= (float& rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;


		return *this;
	}

	Vec3 Vec3::operator -() const
	{
		return Vec3(-x, -y, -z);
	}





	void Normalize(Vec3& result, const Vec3 Vec0)
	{
		float mag = sqrtf((Vec0.x * Vec0.x) + (Vec0.y * Vec0.y) + (Vec0.z * Vec0.z));
		result.x = Vec0.x / mag;
		result.y = Vec0.y / mag;
		result.z = Vec0.z / mag;

	}

	float  length(const Vec3& Vec0) ///calculate the length of the vector
	{
		return (float)(sqrt(powf(Vec0.x, 2.0f) + powf(Vec0.y, 2.0f) + powf(Vec0.z, 2.0f)));
	}

	float squarelength(const Vec3& Vec0)
	{
		return (float)(powf(Vec0.x, 2.0f) + powf(Vec0.y, 2.0f) + powf(Vec0.z, 2.0f));
	}

	float distance(const Vec3& vec_p0, const Vec3& vec_p1)
	{
		Vec3 temp = vec_p1 - vec_p0;
		return length(temp);
	}

	float squaredistance(const Vec3& vec_p0, const Vec3& vec_p1)
	{
		Vec3 temp = vec_p1 - vec_p0;//find the vector between the 2 point

		return powf(length(temp), 2.0f);//return the result after running the temp value through Length();
	}

	float dotProduct(const Vec3& Vec0, const Vec3& Vec1)
	{
		return (Vec0.x * Vec1.x + Vec0.y * Vec1.y + Vec0.z * Vec1.z);
	}

	Vec3 crossproduct(const Vec3& Vec0, const Vec3& Vec1)
	{
		Vec3 temp;
		temp.x = Vec0.y * Vec1.z - Vec0.z * Vec1.y;
		temp.y = Vec0.z * Vec1.x - Vec0.x * Vec1.z;
		temp.z = Vec0.x * Vec1.y - Vec0.y * Vec1.x;
		return temp;
	}

	Vec3 operator - (const Vec3& lhs, const Vec3& rhs)
	{

		return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}
	Vec3 operator + (const Vec3& lhs, const Vec3& rhs)
	{
		return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}
	Vec3 operator * (const Vec3& lhs, float rhs)
	{
		return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}
	Vec3 operator * (float lhs, const Vec3& rhs)
	{
		return Vec3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}

	Vec3 operator / (const Vec3& lhs, float rhs)
	{
		return Vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}
}