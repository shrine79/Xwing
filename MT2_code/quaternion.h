#ifndef _QUAT_
#define _QUAT_

/*
*  ----------------------------------------------------------
*   quaternion.h
*   by Yang Ye, University of Canberra
*   Version 1.0,  2016.
*   Last modified by Yang Ye 21/10/2016.
*   ref: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/arithmetic/
* -----------------------------------------------------------
*/


#define _USE_MATH_DEFINES

#include "vec3.h"
#include "math.h"

class quaternion
{
public:

	// data
	float w, i, j, k; // Mathmatically the x,y,z part should be called i, j, k as the imaginary part of quaternion. 
					  //They are equivalent to the 3D vector x, y, z.

	/* ----------- Constructors and destructors -------------*/

	quaternion(); // the default constructor makes an pure real quaternion
	quaternion(const quaternion& q); // copy constructor
	quaternion(const float degree, const float x, const float y, const float z); // four arguments constructor
	quaternion(float degree, const vec3& v); // create quaternion q2 from vec3 (w = cos(degree), i = x* sin(degree), j = y*sin(degree), k = z*sin(degree))
	quaternion(float const *pf); // constructor takes array of float data. assumed to be stored in order of w, x, y, z. 

	inline void setVector(const vec3& v); // make a special quaternion to represent a point in 3D space. w is always 0 in this case. 
	inline vec3 getVector(void); // return the pure imaginary part of the quaternion, this could be considered to represent the vector (x, y, z) in 3D space.

	/* ---------- 2 arguments Operators overload ------------*/
	friend inline quaternion operator*(const quaternion& q1, const quaternion& q2);  // Multiply q1 by q2, eg. q1 * q2; Note, it's not commutative, the order matters. 
																				     // basically the Hamilteon multiplication. 
	
	friend inline quaternion operator+(const quaternion& q1, const quaternion& q2); // quaternion addition is not different from vector addition but in 4D. 
	friend inline quaternion operator-(const quaternion& q1, const quaternion& q2); // quaternion substraction q1 - q2; 

	/* ----------- 1 argument functions ---------------*/
	inline quaternion operator*(const float a); // The scalar mulitiplication as vector
	inline quaternion operator/(const float a); // the scalar division as vector

	/* ------------- Member functions ---------------*/
	inline void normalize(void); // normalize this quaternion. 
	inline quaternion getConjugated(void); // return the conjugated quaternion 
	inline bool isNormalized(void); // Test the sqr(length) of quaternion, it may need to be normalized again due to loss of precision. 
									// Don't test length to avoid square-root operation. 
	inline float norm(void); // return the length of quaternion. 
	friend inline quaternion mix(quaternion& qa, quaternion& qb, float t); // the implemetation of geometirc SLERP interpolation. 
																		   // Note: this function may normalize the original qa and qb.

	~quaternion();
};

/* Free functions are friends to quaternion class*/

inline quaternion operator*(const quaternion& q1, const quaternion& q2);  // Multiply q1 by q2, eg. q1 * q2; Note, it's not commutative, the order matters. 
																				 // basically the Hamilteon multiplication. 

inline quaternion operator+(const quaternion& q1, const quaternion& q2); // quaternion addition is not different from vector addition but in 4D. 
inline quaternion operator-(const quaternion& q1, const quaternion& q2); // quaternion substraction q1 - q2; 

inline float quaternion::norm(void)
{
	float norm = sqrtf(w*w + i*i + j*j + k*k); 
	return norm;
}

inline void quaternion::normalize(void)
{
	float n = norm();
	if (n == 0)
	{
		// do nothing for zero length quaternion, it's illeagal and cause divid by zero error
		// just make unit quaternion (1,0,0,0). 
		w = 1.0f;
		i = 0.0f;
		j = 0.0f;
		k = 0.0f;
	}
	else 
	{
		float inv_n = 1 / n; // little speed optimization 
		w *= inv_n; 
		i *= inv_n; 
		j *= inv_n;
		k *= inv_n;
	}
}

// return the conjugated quaternion 
inline quaternion quaternion::getConjugated(void)
{
	quaternion qr;
	qr.i = -i; 
	qr.j = -j; 
	qr.k = -k;
	qr.w = w;
	return qr;
}

// Test the sqr(length) of quaternion, it may need to be normalized again due to loss of precision. 
// Don't test length to avoid square-root operation. 
inline bool quaternion::isNormalized(void)
{
	return (norm() == 1) ? TRUE : FALSE;
}


inline void quaternion::setVector(const vec3& v)
{
	// the vector is not normalized.
	w = 0; 
	i = v.x;
	j = v.y;
	k = v.z;
}

inline vec3 quaternion::getVector(void)
{
	return vec3(i, j, k); 
}

// Multiply q1 by q2, eg. q1 * q2; Note, it's not commutative, the order matters. 
// basically the Hamilteon multiplication. 
// It's defined as 
/*
(a + i b + j c + k d)*(e + i f + j g + k h) 	

=
    a(e + i f + j g + k h)
+ i b(e + i f + j g + k h)
+ j c(e + i f + j g + k h)
+ k d(e + i f + j g + k h)
= 	
    a*e     + i a*f 	    + j a*g 	    + k a*h
+ i b*e 	+ i*i * b*f 	+ i*j * b*g 	+ i*k * b*h
+ j c*e 	+ j*i * c*f 	+ j*j * c*g 	+ j*k * c*h
+ k d*e 	+ k*i * d*f 	+ k*j * d*g 	+ k*k * d*h
= 	
    a*e 	+ i a*f 	+ j a*g 	+ k a*h
+ i b*e 	- b*f 	+ k * b*g 	-j * b*h
+ j c*e 	- k * c*f 	- c*g 	+ i * c*h
+ k d*e 	+ j * d*f 	-i * d*g 	- d*h
= 	

     a*e - b*f - c*g - d*h
+ i (b*e + a*f + c*h - d*g)
+ j (a*g - b*h + c*e + d*f)
+ k (a*h + b*g - c*f + d*e)

*/

inline quaternion operator*(const quaternion& q1, const quaternion& q2)
{
	quaternion qr;
	qr.w = q1.w*q2.w - q1.i*q2.i - q1.j*q2.j - q1.k*q2.k;
	qr.i = q1.i*q2.w + q1.w*q2.i + q1.j*q2.k - q1.k*q2.j;
	qr.j = q1.w*q2.j - q1.i*q2.k + q1.j*q2.w + q1.k*q2.i;
	qr.k = q1.w*q2.k + q1.i*q2.j - q1.j*q2.i + q1.k*q2.w;

	return qr;
}

// quaternion addition is not different from vector addition but in 4D. 
inline quaternion operator+(const quaternion& q1, const quaternion& q2) 
{
	quaternion qr;
	qr.i = q1.i + q2.i;
	qr.j = q1.j + q2.j;
	qr.k = q1.k + q2.k;
	qr.w = q1.w + q2.w;
	return qr;
}

// quaternion substraction q1 - q2; 
inline quaternion operator-(const quaternion& q1, const quaternion& q2)
{
	quaternion qr;
	qr.w = q1.w - q2.w; 
	qr.i = q1.i - q2.i;
	qr.j = q1.j - q2.j;
	qr.k = q1.k - q2.k;
	return qr;
}
// The scalar mulitiplication as vector
inline quaternion quaternion::operator*(const float a)
{
	quaternion qr;
	qr.w = w * a; 
	qr.i = i * a;
	qr.j = j * a;
	qr.k = k * a;
	return qr; 
}

// the scalar division as vector
inline quaternion quaternion::operator/(const float a)
{
	return (*this) * (1 / a);
}

// the implemetation of geometirc SLERP interpolation. 
// ref:: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/
// The mix function was developed by the operator of this website with assistance from 
// Anthony(Prospero) and Josh; 
// It's modified by Yang Ye to suit this class

inline quaternion mix(quaternion& qa, quaternion& qb, float t)
{
#pragma warning(push)
#pragma warning(disable : 4244)
	// quaternion to return
	quaternion qm;

	// Yang YE: we have to make sure the two qaternions are both unit quaternion before we calculate the angle by dot product. 
	if (!qa.isNormalized())
		qa.normalize(); 
	if (!qb.isNormalized())
		qb.normalize();

	// Calculate angle between them. Yang YE: Given the fact that dot(qa, qb) = cos(theta)*|qa|*|qb|
	double cosHalfTheta = qa.w * qb.w + qa.i * qb.i + qa.j * qb.j + qa.k * qb.k;
	
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (abs(cosHalfTheta) >= 1.0) {
		qm = qa; 
		return qm;
	}

	// Calculate temporary values.
	double halfTheta = acos(cosHalfTheta);
	double sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);

	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	// Yang Ye: so move qm a little bit away from qa by a small random amount; 
	if (fabs(sinHalfTheta) < 0.001) { // fabs is floating point absolute
		qm.w = (qa.w * 0.9 + qb.w * 0.1);
		qm.i = (qa.i * 0.9 + qb.i * 0.1);
		qm.j = (qa.j * 0.9 + qb.j * 0.1);
		qm.k = (qa.k * 0.9 + qb.k * 0.1);
		return qm;
	}

	double ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
	double ratioB = sin(t * halfTheta) / sinHalfTheta;
	//calculate Quaternion.
	qm.w = (qa.w * ratioA + qb.w * ratioB);
	qm.i = (qa.i * ratioA + qb.i * ratioB);
	qm.j = (qa.j * ratioA + qb.j * ratioB);
	qm.k = (qa.k * ratioA + qb.k * ratioB);
	return qm;
#pragma warning(pop)
}





#endif
