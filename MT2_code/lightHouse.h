#ifndef _light_house_
#define _light_house_

#include "vec3.h"
#include "lighting.h"
#include "glut.h"

#define LIGHT_HOUSE_HEIGHT 3

class lightHouse
{
public:
	lightHouse();
	~lightHouse();

	int render(); 
	void update();

	void setup(GLfloat rad, GLfloat h, GLuint angle, vec3 dir, GLfloat spd);
	void setupBeam(GLuint angle, vec3 dir, GLfloat spd);

protected:
	GLfloat radius, height, spinSpd, oldTime; 
	GLuint cutOffAngle;
	vec3 beamDir;
	void rotateBeam(); 
	
};

#endif // !_light_house_
