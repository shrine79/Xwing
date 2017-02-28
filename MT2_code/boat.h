#ifndef __boat__
#define __boat__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"
#include "tringularPrism.h"
#define NUM_SECTION 24

static GLfloat frontMastHeigth = 2;

class section
{
	// a compartment of ship consists of 
	//				 w0
	//   -----------------------------
	//    \ |         w1          | / h0
	//	   \|---------------------|/
	//      \  |      w2       |  /   h1
	//       \ |---------------| /
	//                                h2
public:
	GLfloat w0, h0, w1, h1, w2, h2, w3, h3, w4, h4, len; 
	section(); 
	section(GLfloat W0, GLfloat H0, GLfloat W1, GLfloat H1, GLfloat W2, GLfloat H2, GLfloat W3, GLfloat H3, GLfloat W4, GLfloat H4, GLfloat L);
};

class compartment
{
public:
	QuadPrismClass *quadTop = NULL;
	QuadPrismClass *quadBot = NULL;
	QuadPrismClass *quadUpper = NULL;
	TriPrismClass2  *bottom = NULL;

	compartment();
	compartment(section back, section front);
	~compartment();
	void renderWithNormals();
	void renderTheNormals();


};

class boat
{
public:
	

	boat();
	boat(GLfloat Width, GLfloat Height);
	~boat();

	void makeBoat(); 
	void renderWithNormals();
	void renderTheNormals();
	GLfloat getCentreOffsetZ();
protected: 
	GLfloat maxHeight, maxWidth; // max width and heigth of boat hull
	section Sections[NUM_SECTION];
	compartment * PartArr[NUM_SECTION - 1];
	void drawSail(GLfloat width, GLfloat height, GLuint handle);
	void drawSail(GLfloat width, GLfloat height, GLuint handle1, GLuint handle2);
	void drawMast(GLfloat height, GLfloat deckHeight, GLuint handle);
};



#endif

