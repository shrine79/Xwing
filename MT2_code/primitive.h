#ifndef _primitive_
#define _primitive_

#include "glut.h"

class primitive
{
public:
	primitive();

	static void drawTriPrism();
	static void drawQuad();
	static void drawCone(GLfloat smallEndRadius, GLenum  glunormals);
	static void drawTriangle();
	static void drawSphere(GLenum glunormals);
	static void drawCylinderWithEnds(GLenum glunormals, GLuint tHandle_disk, GLuint tHandle_cylinder);
	static void drawConeWithEnds(GLfloat baseRadius, GLfloat topRadius, GLuint handle, GLenum glunormals);
	static void drawDisk(GLfloat innerRadius, GLenum glunormals, GLuint tHandle);
	static void drawCylinder(GLenum glunormals, GLuint tHandle);
	static void drawCubeWithNormals(GLuint texHandle, GLfloat texScale = 1);
	static void drawWireCubeWithNormals();
	static void drawTeapot();
	static void drawCurve(int slices, GLfloat angle, GLuint handle);
	static void drawCurveWith2Tex(int slices, GLfloat angle, GLuint handle1, GLuint handle2);
	static void primitive::drawQuadMultiTex();
	~primitive();
};

#endif

