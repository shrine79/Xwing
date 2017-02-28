

#include <stdio.h>
#include "main.h"

#include "lighting.h"
#include "lightHouse.h"
#include "primitive.h"




float	dif1[] = { 0.78f, 0.57f, 0.11f, 1.0f }; // brass
float	amb1[] = { 0.33f, 0.22f, 0.027f, 1.0f };
float	spe1[] = { 0.99f, 0.94f, 0.8f, 1.0f };
float   emi1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   shiny1 = 0.218f * 128;

float	dif2[] = { 0.6f, 0.6f, 0.6f, 1.0f }; // canvas
float	amb2[] = { 0.5f, 0.5f, 0.5f, 1.0f };
float	spe2[] = { 0.5f, 0.5f, 0.5f, 1.0f };
float   emi2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   shiny2 = 0.25f;

float	dif3[] = { 0.3f, 0.6f, 0.1f, 1.0f }; // Jade
float	amb3[] = { 0.3f, 0.6f, 0.2f, 1.0f };
float	spe3[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   emi3[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   shiny3 = 1.0f * 128;

float	dif4[] = { 0.3f, 0.2f, 0.05f, 1.0f }; // timber - modified
float	amb4[] = { 0.8f, 0.4f, 0.15f, 1.0f };
float	spe4[] = { 0.1f, 0.1f, 0.05f, 1.0f };
float   emi4[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   shiny4 = 0.01f * 128;

float	dif5[] = { 0.8f, 0.8f, 0.8f, 0.6f }; // water
float	amb5[] = { 0.4f, 0.8f, 1.0f, 0.6f };
float	spe5[] = { 0.9f, 0.7f, 0.9f, 1.0f };
float   emi5[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   shiny5 = 0.08f * 128;

float	dif6[] = { 0.5f, 0.5f, 0.4f, 1.0f }; // sand
float	amb6[] = { 0.05f, 0.05f, 0.5f, 1.0f };
float	spe6[] = { 0.7f, 0.7f, 0.04f, 1.0f };
float   emi6[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   shiny6 = 0.08f * 128;

float	dif7[] = { 0.1f, 0.1f, 0.1f, 0.6f }; // light source
float	amb7[] = { 0.1f, 0.1f, 0.1f, 0.6f };
float	spe7[] = { 0.1f, 0.1f, 0.1f, 1.0f };
float   emi7[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float   shiny7 = 0.68f * 128;

float	dif8[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // skybox
float	amb8[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float	spe8[] = { 0,    0,    0,    1.0f };
float   emi8[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   shiny8 = 0.08f * 128;

float	dif9[] = { 0.50754, 0.50754, 0.50754, 1.0f }; // silver
float	amb9[] = { 0.19225, 0.19225, 0.19225, 1.0f };
float	spe9[] = { 0.508273,0.508273,0.508273,1.0f };
float   emi9[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float   shiny9 = 0.4f * 128;


int lpos = 1;
float	lpos1[] = { 100,  30, 0.0, 0.0 }; // point source close 
float	lpos2[] = { 10,  4.5 + LIGHT_HOUSE_HEIGHT, 0, 1.0f }; // point source 
float	lpos3[] = { 140, 140, 0.0, 0.0 }; // distant light

float	ldif0[] = { 0.6f, 0.3f, 0.3f, 1.0f }; // light at the dawn and twilight
float	lamb0[] = { 0.4f, 0.2f, 0.2f, 1.0f };
float	lspe0[] = { 0.3f, 0.2f, 0.2f, 1.0f };

float	ldif1[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // light at the noon 
float	lamb1[] = { 0.9f, 0.9f, 0.9f, 1.0f };
float	lspe1[] = { 0.5f, 0.5f, 0.4f, 1.0f };

float	ldif2[] = { 0.3f, 0.4f, 0.5f, 1.0f }; // moon light
float	lamb2[] = { 0.01f, 0.05f, 0.05f, 1.0f };
float	lspe2[] = { 0.2f, 0.25f, 0.3f, 1.0f };

float   zero_vec[] = { 0.0f, 0.0f,  0.0f, 1.0f };
float   one_vec[] = { 1.0f, 1.0f, 1.0f, 1.0f };

static GLfloat ambientIntensity01[] = { 0.3f, 0.3f ,0.3f ,1.0f }; // RGBA color for ambient
static GLfloat ambientIntensity0[] = { 0.2f, 0.2f, 0.2f ,1.0f }; // RGBA color for ambient
bool ambientLight = true;
bool eLight = true;
bool eLight0 = true;
bool eLight1 = true;

GLenum __stdcall err1;

////////// Lighting functions ///////////

//void enableLighting(bool day)
//{
//	if (eLight) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);
//	if (eLight0) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
//	if (day)
//	{
//		GLfloat lamb_t[] = { 0, 0, 0, 1 };
//		lerp4(lamb_t, lamb1, lamb0, lpos[1] / 300);
//		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambientIntensity01[0]);
//		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambientIntensity01[0]);
//		err1 = glGetError();
//	}
//	else
//	{
//		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambientIntensity0[0]);
//		err1 = glGetError();
//	}
//	if (err1 != GL_NO_ERROR) printf("err1=%d\n", (int)err1);
//}

void setSunMat(void)
{
	for (int i = 0; i < 3; i++)
		global_amb[i] = global_amb[i] * 2;

	glMaterialfv(GL_FRONT, GL_AMBIENT, zero_vec);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, zero_vec);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zero_vec);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
	glMaterialfv(GL_FRONT, GL_EMISSION, global_amb);
}

void setupLights(void)
{
	setupLights(lpos1, true); 
}

void setupLights(float * lpos, bool day)
{
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	if (day)
	{
		// day light
		GLfloat lamb_t[] = { 0, 0, 0, 1 };
		GLfloat ldif_t[] = { 0, 0, 0, 1 };
		GLfloat lspe_t[] = { 0, 0, 0, 1 };

		float temp = lpos[1] / 300;
		lerp4(ldif_t, ldif1, ldif0, temp);
		lerp4(lspe_t, lspe1, lspe0, temp);

		glLightfv(GL_LIGHT0, GL_AMBIENT, zero_vec);
		glLightfv(GL_LIGHT0, GL_DIFFUSE,  ldif_t);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lspe_t);
	}
	else 
	{
		// moon light
		glLightfv(GL_LIGHT0, GL_AMBIENT, zero_vec);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, ldif2);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lspe2);
	}

	if (eLight) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);
	if (eLight0) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);

	// Set global ambient
	if (day)
	{
		float temp = lpos[1] / 300;
		lerp4(global_amb, lamb1, lamb0, temp);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_amb);
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambientIntensity01[0]);
		err1 = glGetError();
	}
	else
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambientIntensity0[0]);
		err1 = glGetError();
	}
	if (err1 != GL_NO_ERROR) printf("err1=%d\n", (int)err1);

	//glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.3);
	//glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.3);

	

	// Draw a ball to represent the light source 
	//setMaterial(7);
	//glPushMatrix();
	//glTranslatef(lpos1[0], lpos1[1], lpos1[2]);
	//primitive::drawSphere(GL_FLAT);
	//glPopMatrix();
}




void setMaterial(int m)
{
	if (m == 1)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb1);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif1);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spe1);
		glMaterialf(GL_FRONT, GL_SHININESS, shiny1);
		glMaterialfv(GL_FRONT, GL_EMISSION, emi1);
	}
	if (m == 2)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spe2);
		glMaterialf(GL_FRONT, GL_SHININESS, shiny2);
		glMaterialfv(GL_FRONT, GL_EMISSION, emi2);
	}
	if (m == 3)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb3);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif3);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spe3);
		glMaterialf(GL_FRONT, GL_SHININESS, shiny3);
		glMaterialfv(GL_FRONT, GL_EMISSION, emi3);
	}
	if (m == 4)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb4);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif4);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spe4);
		glMaterialf(GL_FRONT, GL_SHININESS, shiny4);
		glMaterialfv(GL_FRONT, GL_EMISSION, emi4);
	}
	if (m == 5)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif5);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spe5);
		glMaterialf(GL_FRONT, GL_SHININESS, shiny5);
		glMaterialfv(GL_FRONT, GL_EMISSION, emi5);
	}
	if (m == 6)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb6);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif6);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spe6);
		glMaterialf(GL_FRONT, GL_SHININESS, shiny6);
		glMaterialfv(GL_FRONT, GL_EMISSION, emi6);
	}
	if (m == 7)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb7);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif7);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spe7);
		glMaterialf(GL_FRONT, GL_SHININESS, shiny7);
		glMaterialfv(GL_FRONT, GL_EMISSION, emi7);
	}
	if (m == 8)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb8);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif8);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spe8);
		glMaterialf(GL_FRONT, GL_SHININESS, shiny8);
		glMaterialfv(GL_FRONT, GL_EMISSION, emi8);
	}
}


///////// End of lighting functions //////


/////////// Definition of Material class //////////////

materialClass::materialClass()
{
	// white rubber aS DEFAULT 
	setMaterialWhiteRubber();
}



void  materialClass::doMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spe);
	glMaterialfv(GL_FRONT, GL_EMISSION, eme);
	glMateriali(GL_FRONT, GL_SHININESS, shiny);
}

materialClass& mixMat(materialClass& rvl, const materialClass& mat0, const materialClass& mat1, GLfloat ratio)// material interpolation
{
	lerp4(rvl.dif, mat0.dif, mat1.dif, ratio);
	lerp4(rvl.amb, mat0.amb, mat1.amb, ratio);
	lerp4(rvl.spe, mat0.spe, mat1.spe, ratio);
	lerp1(rvl.shiny, mat0.shiny, mat1.shiny, ratio);
	// do not mix alpha channel
	return rvl;
}
// end