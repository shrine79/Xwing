#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <conio.h>

#include "main.h"
#include "glut.h"
#include "utility.h"

void * New (int size)
{
  void * ptr;
  char   msg[256];
  
  ptr = calloc(1, size);
  if (ptr == NULL) {
    sprintf(msg, "Unable to allocate %d bytes in New", size);
    Fail(msg);
  }
  return ptr;
}

void Fail (String message)
{
  fprintf(stderr, "%s\n", message);
  getch();
  exit(1);
}

void FailNull (void * ptr, String source)
{
  char msg[256];
  
  if (ptr == NULL) {
    sprintf(msg, "NULL pointer: %s", source);
    Fail(msg);
  }
}

void FailErr (int code, String source)
{
  char msg[256];
  
  if (code < 0) {
    if (errno == 0)
      sprintf(msg, "Error code %d: %s", code, source);
    else
      sprintf(msg, "Error code %d: %s", errno, source);
    Fail(msg);
  }
}

GLfloat Radians(GLfloat degreesZ)
{
  return degreesZ * M_PI / 180.0;
}

GLfloat Degrees(GLfloat radiansZ)
{
	return radiansZ * 180.0 / M_PI;
}




void rotate2D(GLfloat oldx, GLfloat oldz,
	GLfloat angle, GLfloat speed, // angle in degrees
	GLfloat *resultx, GLfloat *resultz)
{
	GLfloat tmpx, tmpz;
	GLfloat newx, newz;
	float yaw; // yaw is an angle in radians

	yaw = angle*3.1416 / 180;


	tmpx = 0;/* set to origin */
	tmpz = speed; /* move us forward by speed */

	newx = (tmpz * sin(yaw)) + (tmpx * cos(yaw));
	newz = (tmpz * cos(yaw)) - (tmpx * sin(yaw));
	tmpx = newx;
	tmpz = newz;

	newx = tmpx + oldx; // adjust because it was around the origin
	newz = tmpz + oldz;

	*resultx = newx;
	*resultz = newz;
}


vec3 quaternionRotation(const vec3& point, const vec3& origin, const vec3& axis, GLfloat degree)
{
	// make the point quaternion (location of the sun)
	// translate the rotation origin to sun origin
	quaternion p;
	p.setVector(point - origin);

	// make the unit rotation quaternion
	quaternion quat = quaternion(degree, axis);
	// build a conjugate of rotation quaternion
	quaternion quat_conj = quat.getConjugated();

	// calculate the new position of the point after the rotation about rotation axis by w degree
	quaternion pr = quat * p * quat_conj;

	// extract the new point vector and transfer back to world coordinate. 
	return pr.getVector() + origin; 
}

GLfloat dist_point_to_line_seg(vec3 p0, vec3 p1, vec3 p, vec3& p_hit)
{
	vec3 v = p1 - p0; // the vector represents the line segment 
	vec3 w = p - p0; // the vector represents the line from p0 to the point

	GLfloat c1 = dot(w, v);
	if (c1 <= 0)
	{
		// in this case, 'pb' < p0 < p1.  
		// where 'pb' is the projection of the point on the infinite line.
		// therefore replace the 'pb' with 'p0' for distance calculation
		p_hit = p0; 
		return dist(p0, p);
	}

	GLfloat c2 = dot(v, v);
	if (c2 <= c1)
	{
		// in this case, p0 < p1 < pb
		// where 'pb' is the projection of the point on the infinite line.
		// therefore replace the 'pb' with 'p1' for distance calculation
		p_hit = p1; 
		return dist(p1, p);
	}

	// In this case, pb is on the line segment, calculate the position of 'pb'
	GLfloat b = c1 / c2; // the ration of the projection of w and v on the infinite line 
	vec3 pb = p0 + b * v; // apply the ratio to find pb
	p_hit = pb;
	return dist(p, pb);
}

GLfloat vec2PlaneAngle(vec3 L0, vec3 L1, vec3 v0)
{
	// Calculate the normal of the plane formed by line of sight and up right
	vec3 tempNorm = normalize(cross(L0, L1));
	// find the projection of up vector on the normal, than calculate the angle between the normal and up vector
	GLfloat tempAngle = 180 * acos((dot(v0, tempNorm))) / M_PI;
	// the rotate angle is the complementary of the angle above
	return 90 - tempAngle;
}

// the origin is assumed to be on the plane
// the normal of the plane is denoted by n
// the vector is denoted by v
vec3 projectVecOnPlane(vec3 n, vec3 v)
{
	return v - dot(v, n) * n; 
}

// remove rotations from current modelview matrix
// NOTE: this function will change the matrix mode to ModelView mode after execution. 
void removeModelRot()
{
	GLfloat cur_mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, cur_mat);

	// copy the last column over. 
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			if (i == j) cur_mat[i * 4 + j] = 1;
			else cur_mat[i * 4 + j] = 0;
		}

	glMatrixMode(GL_MODELVIEW); // ensure the matrix mode
	glLoadMatrixf(cur_mat);
}

sineTab::sineTab()
{
	tab = new float[SIN_TAB_MAX];
	for (int i = 1; i < SIN_TAB_MAX; i++)
		tab[i] = sinf(i * M_PI / SIN_TAB_MAX);

	tab[0] = 0; // assign sin(0) value
}

sineTab::~sineTab()
{
	delete[] tab;
}

float sineTab::getSin(float multipleOf2PI)
{
	int sign = 1;
	float ret; 
	if (multipleOf2PI < 0) sign = -1;
	// get the fraction part of the float radian, multipleOf2PI is converted to the range of -PI to PI. 
	float r = (multipleOf2PI - (int)(multipleOf2PI) - (0.5*sign));
	// convert to multiple of PI
	int idx = r * SIN_TAB_MAX * 2;
	if (idx < 0) sign = -1;
	else sign = 1; 
	idx = sign * idx; 
	ret = sign * tab[idx];
	return ret; 
}