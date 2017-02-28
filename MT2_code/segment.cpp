
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "segment.h"
#include "primitive.h"
#include "lighting.h"
#include "glut.h"

#include "main.h"

segment::segment()
{
	length_v = 4.0; 
	width_v = 2.0;
	height_v = 7.5; 
	thinkness_v = 0.1;
	colorPost = red;
	colorPlank = black;
}


segment::~segment()
{

}

void segment::setup(GLfloat length, GLfloat width, GLfloat height,
	GLfloat thinkness, GLfloat * colPost, GLfloat * colPlank)
{
	length_v = length;
	width_v = width;
	height_v = height;
	thinkness_v = thinkness;
	colorPost = colPost;
	colorPlank = colPlank;
}

int segment::render()
{
	if (showWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (showTex) glEnable(GL_TEXTURE_2D); 
	else glDisable(GL_TEXTURE_2D);

	// draw posts
	glPushMatrix();
		glTranslatef(0, -4.0, 0); // move down the segment
		glRotatef(180, 0.0, 1.0, 0.0);
		glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT);
		glColor3fv(red);
		setMaterial(4); 
		drawPost();

		glPushMatrix();
			glTranslatef(width_v, 0.0, 0.0);
			drawPost();
		glPopMatrix();

		// Draw planks 
		glTranslatef(width_v /2, height_v - 0.5, length_v/2);
		glScalef(width_v, 0.1, length_v);
		glColor3fv(brown);
		primitive::drawCubeWithNormals(TexHandles[0]);

		glPopAttrib();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	return true; 
}

void segment::drawPost()
{
	//glEnable(GL_TEXTURE_2D);
	// Draw post which is 3 meters high and 0.2 in diameters. 
	glPushMatrix();
	glScalef(0.1, height_v, 0.1);
	primitive::drawCylinderWithEnds(GL_FLAT, TexHandles[2], TexHandles[2]); //Handle == 0 means no texture specified. 
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, height_v, 0.0);
	glScalef(0.1, 0.1, 0.1); 
	glPushAttrib(GL_CURRENT_BIT| GL_LIGHTING_BIT);
	glColor3fv(cyan);
	setMaterial(3);
	primitive::drawCylinderWithEnds(GL_FLAT, 0, 0); //Handle == 0 means no texture specified. 
	glPopAttrib();
	glPopMatrix();


}


