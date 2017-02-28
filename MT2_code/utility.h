#ifndef _utility_
#define _utility_

#include "vec3.h"
#include "vec4.h"
#include "quaternion.h"
#include <math.h>
#pragma warning(disable : 4996) // Reduce unsafe warning messages

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SIN_TAB_MAX 1000

typedef int	Boolean;
typedef char *	String;

#ifndef TRUE
#define TRUE	(1)
#define FALSE	(0)
#endif

void * New (int size);

void Fail (String message);
void FailNull (void * ptr, String source);
void FailErr (int code, String source);

GLfloat Radians (GLfloat degreesZ);
GLfloat Degrees (GLfloat radiansZ);

void rotate2D(GLfloat oldx, GLfloat oldz, GLfloat angle, GLfloat speed, // angle in degrees
	GLfloat *resultx, GLfloat *resultz);

// return rotated vector by quaternion. 
vec3 quaternionRotation(const vec3& point, const vec3& origin, const vec3& axis, GLfloat degree); 

// return the angle between a vector and any plane
// The plane is defined by two lines denoted by L0 and L1
GLfloat vec2PlaneAngle(vec3 L0, vec3 L1, vec3 v0);

// return the projected vector of vector v on plane containing origin (0, 0, 0)
vec3 projectVecOnPlane(vec3 n, vec3 v); 

/* find the distance from a point to a line segment.
param: 
p0: the start point of line segment
p1: the end point of line segment
p : the point in question. 
*/
GLfloat dist_point_to_line_seg(vec3 p0, vec3 p1, vec3 p, vec3& p_hit);

// remove rotation from modelview current matrix
void removeModelRot(); 


class sineTab
{
protected:
	float * tab;
public:
	sineTab();
	~sineTab(); 

	// To speedup, the rad argument should be supplied with Radian/(2 * PI)
	float getSin(float multipleOf2PI); 
};



#endif
