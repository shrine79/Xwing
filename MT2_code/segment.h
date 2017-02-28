#ifndef __segment_ZZ__
#define __segment_ZZ__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"
class segment
{
public:
	segment();

	void setup(GLfloat length, GLfloat width, GLfloat height, 
		       GLfloat thinkness, GLfloat * colPost, GLfloat * colPlank);

	int render();

	GLfloat length_v, width_v, height_v, thinkness_v;

	~segment();

protected: 
	
	const GLfloat * colorPost; 
	const GLfloat * colorPlank; 

	void drawPost();
};

#endif