#include"Vector3.h"
#include <cmath>

//’·‚³‚ğ‹‚ß‚é
float Vector3::length() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}

//³‹K‰»
Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}

	return *this;
}

//“àÏ
float Vector3::dot(const Vector3& v)const
{
	return  (x * v.x) + (y * v.y) + (z * v.z);
}

//ŠOÏ
Vector3 Vector3::cross(const Vector3& v)const
{
	return Vector3((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x));
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return  temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}