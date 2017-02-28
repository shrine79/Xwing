#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"
#include "glext.h"
#include "utility.h"
#include "glUtils.h"
#include "main.h"


#include "primitive.h"





primitive::primitive()
{
}


primitive::~primitive()
{
}


// ---------------------- primitives ------------------------

void primitive::drawCubeWithNormals(GLuint texHandle, GLfloat texScale)
{
	glPushMatrix();
	if (showWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	
	cube->renderWithNormals(texScale);
	glPopMatrix();
}

void primitive::drawWireCubeWithNormals()
{
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	cube->renderWithNormals(1);
	glPopMatrix();

	if (showWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void primitive::drawCylinder(GLenum glunormals, GLuint tHandle)
{
	//
	// glunormals values are GLU_NONE, GLU_FLAT, GLU_SMOOTH.
	// you may also need to set glShadeModel(GLenum  mode) to GL_FLAT or GL_SMOOTH
	//
	if (showWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();
	//glTranslatef(0,0,0);
	gluQuadricOrientation(quadric1, GLU_OUTSIDE);
	gluQuadricNormals(quadric1, glunormals);
	gluQuadricTexture(quadric1, GL_TRUE); 
	glBindTexture(GL_TEXTURE_2D, tHandle);
	gluCylinder(quadric1, 0.5, 0.5, 1, 17, 4);
	glPopMatrix();
}


void primitive::drawDisk(GLfloat innerRadius, GLenum glunormals, GLuint tHandle)
{
	// inner raduis 0 = full disk 0.5 = no disk or circle
	glPushMatrix();
	gluQuadricOrientation(quadric3, GLU_OUTSIDE);
	gluQuadricNormals(quadric3, glunormals);
	gluQuadricTexture(quadric1, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, tHandle);
	gluDisk(quadric3, innerRadius, 0.5, 17, 4);
	glPopMatrix();
}

void primitive::drawCylinderWithEnds(GLenum glunormals, GLuint tHandle_disk, GLuint tHandle_cylinder)
{
	//
	// glunormals values are GLU_NONE, GLU_FLAT, GLU_SMOOTH.
	// you may also need to set glShadeModel(GLenum  mode) to GL_FLAT or GL_SMOOTH
	//

	// student code here

	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	drawCylinder(glunormals, tHandle_cylinder);
	drawDisk(0.0, glunormals, tHandle_disk);
	glTranslatef(0.0, 0.0, 1.0);
	drawDisk(0.0, glunormals, tHandle_disk);
	glPopMatrix();


}

void primitive::drawCone(GLfloat smallEndRadius, GLenum  glunormals)
{
	glPushMatrix();
	glTranslatef(0, 0, -0.5);
	gluQuadricOrientation(quadric4, GLU_OUTSIDE);
	gluQuadricNormals(quadric4, glunormals);
	gluCylinder(quadric4, 0.5, smallEndRadius, 1, 17, 4);
	glPopMatrix();
}


void primitive::drawConeWithEnds(GLfloat baseRadius, GLfloat topRadius, GLuint handle, GLenum glunormals)
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluQuadricOrientation(quadric1, GLU_OUTSIDE);
	gluQuadricNormals(quadric1, glunormals);
	glBindTexture(GL_TEXTURE_2D, handle);
	gluCylinder(quadric1, baseRadius, topRadius, 1, 17, 4);

	gluQuadricOrientation(quadric3, GLU_INSIDE);
	gluQuadricNormals(quadric3, glunormals);
	gluQuadricTexture(quadric3, GL_TRUE);
	gluDisk(quadric3, 0, baseRadius, 17, 4);

	glTranslatef(0.0, 0.0, 1.0);
	gluQuadricOrientation(quadric3, GLU_OUTSIDE);
	gluQuadricNormals(quadric3, glunormals);
	gluQuadricTexture(quadric3, GL_TRUE);
	gluDisk(quadric3, 0, topRadius, 17, 4);
	glPopMatrix();
}

void primitive::drawSphere(GLenum glunormals)
{
	glPushMatrix();
	gluQuadricOrientation(quadric2, GLU_OUTSIDE);
	gluQuadricNormals(quadric2, glunormals);
	gluSphere(quadric2, 0.5, 13, 13);
	glPopMatrix();
}

void primitive::drawTriangle()
{
	glPushMatrix();
	triangle->renderWithNormals();
	glPopMatrix();
}

void primitive::drawQuad()
{
	glPushMatrix();
	quad->renderWithNormals();
	glPopMatrix();
}

void primitive::drawQuadMultiTex()
{
	glPushMatrix();
	quad->renderWithNormals();
	glPopMatrix();
}

void primitive::drawTriPrism()
{
	glPushMatrix();
	triPrism->renderWithNormals();
	glPopMatrix();
}

void primitive::drawTeapot()
{
	if (showWireFrame)
	{
		glutWireTeapot(0.5);
	}
	else
	{
		glutSolidTeapot(0.5);
	}
}

void primitive::drawCurve(int slices, GLfloat angle, GLuint handle)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	int i;
	GLfloat len;
	GLfloat r = 0.5;
	if (angle > 179) angle = 179; // stop tan(90)
								  //rotate2D(GLfloat oldx, GLfloat oldz, GLfloat angle, GLfloat speed, GLfloat *resultx, GLfloat *resultz)

	glPushMatrix();
	for (i = 0; i < slices; i++)
	{
		glPushMatrix();
		//glTranslatef(0, 0, 0.5);
		glRotatef(angle*i, 0, 1, 0);
		glTranslatef(0, 0, r);
		len = tan(Radians(angle / 2)) * 2 * r;
		glScalef(len, 1, 1);
		glBindTexture(GL_TEXTURE_2D, handle);
		//primitive::drawQuad();
		quad->renderWithNormalsSingleTex(i, slices);
		if (showNormals)
			quad->renderTheNormals();
		glPopMatrix();
	}
	glPopMatrix();
}


void primitive::drawCurveWith2Tex(int slices, GLfloat angle, GLuint handle1, GLuint handle2)
{
	glBindTexture(GL_TEXTURE_2D, handle1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	int i;
	GLfloat len;
	GLfloat r = 0.5;
	if (angle > 179) angle = 179; // stop tan(90)
								  //rotate2D(GLfloat oldx, GLfloat oldz, GLfloat angle, GLfloat speed, GLfloat *resultx, GLfloat *resultz)

	glPushMatrix();

	for (i = 0; i < slices; i++)
	{
		glPushMatrix();
		glRotatef(angle*i, 0, 1, 0);
		glTranslatef(0, 0, r);
		len = tan(Radians(angle / 2)) * 2 * r;
		glScalef(len, 1, 1);
		quad->renderWithNormalsSingleTex(i, slices);
		glPopMatrix();
	}

	glBindTexture(GL_TEXTURE_2D, handle2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, 1, 1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(255,255, 255, 255);
	glDepthFunc(GL_LEQUAL); 
	glEnable(GL_BLEND);

	for (i = 0; i < slices; i++)
	{
		glPushMatrix();
		glRotatef(angle*i, 0, 1, 0);
		glTranslatef(0, 0, r);
		len = tan(Radians(angle / 2)) * 2 * r;
		glScalef(len, 1, 1);
		quad->renderWithNormalsSingleTex(i, slices);
		if (showNormals)
			quad->renderTheNormals();
		glPopMatrix();
	}

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);

	glPopMatrix();
	
}