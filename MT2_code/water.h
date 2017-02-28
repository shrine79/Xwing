#ifndef _water_
#define _water_
#include <stdlib.h>
#include "vec3.h"
#include "terran.h"
#include "glut.h"
#include "utility.h"

#define WATER_WAVE_RADIUS 50
#define WATER_WAVE_RANGE WATER_WAVE_RADIUS * 2

class water
{
public:
	water();
	~water();

	void setUp(int xstepsZ, int zstepsZ, GLfloat theta, GLfloat lamda, GLfloat frequency, 
		       GLfloat time, GLfloat magnitude, terrain * ptr);
	void update(GLfloat time); 
	int render(); 
	int renderSmooth(); 
	int renderLines(); 
	int offsetX, offsetZ;

protected:

	int xsteps;
	int zsteps;
	GLfloat xdelta;
	GLfloat zdelta;
	vec3 * meshPtr; 
	vec3 * normalMeshPtr; 
	terrain * tPtr; 
	sineTab * sinPtr; 

	////////////////////////////////////////////////////////////////////
	//     Waveform function                                          //
	// y = Mag * sin(x * 2 * Pi/Lamda - 2*Pi*Frequency*time + Theta)  //
	////////////////////////////////////////////////////////////////////
	GLfloat Theta; //the initial phase
	GLfloat Lamda; //the wavelength
	GLfloat Freq;  //the frequency
	GLfloat FreqX, FreqZ; // pre-calculated frequency for waves along x and z axis prespectively to speed up. 
	GLfloat Time;  
	GLfloat Mag;   // Magnitude

	GLfloat GetYfromX(GLfloat x); 
	GLfloat GetYfromZ(GLfloat z);
	GLfloat GetYfromXLarge(GLfloat x);
	GLfloat GetYfromZSmall(GLfloat x, GLfloat z);
	inline GLfloat GetY(GLfloat x, GLfloat z);
};

#endif

