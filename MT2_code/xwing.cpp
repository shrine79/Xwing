#include "xwing.h"


//                              width_low						width_high								height
xwing::xwing(billboard * ptr) : s0{ static_cast<GLfloat>(2.0),			 static_cast<GLfloat>(1.0),				static_cast<GLfloat>(0.7),			static_cast<GLfloat>(LEN0) },
				 s1{ static_cast<GLfloat>(3.0),			 static_cast<GLfloat>(1.5),				static_cast<GLfloat>(2.0),			static_cast<GLfloat>(LEN1) },
				 s2{ static_cast<GLfloat>(3.0),			 static_cast<GLfloat>(4.0),				static_cast<GLfloat>(3.0),			static_cast<GLfloat>(LEN2) },
				 s3{ static_cast<GLfloat>(5.5),			 static_cast<GLfloat>(5.0),				static_cast<GLfloat>(4.0),			static_cast<GLfloat>(LEN3) },
				 s4{ static_cast<GLfloat>(6.0),			 static_cast<GLfloat>(6.0),				static_cast<GLfloat>(4.0),			static_cast<GLfloat>(0.0) },
				 s5{ static_cast<GLfloat>(2.0),			 static_cast<GLfloat>(1.5),				static_cast<GLfloat>(0.5),			static_cast<GLfloat>(COCKPIT_LEN0) },
				 s6{ static_cast<GLfloat>(4.0),			 static_cast<GLfloat>(3.5),				static_cast<GLfloat>(4.5),			static_cast<GLfloat>(COCKPIT_LEN1) },
				 s7{ static_cast<GLfloat>(4.0),			 static_cast<GLfloat>(3.0),				static_cast<GLfloat>(4.0),			static_cast<GLfloat>(COCKPIT_LEN2) },
				 s8{ static_cast<GLfloat>(5.0),			 static_cast<GLfloat>(4.0),				static_cast<GLfloat>(4.0),			static_cast<GLfloat>(0.0) },
				ws0{ static_cast<GLfloat>(WING_W0),	     static_cast<GLfloat>(WING_W0),			static_cast<GLfloat>(WING_THICK),   static_cast<GLfloat>(WING_LEN) },
				ws1{ static_cast<GLfloat>(WING_W1),		 static_cast<GLfloat>(WING_W1),			static_cast<GLfloat>(WING_THICK),   static_cast<GLfloat>(WING_LEN) }

{
	wingPoint = vec3(WING_POINT_X, WING_POINT_Y, WING_POINT_Z);
	makeBody();
	thrustPtr = ptr;
	thrustDiscOffset = 0;
	active = TRUE; 
}


xwing::~xwing()
{
	delete s1Ptr;
	delete s2Ptr;
	delete s3Ptr;
	delete s4Ptr;
	delete s5Ptr;
	delete s6Ptr;
	delete wingPtr;
	delete flamePtr;

}


void xwing::render()
{
	// return immediately if not active
	if (!active) return;

	if (showWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* Wireframe mode */
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); /* fill mode */
	}

	if (showTex)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, TexHandles[12]);

	setMaterial(9); 

	// render body
	glPushMatrix();

		glScalef(XWING_SCALE, XWING_SCALE, XWING_SCALE);
		s1Ptr->renderWithNormals();
	
		glTranslatef(0, 0, s0[3]);
		s2Ptr->renderWithNormals();
	
		glTranslatef(0, 0, s1[3]);
		s3Ptr->renderWithNormals();
		// render cockpit
		glPushMatrix();
			glTranslatef(0, s2[2]/2, s2[3] / 2);
			glBindTexture(GL_TEXTURE_2D, TexHandles[13]);
			s5Ptr->renderWithNormals();
			glTranslatef(0, 0, s5[3]);
			s6Ptr->renderWithNormals();
			glTranslatef(0, 0, s6[3]);
			glBindTexture(GL_TEXTURE_2D, TexHandles[12]);
			s7Ptr->renderWithNormals();
		glPopMatrix();

		glTranslatef(0, 0, s2[3]);
		s4Ptr->renderWithNormals();
	
		
		// Draw wings
		glPushMatrix();
			glTranslatef(WING_POINT_X, WING_POINT_Y, WING_POINT_Z);
			glRotatef(90, 0, 1, 0);
			glRotatef(-XWING_WING_ANGLE, 1, 0, 0);
			renderWingMod();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(WING_POINT_X, -WING_POINT_Y, WING_POINT_Z);
			glRotatef(90, 0, 1, 0);
			glRotatef(XWING_WING_ANGLE, 1, 0, 0);
			renderWingMod();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-WING_POINT_X, WING_POINT_Y, WING_POINT_Z);
			glRotatef(-90, 0, 1, 0);
			glRotatef(180, 0, 0, 1);
			glRotatef(XWING_WING_ANGLE, 1, 0, 0);
			renderWingMod();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-WING_POINT_X, -WING_POINT_Y, WING_POINT_Z);
			glRotatef(-90, 0, 1, 0);
			glRotatef(180, 0, 0, 1);
			glRotatef(-XWING_WING_ANGLE, 1, 0, 0);
			renderWingMod();
		glPopMatrix();

		//thrustPtr->setSize(50, 50);
		//thrustPtr->render();

		// Draw thrust flames
		renderFlame(vec3(-WING_POINT_X, WING_POINT_Y, WING_POINT_Z));
		renderFlame(vec3(-WING_POINT_X, -WING_POINT_Y, WING_POINT_Z));
		renderFlame(vec3(WING_POINT_X , WING_POINT_Y, WING_POINT_Z));
		renderFlame(vec3(WING_POINT_X, -WING_POINT_Y, WING_POINT_Z));
		
	
	glPopMatrix();


}


void xwing::renderFlame(const vec3& pos)
{
	if (camSpeed > 0.01)
	{
		glPushMatrix();
			//raster to proper position
			glTranslatef(pos.x, pos.y, pos.z);
			glTranslatef(0, 0, JET_LEN);
			// draw the flame
			
			
			glDisable(GL_LIGHTING);
			glDepthMask(GL_FALSE);
			setMaterial(7);

			glPushMatrix();
				flameLen = 3 +  camSpeed * 15; 
				thrustPtr->setSize(10 - flameLen, flameLen);
				thrustPtr->render();
			glPopMatrix();
			//glScalef(1, flameLen, 1);
			//primitive::drawConeWithEnds(JET_RAD, JET_RAD * 1.1f, TexHandles[11], GLU_SMOOTH);
			glScalef(2.5, 2.5, 1);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, 0, GL_ONE);
			glBlendFuncSeparate(GL_ONE, GL_ONE, 0, GL_ONE);
			glBindTexture(GL_TEXTURE_2D, TexHandles[14]);
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			GLfloat discInterval = camSpeed * 0.9; 
			GLfloat maxThrustLen = camSpeed * 5;
			GLfloat finalInterval = 0; 
			GLfloat scaleFactor = 1;
			GLfloat distCounter = 0;
			glTranslatef(0, 0, 0.1);

			// render discs 
			flamePtr->renderWithNormals(1.0, 1.0);

			//for (int i = 0; i < XWING_THRUST_DISC_NUM; i++)
			for (int i = 0; distCounter < 36; i++)
			{
				flamePtr->renderWithNormals(1.0, 1.0);
				discInterval *= 1.2; 
				finalInterval = fmodf(discInterval + thrustDiscOffset, maxThrustLen);
				glTranslatef(0, 0, finalInterval);
				distCounter += finalInterval;
				scaleFactor = 1 / (distCounter * 0.05 + 1);
				glScalef(scaleFactor, scaleFactor, 1);
			}

			//increase offset step
			thrustDiscOffset += 0.01;

			setMaterial(9);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			if (eLight) glEnable(GL_LIGHTING);
			glDisable(GL_BLEND);
		glPopMatrix();
	}
}


void xwing::renderWingMod()
{
	QuadPrismClass;
	//glEnable(GL_BLEND);

	//draw engine cylender
	glPushMatrix();
		glRotatef(90, 0, 0, -1);
		glPushMatrix();
		glScalef(1, ENGINE_LEN, 1);
		primitive::drawConeWithEnds(ENGINE_RAD, ENGINE_RAD * 0.7f, TexHandles[11], GLU_SMOOTH);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, ENGINE_LEN, 0);
		glScalef(1, ENGINE_LEN * 0.2, 1);
		primitive::drawConeWithEnds(ENGINE_RAD * 0.5, ENGINE_RAD * 0.1f, TexHandles[11], GLU_SMOOTH);
	glPopMatrix();

	// the last part of engine
	glTranslatef(0, -JET_LEN, 0);
	glPushMatrix();
		glScalef(1, JET_LEN, 1);
		primitive::drawConeWithEnds(JET_RAD, JET_RAD * 1.1f, TexHandles[11], GLU_SMOOTH);
	glPopMatrix();


	glPopMatrix();

	//draw wing
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, TexHandles[12]);
	glTranslatef(WING_W0 / 2, 0, ENGINE_RAD / 2);
	wingPtr->renderWithNormals();


	glTranslatef(-WING_W0 / 3, 0, WING_LEN);

	//draw the gun
	glRotatef(90, 0, 0, -1);
	glScalef(1, GUN_LEN, 1);
	primitive::drawConeWithEnds(GUN_RAD, GUN_RAD * 0.7f, TexHandles[11], GLU_SMOOTH);

	glPopMatrix();

}

QuadPrismClass* xwing::makePrism(GLfloat* front, GLfloat* back)
{
	// construct points from width, length and height
	vec3 p0, p1, p2, p3, p4, p5, p6, p7;

	p0 = vec3(-0.5 * front[0], -0.5*front[2], 0);
	p1 = vec3(0.5 * front[0], -0.5*front[2], 0);
	p2 = vec3(0.5 * front[1], 0.5*front[2], 0);
	p3 = vec3(-0.5 * front[1], 0.5*front[2], 0);

	p4 = vec3(-0.5 * back[0], -0.5*back[2], front[3]);
	p5 = vec3(0.5 * back[0], -0.5*back[2], front[3]);
	p6 = vec3(0.5 * back[1], 0.5*back[2], front[3]);
	p7 = vec3(-0.5 * back[1], 0.5*back[2], front[3]);

	// Construct prism with points
	QuadPrismClass * ptr = new QuadPrismClass(p0, p1, p2, p3, p4, p5, p6, p7);
	return ptr; 
}

QuadPrismClass* xwing::makeSingleSidePrism(GLfloat* front, GLfloat* back)
{
	// construct points from width, length and height
	vec3 p0, p1, p2, p3, p4, p5, p6, p7;

	p0 = vec3(-0.5 * front[0], 0, 0);
	p1 = vec3(0.5 * front[0], 0, 0);
	p2 = vec3(0.5 * front[1],  0.5*front[2], 0);
	p3 = vec3(-0.5 * front[1], 0.5*front[2], 0);

	p4 = vec3(-0.5 * back[0], 0, front[3]);
	p5 = vec3(0.5 * back[0], 0, front[3]);
	p6 = vec3(0.5 * back[1], 0.5*back[2], front[3]);
	p7 = vec3(-0.5 * back[1], 0.5*back[2], front[3]);

	// Construct prism with points
	QuadPrismClass * ptr = new QuadPrismClass(p0, p1, p2, p3, p4, p5, p6, p7);
	return ptr; 
}


void xwing::makeBody()
{
	// section 1, 2, 3, 4
	s1Ptr = makePrism(s0, s1); 
	s2Ptr = makePrism(s1, s2);
	s3Ptr = makePrism(s2, s3);
	s4Ptr = makePrism(s3, s4);

	// cockpit
	s5Ptr = makeSingleSidePrism(s5, s6);
	s6Ptr = makeSingleSidePrism(s6, s7);
	s7Ptr = makeSingleSidePrism(s7, s8);

	flamePtr = new QuadClass(); 

	wingPtr = makePrism(ws0, ws1);

}
