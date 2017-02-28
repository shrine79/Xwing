#ifndef triprismclass_def 
#define triprismclass_def 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"

#include "utility.h"
#include "glUtils.h"
#include "vec3.h"

class TriClass
{
public:

	vec3 p0;
	vec3 p1;
	vec3 p2;
	vec3 n0;

	vec3 temp1n; // for rendering normals
	vec3 temp2n;

	TriClass(); // constructor
	TriClass(vec3 v0, vec3 v1, vec3 v2 ); // constructor
	void computeNormal();
	void renderWithNormals();
	void renderTheNormals();
};

class QuadClass
{
public:

	vec3 p0;
	vec3 p1;
	vec3 p2;
	vec3 p3;
	vec3 n0;

	vec3 temp1n; // for rendering normals
	vec3 temp2n;

	QuadClass(); // constructor
	QuadClass(vec3 v0, vec3 v1, vec3 v2, vec3 v3 ); // constructor
	void computeNormal();
	void renderWithNormals();
	void renderWithNormals(GLfloat texW, GLfloat texH);
	void renderWithNormalsMultiTex(GLuint counter, GLuint slice);
	void renderWithNormalsSingleTex(GLuint counter, GLuint slice);
	void renderTheNormals();
};

class TriPrismClass
{
public:

	TriClass *tri1;
	TriClass *tri2;
	QuadClass *quad1;
	QuadClass *quad2;
	QuadClass *quad3;

	TriPrismClass(); // constructor
	~TriPrismClass(); // destructor
	TriPrismClass(GLfloat height, GLfloat width, GLfloat length); // constructor
	void makeIt(GLfloat height, GLfloat width, GLfloat length);
	void computeNormal();
	void renderWithNormals();
	void renderTheNormals();
};

class TriPrismClass2 : public TriPrismClass
{
public:
	TriPrismClass2();
	TriPrismClass2(vec3 p0, vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 p5);
	~TriPrismClass2();
	virtual void makeIt(vec3 p0, vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 p5);
};

class QuadPrismClass
{
public: 
	QuadClass *quadFront;
	QuadClass *quadBack;
	QuadClass *quadTop;
	QuadClass *quadBot;
	QuadClass *quadLeft;
	QuadClass *quadRight;

	QuadPrismClass(); // constructor
	~QuadPrismClass(); // destructor

	QuadPrismClass(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5, vec3 v6, vec3 v7);

	void makeIt(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5, vec3 v6, vec3 v7);

	void computeNormal();

	void renderWithNormals();

	void renderTheNormals();

};



#endif
