#include "quaternion.h"



// the default constructor makes an pure imaginary quaternion
// The w = cos(theta/2) where theta represent the rotation angle. w = 1 where the rotation angle = 0; 
// So the constructed quaternion represent a rotation around origin(0,0,0,0) by 0 degree. 
quaternion::quaternion() : w(1.0f), i(0.0f), j(0.0f), k(0.0f)
{
}

quaternion::quaternion(const quaternion& q): w(q.w), i(q.i), j(q.j), k(q.k)
{
}

// four arguments constructor. This will construct a quaternion based on the rotation angle and the rotation axis vector. 
// the new quaternion will be normalized to unit quaternion
quaternion::quaternion(const float degree, const float x, const float y, const float z)
{
	float rad = degree * (float)M_PI / 180.0f;
#pragma warning(push)
#pragma warning(disable : 4244)
	w = cos(rad / 2);
	i = x*sin(rad / 2);
	j = y*sin(rad / 2);
	k = z*sin(rad / 2);
#pragma warning(pop)
	normalize();
}


quaternion::quaternion(float degree, const vec3& v) // create quaternion from w and vec3
{
	float rad = degree * (float)M_PI / 180.0f;
#pragma warning(push)
#pragma warning(disable : 4244)
	w = cos(rad / 2);
	i = v.x*sin(rad / 2);
	j = v.y*sin(rad / 2);
	k = v.z*sin(rad / 2);
#pragma warning(pop)
	normalize();
}
quaternion::quaternion(float const *pf) // constructor takes array of float data. assumed to be stored in order of degree, x, y, z. 
{
	float rad = pf[0] * (float)M_PI / 180.0f;
#pragma warning(push)
#pragma warning(disable : 4244)
	w = cos(rad / 2);
	i = pf[1] * sin(rad / 2);
	j = pf[2] * sin(rad / 2);
	k = pf[3] * sin(rad / 2);
#pragma warning(pop)
	normalize();
}

quaternion::~quaternion()
{

}

