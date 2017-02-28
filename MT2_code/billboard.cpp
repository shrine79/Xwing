#include "main.h"
#include "math.h"
#include "primitive.h"
#include "utility.h"
#include "billboard.h"



billboard::billboard(GLuint tex, bb_render_mode r, bb_blend_mode b, bb_orientation ori, GLfloat w, GLfloat h)
{
	texHandle = tex; 
	orientation = ori;
	setPos(vec3(0, 0, 0));
	blend_mode = b; 
	render_mode = r; 
	setupTex(tex, w, h); 
	omniRotate = 0; 
}


billboard::~billboard()
{
}



void billboard::setPos(vec3 p)
{
	pos = p;
}

void billboard::setOmniRot(GLfloat angle)
{
	omniRotate = angle; 
}

void billboard::setupTex(GLuint tex, GLfloat w, GLfloat h)
{
	texHandle = tex;
	width = w;
	height = h;
}

void billboard::drawBoard(GLfloat w, GLfloat h)
{
	vec3 p0 = vec3(-0.5 * w, -0.5 * h, 0);
	vec3 p1 = vec3(-0.5 * w,  0.5 * h, 0);
	vec3 p2 = vec3( 0.5 * w,  0.5 * h, 0);
	vec3 p3 = vec3( 0.5 * w, -0.5 * h, 0);

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 0.0);     glVertex3fv(p3.fv());
	glTexCoord2f(1.0, 1.0);   glVertex3fv(p2.fv());
	glTexCoord2f(0, 1.0);     glVertex3fv(p1.fv());
	glTexCoord2f(0, 0.0);     glVertex3fv(p0.fv());

	glEnd();

	
}

void billboard::drawBoardAlpha(GLfloat w, GLfloat h, GLuint handle)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 1, 1);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	drawBoard(w, h); 

	glDisable(GL_BLEND);
	if (!ModulateTex) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	if (!showTex) glDisable(GL_TEXTURE_2D);
}

void billboard::drawBoardAdd(GLfloat w, GLfloat h, GLuint handle)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	drawBoard(w, h);

	glDisable(GL_BLEND);
	if (ModulateTex) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (!showTex) glDisable(GL_TEXTURE_2D);
}

void billboard::renderSemiDirctional()
{
	vec3 sight = vec3(eyePoint) - pos;
	// Calculat the angle between the sight vector and z-y plane
	GLfloat theta = atan2f(sight.x, sight.z); 

	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z); 
	glRotatef(theta*180/M_PI, 0, 1, 0); 
	if (blend_mode == bb_blend_mode::alpha)
		drawBoardAlpha(width, height, texHandle);
	else
		drawBoardAdd(width, height, texHandle);

	//primitive::drawSphere(GL_FLAT);
	glPopMatrix(); 
}

void billboard::renderOmniDirectional()
{
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);

	removeModelRot();

	glRotatef(omniRotate, 0, 0, -1); 
	if (blend_mode == bb_blend_mode::alpha)
		drawBoardAlpha(width, height, texHandle);
	else
		drawBoardAdd(width, height, texHandle);

	glPopMatrix();

}

void billboard::renderRotatedMultiDirectional()
{
	glPushMatrix();
		glRotatef(upAngle_m, rotAxis_m.x, rotAxis_m.y, rotAxis_m.z); // rotate to plane head angle
		renderMultiDirectional();
	glPopMatrix(); 
}

void billboard::renderMultiDirectional()
{
	glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		if(orientation == horizental) glRotatef(90, -1, 0, 0);  // lay down the up right board. 
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);
		//glDisable(GL_DEPTH_TEST);

		if (blend_mode == bb_blend_mode::alpha)
			drawBoardAlpha(width, height, texHandle);
		else
			drawBoardAdd(width, height, texHandle);

		// turn the board 30 degrees around its height and draw the 2nd board. 
		glRotatef(30, 0, 1, 0);
		if (blend_mode == bb_blend_mode::alpha)
			drawBoardAlpha(width, height, texHandle);
		else
			drawBoardAdd(width, height, texHandle);

		// turn the board 30 degrees around its height and draw the 3rd board. 
		glRotatef(30, 0, 1, 0);
		if (blend_mode == bb_blend_mode::alpha)
			drawBoardAlpha(width, height, texHandle);
		else
			drawBoardAdd(width, height, texHandle);

		// turn the board 30 degrees around its height and draw the 4th board. 
		glRotatef(30, 0, 1, 0); 
		if (blend_mode == bb_blend_mode::alpha)
			drawBoardAlpha(width, height, texHandle);
		else
			drawBoardAdd(width, height, texHandle);

		// turn the board 30 degrees around its height and draw the 5th board. 
		glRotatef(30, 0, 1, 0);
		if (blend_mode == bb_blend_mode::alpha)
			drawBoardAlpha(width, height, texHandle);
		else
			drawBoardAdd(width, height, texHandle);

		// turn the board 30 degrees around its height and draw the 6th board. 
		glRotatef(30, 0, 1, 0);
		if (blend_mode == bb_blend_mode::alpha)
			drawBoardAlpha(width, height, texHandle);
		else
			drawBoardAdd(width, height, texHandle);

		if(hiddenCull) glEnable(GL_CULL_FACE);
		//if (hiddenDepth) glEnable(GL_DEPTH_TEST);

		glDepthMask(GL_TRUE);
	glPopMatrix();
}

void billboard::renderRotatedMultiDirectionalWithEndinMiddle()
{
	glPushMatrix();
		glRotatef(upAngle_m, rotAxis_m.x, rotAxis_m.y, rotAxis_m.z); // rotate to plane head angle
		renderMultiDirectionalWithEndinMiddle();
	glPopMatrix();
}

void billboard::renderMultiDirectionalWithEndinMiddle()
{
	glDisable(GL_CULL_FACE);
	// glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(90, -1, 0, 0); 
		// draw the end
		if (blend_mode == bb_blend_mode::alpha)
			drawBoardAlpha(width, width, end_texHandle); // end_height and end_width has to match width all the time
		else
			drawBoardAdd(width, width, end_texHandle); // end_height and end_width has to match width all the time
	glPopMatrix();
	glDepthMask(GL_TRUE);
	if (hiddenCull) glEnable(GL_CULL_FACE);
	//if (hiddenDepth) glEnable(GL_DEPTH_TEST);

	renderMultiDirectional();

}