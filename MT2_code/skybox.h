#ifndef _SKY_
#define _SKY_

#include "main.h"
#include "quaternion.h"
#include "billboard.h"
#include "lighting.h"
#include "glut.h"
#define SKY_LEN 500



class skybox
{
protected:
	GLfloat length; 
	GLboolean isDay; 
	vec3 sunPos; 
	vec3 sunAxis, sunOrigin;
	billboard * sunPtr; 
	GLuint skyHandle, sunHandle, moonHandle; 
	GLfloat oldTime, curTime; 
	GLfloat spinSpd; 

public:
	void rotate(GLfloat degree); 
	void render();
	void renderSun(void); 
	vec3 getSunPos(void); 
	inline bool isNowDay(void); 
	skybox(GLuint sky_h, GLuint sun_h, GLuint moon_h, vec3 axis, vec3 origin);
	~skybox();
};

inline bool skybox::isNowDay(void)
{
	return isDay;
}

#endif

