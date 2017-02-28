#include "boat.h"
#include "primitive.h"
#include "lighting.h"
#include <math.h>
#include "main.h"


section::section()
{
}

section::section(GLfloat W0, GLfloat H0, GLfloat W1, GLfloat H1, GLfloat W2, GLfloat H2, GLfloat W3, GLfloat H3, GLfloat W4, GLfloat H4, GLfloat L)
{
	w0 = W0; 
	h0 = H0;
	w1 = W1;
	h1 = H1;
	w2 = W2;
	h2 = H2;
	w3 = W3; 
	h3 = H3; 
	w4 = W4;
	h4 = H4;
	len = L;
}

compartment::compartment()
{

}

compartment::compartment(section back, section front)
{
	
	// points on the deck level
	vec3 p0(-(front.w0 + front.w4) / 2, front.h4, -back.len);
	vec3 p1((front.w0 + front.w4) / 2, front.h4, -back.len);
	vec3 p2((back.w0 + back.w4) / 2, back.h4, 0);
	vec3 p3(-(back.w0 + back.w4) / 2, back.h4, 0);

	// points on the lower deck level
	vec3 p4(-front.w1 / 2, front.h4 -front.h0, -back.len);
	vec3 p5(front.w1 / 2, front.h4 -front.h0, -back.len);
	vec3 p6(back.w1 / 2, back.h4 -back.h0, 0);
	vec3 p7(-back.w1 / 2, back.h4 -back.h0, 0);

	// points lowest deck level
	vec3 p8(-front.w2 / 2, front.h4 -(front.h0 + front.h1), -back.len);
	vec3 p9(front.w2 / 2, front.h4 -(front.h0 + front.h1), -back.len);
	vec3 p10(back.w2 / 2, back.h4 -(back.h0 + back.h1), 0);
	vec3 p11(-back.w2 / 2, back.h4 -(back.h0 + back.h1), 0);

	// points on the upper deck level
	vec3 p12(-front.w3  / 2, front.h3, -back.len);
	vec3 p13(front.w3 / 2, front.h3, -back.len);
	vec3 p14(back.w3 / 2, back.h3, 0);
	vec3 p15(-back.w3 / 2, back.h3, 0);

	
	// points of the bottom, calculated by points on the lowest deck
	vec3 p16 = (p8 + p9) / 2;
	p16.y -= front.h2; 
	vec3 p17 = (p10 + p11) / 2; 
	p17.y -= back.h2;
	
	quadTop = new QuadPrismClass(p0, p1, p2, p3, p4, p5, p6, p7); 
    quadBot = new QuadPrismClass(p4, p5, p6, p7, p8, p9, p10, p11);
	quadUpper = new QuadPrismClass(p12, p13, p14, p15, p0, p1, p2, p3);
	bottom = new TriPrismClass2(p11, p10, p17, p8, p9, p16); 
}

compartment::~compartment()
{
	if(quadTop != NULL)
		delete quadTop;
	if (quadBot != NULL)
		delete quadBot;

	//delete bottom;
}

void compartment::renderWithNormals()
{
	glBindTexture(GL_TEXTURE_2D, TexHandles[0]);
	

	quadTop->renderWithNormals();
	quadBot->renderWithNormals();
	quadUpper->renderWithNormals();
	bottom->renderWithNormals();
}

void compartment::renderTheNormals()
{
	quadTop->renderTheNormals();
	quadBot->renderTheNormals();
	quadUpper->renderTheNormals();
	bottom->renderTheNormals();
}





boat::boat()
{
	maxWidth = 10;
	maxHeight = 3;
	frontMastHeigth = 1; 
	makeBoat();
}

boat::boat(GLfloat Width, GLfloat Height)
{
	maxWidth = Width; 
	maxHeight = Height; 

	makeBoat(); 
}


boat::~boat()
{
	for (int i = 0; i < sizeof(Sections) / sizeof(section); i++)
	{
		delete PartArr[i];
		PartArr[i] = NULL;
	}
	
}

void boat::makeBoat()
{
	// Populate the sections
	//..................................W...............H....................W1................H1................W2..............H2..............W3........................H3................W4...............H4...........Len..//
	Sections[24] = section::section(maxWidth * 0.75, maxHeight *  1.00, maxWidth * 0.70, maxHeight * 0.10, maxWidth * 0.70, maxHeight * 0.05, maxWidth * 0.8 * 0.75,  maxHeight * 1.45, maxWidth * 0.00, maxHeight * 0.00, 1.0);
	Sections[23] = section::section(maxWidth * 0.82, maxHeight *  1.00, maxWidth * 0.76, maxHeight * 0.16, maxWidth * 0.70, maxHeight * 0.10, maxWidth * 0.8 * 0.82,  maxHeight * 1.39, maxWidth * 0.00, maxHeight * 0.00, 1.0);
	Sections[22] = section::section(maxWidth * 0.88, maxHeight *  1.00, maxWidth * 0.79, maxHeight * 0.20, maxWidth * 0.70, maxHeight * 0.10, maxWidth * 0.8 * 0.88,  maxHeight * 1.34, maxWidth * 0.00, maxHeight * 0.00, 1.0);
	Sections[21] = section::section(maxWidth * 0.93, maxHeight *  1.00, maxWidth * 0.82, maxHeight * 0.53, maxWidth * 0.70, maxHeight * 0.40, maxWidth * 0.8 * 0.93,  maxHeight * 1.29, maxWidth * 0.00, maxHeight * 0.00, 1.0);
	Sections[20] = section::section(maxWidth * 0.98, maxHeight *  1.00, maxWidth * 0.85, maxHeight * 0.56, maxWidth * 0.70, maxHeight * 0.50, maxWidth * 0.8 * 0.98,  maxHeight * 1.25, maxWidth * 0.00, maxHeight * 0.00, 1.0);
	Sections[19] = section::section(maxWidth * 1.02, maxHeight *  1.00, maxWidth * 0.88, maxHeight * 0.58, maxWidth * 0.70, maxHeight * 0.57, maxWidth * 0.8 * 1.02,  maxHeight * 1.22, maxWidth * 0.00, maxHeight * 0.00, 1.0);
	Sections[18] = section::section(maxWidth * 1.05, maxHeight *  1.00, maxWidth * 0.90, maxHeight * 0.60, maxWidth * 0.70, maxHeight * 0.60, maxWidth * 0.8 * 1.05,  maxHeight * 1.20, maxWidth * 0.00, maxHeight * 0.00, 5.0);
	Sections[17] = section::section(maxWidth * 1.15, maxHeight *  1.00, maxWidth * 0.95, maxHeight * 0.75, maxWidth * 0.70, maxHeight * 0.80, maxWidth * 0.8 * 1.15,  maxHeight * 1.10, maxWidth * 0.00, maxHeight * 0.00, 5.0);
	Sections[16] = section::section(maxWidth * 1.20, maxHeight *  1.00, maxWidth * 1.10, maxHeight * 0.85, maxWidth * 0.70, maxHeight * 0.80, maxWidth * 0.8 * 1.20,  maxHeight * 1.00, maxWidth * 0.00, maxHeight * 0.00, 5.0);
	Sections[15] = section::section(maxWidth * 1.20, maxHeight *  1.00, maxWidth * 1.10, maxHeight * 0.90, maxWidth * 0.70, maxHeight * 0.80, maxWidth * 0.8 * 1.20,  maxHeight * 0.00, maxWidth * 0.00, maxHeight * 0.00, 5.0);
	Sections[14] = section::section(maxWidth * 1.20, maxHeight *  1.00, maxWidth * 1.10, maxHeight * 0.95, maxWidth * 0.80, maxHeight * 0.80, maxWidth * 0.8 * 1.20,  maxHeight * 0.00, maxWidth * 0.00, maxHeight * 0.00, 4.0);
	Sections[13] = section::section(maxWidth * 1.20, maxHeight *  1.00, maxWidth * 1.10, maxHeight * 0.95, maxWidth * 0.80, maxHeight * 0.80, maxWidth * 0.8 * 1.20,  maxHeight * 0.00, maxWidth * 0.00, maxHeight * 0.00, 4.0);
	
	Sections[12] = section::section(maxWidth * 1.20, maxHeight *  1.00, maxWidth * 1.10, maxHeight * 0.95, maxWidth * 0.80, maxHeight * 0.80, maxWidth * 0.8 * 1.20,  maxHeight * 0.60,  maxWidth * 0.00, maxHeight * 0.00, 4.0);
	Sections[11] = section::section(maxWidth * 1.15, maxHeight *  1.00, maxWidth * 1.05, maxHeight * 0.95, maxWidth * 0.75, maxHeight * 0.75, maxWidth * 0.8 * 1.15,  maxHeight * 0.65,  maxWidth * 0.00, maxHeight * 0.00, 4.0);
	Sections[10] = section::section(maxWidth * 1.10, maxHeight *  1.00, maxWidth * 1.00, maxHeight * 0.95, maxWidth * 0.70, maxHeight * 0.65, maxWidth * 0.8 * 1.10,  maxHeight * 0.70, maxWidth * 0.00, maxHeight * 0.00, 4.0);
	Sections[9]  = section::section(maxWidth * 1.00, maxHeight *  1.00, maxWidth * 0.90, maxHeight * 0.90, maxWidth * 0.60, maxHeight * 0.50, maxWidth * 0.8 * 1.00,  maxHeight * 0.78,  maxWidth * 0.00, maxHeight * 0.00, 2.0);
	Sections[8]  = section::section(maxWidth * 0.85, maxHeight *  1.00, maxWidth * 0.75, maxHeight * 0.80, maxWidth * 0.50, maxHeight * 0.40, maxWidth * 0.8 * 0.85,  maxHeight * 0.85, maxWidth * 0.00, maxHeight * 0.00, 1.5);
	Sections[7]  = section::section(maxWidth * 0.70, maxHeight *  1.00, maxWidth * 0.60, maxHeight * 0.50, maxWidth * 0.35, maxHeight * 0.25, maxWidth * 0.8 * 0.70,  maxHeight * 0.95, maxWidth * 0.00, maxHeight * 0.00, 1.0);
	Sections[6]  = section::section(maxWidth * 0.50, maxHeight *  0.90, maxWidth * 0.20, maxHeight * 0.20, maxWidth * 0.15, maxHeight * 0.20, maxWidth * 0.8 * 0.50,  maxHeight * 1.10, maxWidth * 0.00, maxHeight * 0.00, 0.7);
	Sections[5]  = section::section(maxWidth * 0.30, maxHeight *  0.70, maxWidth * 0.10, maxHeight * 0.00, maxWidth * 0.10, maxHeight * 0.20, maxWidth * 0.8 * 0.30,  maxHeight * 1.30, maxWidth * 0.00, maxHeight * 0.00, 0.7);
	Sections[4]  = section::section(maxWidth * 0.20, maxHeight *  0.15, maxWidth * 0.10, maxHeight * 0.00, maxWidth * 0.00, maxHeight * 0.00, maxWidth * 0.9 * 0.20,  maxHeight * 1.45, maxWidth * 0.00, maxHeight * 0.00, 0.7);
	Sections[3]  = section::section(maxWidth * 0.00, maxHeight *  0.00, maxWidth * 0.00, maxHeight * 0.00, maxWidth * 0.00, maxHeight * 0.00, maxWidth * 1.0 * 0.13,  maxHeight * 1.54, maxWidth * 0.15, maxHeight * 0.30, 0.5);
	Sections[2]  = section::section(maxWidth * 0.00, maxHeight *  0.00, maxWidth * 0.00, maxHeight * 0.00, maxWidth * 0.00, maxHeight * 0.00, maxWidth * 1.0 * 0.12,  maxHeight * 1.60, maxWidth * 0.10, maxHeight * 0.50, 0.5);
	Sections[1]  = section::section(maxWidth * 0.00, maxHeight *  0.00, maxWidth * 0.00, maxHeight * 0.00, maxWidth * 0.00, maxHeight * 0.00, maxWidth * 1.0 * 0.10 , maxHeight * 1.65, maxWidth * 0.08, maxHeight * 0.65, 3.0);
	Sections[0]  = section::section(maxWidth * 0.00, maxHeight *  0.00, maxWidth * 0.00, maxHeight * 0.00, maxWidth * 0.00, maxHeight * 0.00, maxWidth * 1.0 * 0.10,  maxHeight * 1.90, maxWidth * 0.05, maxHeight * 1.30, 3.0);


	// make compartment

	//compartment temp = compartment(Sections[1], Sections[0], 2);
	for (int i = 0; i < sizeof(Sections)/sizeof(section) - 1; i++)
	{
		PartArr[i] = new compartment(Sections[i+1], Sections[i]);
	}

	

}

void boat::drawSail(GLfloat width, GLfloat height, GLuint handle)
{
	if (showWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE); 

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT);
	setMaterial(2);
	glColor3fv(white);
	glRotatef(-90, 0.0, 0.0, 1.0);
	glScalef(height, width, height * 0.8);
	glRotatef(110, 0.0, 1.0, 0.0);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	primitive::drawCurve(18, 8, handle);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glPopAttrib();
	glPopMatrix();

	if (hiddenCull)
		glEnable(GL_CULL_FACE); 
}

void boat::drawSail(GLfloat width, GLfloat height, GLuint handle1, GLuint handle2)
{
	if (showWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);
	setMaterial(2);
	glColor3fv(white);
	glRotatef(-90, 0.0, 0.0, 1.0);
	glScalef(height, width, height * 0.8);
	glRotatef(110, 0.0, 1.0, 0.0);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	primitive::drawCurveWith2Tex(18, 8, handle1, handle2);
	//primitive::drawCurve(18, 8, handle2);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glPopAttrib();
	glPopMatrix();

	if (hiddenCull)
		glEnable(GL_CULL_FACE);
}

void boat::drawMast(GLfloat height, GLfloat deckHeight, GLuint handle)
{
	if (showWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);


	glColor3fv(red);
	glTranslatef(0, deckHeight, 0);
	glScalef(1, height, 1);
	primitive::drawConeWithEnds(0.4, 0.2, handle, GLU_FLAT);

	glPopAttrib();
	glPopMatrix();
}


void boat::renderWithNormals()
{
	if (showWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (showTex)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);


	glPushMatrix();

	// Make the front masts
	glPushAttrib(GL_CURRENT_BIT);
	glColor3fv(red);
	glTranslatef(0, Sections[1].h3, 0);
	GLfloat tempAngle = atan2f(Sections[0].h3 - Sections[1].h3, Sections[1].len) * 180.0 / M_PI;
	glRotatef(tempAngle - 90, 1, 0, 0 );
	glScalef(1, 20, 1);
	primitive::drawConeWithEnds(0.4, 0.2, TexHandles[2], GLU_FLAT);
	glPopAttrib();
	
	glPopMatrix();

	glPushAttrib(GL_CURRENT_BIT);
	glColor3fv(yellow);

	glPushMatrix();
	for (int i = 0; i < sizeof(Sections) / sizeof(section) - 1; i++)
	{
		PartArr[i]->renderWithNormals(); 
		if(showNormals)
			PartArr[i]->renderTheNormals();
		
		switch (i)
		{
			case (10) :
			{
				////////////// Draw first mast ////////////////////
				drawMast(25, Sections[i].h3, TexHandles[2]);

				glPushMatrix();
				///////////Draw first mast bottom sail //////////////
				glTranslatef(0, 12, 0);
				//drawSail(14, 10, TexHandles[3]);
				drawSail(14, 10, TexHandles[3], TexHandles[4]);

				////////// Draw first mast top sail ///////////
				glTranslatef(0, 10, 0);
				drawSail(10, 7, TexHandles[3]);

				glPopMatrix();
				


				break;
			}
			case(14) :
			{
				////////////// Draw first mast ////////////////////
				drawMast(28, Sections[i].h3, TexHandles[2]);

				glPushMatrix();
				///////////Draw main mast bottom sail //////////////
				glTranslatef(0, 10, 0);
				drawSail(16, 12, TexHandles[3]);

				////////// Draw main mast top sail ///////////
				glTranslatef(0, 12, 0);
				drawSail(12, 9, TexHandles[3]);

				glPopMatrix();

				break;
			}
			case(16) :
			{
				drawMast(17, Sections[i].h3, TexHandles[2]);

				glPushMatrix();

				////////// Draw main mast top sail ///////////
				glTranslatef(0, 13, 0);
				drawSail(18, 12, TexHandles[3]);

				glPopMatrix();


				break;
			}
		}


		if(i >= sizeof(Sections)/sizeof(section))
			glTranslatef(0.0, 0.0, 0);
		else
			glTranslatef(0.0, 0.0, Sections[i + 2].len); 
	}

	glDisable(GL_TEXTURE_2D);
	glPopAttrib();

	glPopMatrix();
}

GLfloat boat::getCentreOffsetZ()
{
	// return the offset from first compartment to rotation centre(compartment 12th) along Z axle 
	GLfloat retVal = 0.0;
	for (int i = 0; i <= 12; i++)
	{
		retVal += Sections[i].len;
	}
	return retVal;
}


void boat::renderTheNormals()
{

}