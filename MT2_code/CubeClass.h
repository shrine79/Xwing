#ifndef cubeclass_def 
#define cubeclass_def 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"

#include "utility.h"
#include "glUtils.h"

#define TESS_STEP 0.1

class CubeClass
{
public:
	CubeClass(); // constructor
	void render(); // draw it
	void renderCW(); // draw it CW
	void renderCCW(); // draw it CCW
	void renderWithNormals();
	void renderWithNormals(GLfloat texScale);
	void renderTheNormals();
protected:
	void renderFlat(GLfloat texScale);
	void renderSmooth(GLfloat texScale);
	void renderTessellation(GLfloat texScale);
};

#endif
