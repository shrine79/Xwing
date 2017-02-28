#ifndef _lighting_
#define _lighting_

#include "main.h"
#include "glut.h"

extern int lpos;
extern bool eLight;
extern bool eLight0;
extern bool eLight1;
extern bool ambientLight;
extern float lpos2[];


//void enableLighting(bool day);
void setupLights(float * lpos, bool day);
void setupLights(void);
void setMaterial(int m);
void setSunMat(void);

class materialClass
{
public:

	float	dif[4];  //  = {0.78, 0.57, 0.11, 1.0}; // brass
	float	amb[4];  //  = {0.33, 0.22, 0.027, 1.0};
	float	spe[4];  //  = {0.99, 0.94, 0.8, 1.0};
	float	eme[4];  //  = {0.78, 0.57, 0.11, 1.0}; 

	float shiny;

	inline void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	inline void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	inline void setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	inline void setEmissive(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	inline void setShinyInt(int shinyZ);
	inline void setShinyFloat(GLfloat shinyZ);

	virtual void doMaterial();
	materialClass();

	inline void setMaterialJade();
	inline void setMaterialRedPlastic();
	inline void setMaterialWhiteRubber();
	inline void setMaterialBlackRubber();
	inline void setMaterialPearl();
	inline void setMaterialWhiteStuff();  // mainly for textures
	inline void setMaterialWhiteStuff2(); // mainly for textures
	inline void setMaterialYellowSand();
	inline void setMaterialGreenGrass();
	inline void setMaterialBlue();
	friend materialClass& mixMat(materialClass& rvl, const materialClass& mat0, const materialClass& mat1, GLfloat ratio); // material interpolation
};

inline void lerp4( float(&ret)[4], const float (&v0)[4], const float(&v1)[4], float t )
{
	ret[0] = v0[0] * t + (1 - t)*v1[0];
	ret[1] = v0[1] * t + (1 - t)*v1[1];
	ret[2] = v0[2] * t + (1 - t)*v1[2];
	ret[3] = v0[3] * t + (1 - t)*v1[3];
}

inline void lerp1(float& ret, const float& v0, const float& v1, float t)
{
	ret = v0 * t + v1 * (1 - t);
}

void materialClass::setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	dif[0] = r;
	dif[1] = g;
	dif[2] = b;
	dif[3] = a;
}

void materialClass::setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	amb[0] = r;
	amb[1] = g;
	amb[2] = b;
	amb[3] = a;
}

void materialClass::setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	spe[0] = r;
	spe[1] = g;
	spe[2] = b;
	spe[3] = a;
}

void materialClass::setEmissive(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	eme[0] = r;
	eme[1] = g;
	eme[2] = b;
	eme[3] = a;
}

void materialClass::setMaterialYellowSand()
{
	setAmbient(0.4f, 0.4f, 0.1575f, 1);
	setDiffuse(0.7f, 0.7f, 0.2f, 1);
	setSpecular(0.316228f, 0.316228f, 0.216228f, 1);
	setEmissive(0, 0, 0, 0);
	setShinyInt((int)(0.1*128.0));
}

void materialClass::setShinyInt(int shinyZ)
{
	shiny = (float)shinyZ;
}

void materialClass::setShinyFloat(GLfloat shinyZ)
{
	shiny = shinyZ;
}

void materialClass::setMaterialJade()
{
	setAmbient(0.135f, 0.2225f, 0.1575f, 1);
	setDiffuse(0.54f, 0.89f, 0.63f, 1);
	setSpecular(0.316228f, 0.316228f, 0.316228f, 1);
	setEmissive(0, 0, 0, 0);
	setShinyInt((int)(0.1*128.0));
}

void materialClass::setMaterialRedPlastic()
{
	setAmbient(0.0, 0.0, 0.0, 1);
	setDiffuse(0.5f, 0, 0, 1);
	setSpecular(0.7f, 0.6f, 0.6f, 1);
	setEmissive(0, 0, 0, 0);
	setShinyInt((int)(0.25*128.0));
}

void materialClass::setMaterialWhiteRubber()
{
	setAmbient(0.05f, 0.05f, 0.05f, 1);
	setDiffuse(0.5f, 0.5f, 0.5f, 1);
	setSpecular(0.7f, 0.7f, 0.7f, 1);
	setEmissive(0, 0, 0, 0);
	setShinyInt((int)(0.078125*128.0));
}

void materialClass::setMaterialBlackRubber()
{
	setAmbient(0.02f, 0.02f, 0.02f, 1);
	setDiffuse(0.01f, 0.01f, 0.01f, 1);
	setSpecular(0.4f, 0.4f, 0.4f, 1);
	setEmissive(0, 0, 0, 0);
	setShinyInt((int)(0.078125*128.0));
}

//pearl	0.25	0.20725	0.20725	1	0.829	0.829	0.296648	0.296648	0.296648	0.088
// from http://devernay.free.fr/cours/opengl/materials.html
void materialClass::setMaterialPearl()
{
	setAmbient(0.25f, 0.207f, 0.207f, 1);
	setDiffuse(1.0f, 0.829f, 0.829f, 1);
	setSpecular(0.297f, 0.297f, 0.297f, 1);
	setEmissive(0, 0, 0, 0);
	setShinyInt((int)(0.088*128.0));
}

void materialClass::setMaterialWhiteStuff2()
{
	setAmbient(0.25f, 0.207f, 0.207f, 1);
	setDiffuse(1.0f, 0.829f, 0.829f, 1);
	setSpecular(0.597f, 0.597f, 0.597f, 1); // shinier than whitestuff
	setEmissive(0, 0, 0, 0);
	setShinyInt((int)(0.108*128.0));
}



void materialClass::setMaterialGreenGrass()
{
	setAmbient(0.13f, 0.25f, 0.15f, 1);
	setDiffuse(0.24f, 0.89f, 0.33f, 1);
	setSpecular(0.116228f, 0.316228f, 0.116228f, 1);
	setEmissive(0, 0, 0, 0);
	setShinyInt((int)(0.1*128.0));
}

void materialClass::setMaterialBlue()
{
	setAmbient(0.1f, 0.1f, 0.2f, 1);
	setDiffuse(0.3f, 0.3f, 0.8f, 1);
	setSpecular(0.316228f, 0.316228f, 0.316228f, 1);
	setEmissive(0, 0, 0, 0);
	setShinyInt((int)(0.1*128.0));
}

void materialClass::setMaterialWhiteStuff() // suitable for lighting a texture
{
	setAmbient(0.7f, 0.7f, 0.7f, 1);
	setDiffuse(0.9f, 0.9f, 0.9f, 1);
	setSpecular(0.6f, 0.6f, 0.6f, 1);
	setEmissive(0, 0, 0, 0);
	setShinyInt((int)(12));
}
#endif