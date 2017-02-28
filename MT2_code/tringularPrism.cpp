// Triclass 	
#include "tringularPrism.h"
#include "main.h"

TriClass::TriClass() // constructor
{
	p0=vec3(0,0.5,0); // default triangle
	p1=vec3(-0.5,-0.5,0);
	p2=vec3(0.5,-0.5,0);
	computeNormal();
}
 
TriClass::TriClass(vec3 v0, vec3 v1, vec3 v2 ) // constructor
{
	p0=v0;
	p1=v1;
	p2=v2;
	computeNormal();
}

void TriClass::computeNormal()
{
	n0 = polygonNormal(p0,p1,p2);
}

void TriClass::renderWithNormals()
{
	glBegin(GL_TRIANGLES);
	glNormal3fv(n0.fv());
	glTexCoord2f(0, 0.0); glVertex3fv(p0.fv());
	glTexCoord2f(0, 0.3); glVertex3fv(p1.fv());
	glTexCoord2f(0.3, 0); glVertex3fv(p2.fv());
    glEnd();
}

void TriClass::renderTheNormals()
{
	temp1n.x = (p0.x+p1.x+p2.x)/3.0f; // for rendering normals
	temp1n.y = (p0.y+p1.y+p2.y)/3.0f; // for rendering normals
	temp1n.z = (p0.z+p1.z+p2.z)/3.0f; // for rendering normals
	temp2n = temp1n+n0;
	glBegin(GL_LINES);
	
	glVertex3fv(temp1n.fv());
	glVertex3fv(temp2n.fv());
	
    glEnd();
}

// --------------------- Quad --------------------------- // 



	
QuadClass::QuadClass() // constructor
{
	p3=vec3(0.5,0.5,0); // default quad
	p2=vec3(0.5,-0.5,0);
	p1=vec3(-0.5,-0.5,0);
	p0=vec3(-0.5,0.5,0);
	computeNormal();
}
 
QuadClass::QuadClass(vec3 v0, vec3 v1, vec3 v2, vec3 v3 ) // constructor
{
	p0=v0;
	p1=v1;
	p2=v2;
	p3=v3;
	computeNormal();
}

void QuadClass::computeNormal()
{
	n0 = polygonNormal(p0,p1,p2);
}

void QuadClass::renderWithNormals()
{
	

	glBegin(GL_QUADS);
	glNormal3fv(n0.fv());
	glTexCoord2f(0, 0.0);   glVertex3fv(p0.fv());
	glTexCoord2f(0, 0.2);   glVertex3fv(p1.fv());
	glTexCoord2f(0.5, 0.2); glVertex3fv(p2.fv());
	glTexCoord2f(0.5, 0);   glVertex3fv(p3.fv());

    glEnd();
}

void QuadClass::renderWithNormals(GLfloat texW, GLfloat texH)
{


	glBegin(GL_QUADS);
	glNormal3fv(n0.fv());
	glTexCoord2f(0, texH);   glVertex3fv(p0.fv());
	glTexCoord2f(0, 0);   glVertex3fv(p1.fv());
	glTexCoord2f(texW, 0); glVertex3fv(p2.fv());
	glTexCoord2f(texW, texH);   glVertex3fv(p3.fv());

	glEnd();
}
void QuadClass::renderWithNormalsSingleTex(GLuint counter, GLuint slice)
{
	GLfloat step = 1 / (GLfloat)slice;
	GLfloat offset = (GLfloat)(counter * step);
	glBegin(GL_QUADS);
	glNormal3fv(n0.fv());

	glTexCoord2f(1, offset);   glVertex3fv(p0.fv());
	glTexCoord2f(0, offset);   glVertex3fv(p1.fv());
	glTexCoord2f(0, offset + step);   glVertex3fv(p2.fv());
	glTexCoord2f(1, offset + step);   glVertex3fv(p3.fv());


	glEnd();
}

void QuadClass::renderWithNormalsMultiTex(GLuint counter, GLuint slice)
{
	GLfloat step = 1 / (GLfloat)slice;
	GLfloat offset = (GLfloat)(counter * step);
	glBegin(GL_QUADS);
	glNormal3fv(n0.fv());
	glMultiTexCoord2f(GL_TEXTURE0, 0, 0.0);   glMultiTexCoord2f(GL_TEXTURE1, 1, offset);  glVertex3fv(p0.fv());
	glMultiTexCoord2f(GL_TEXTURE0, 0, 0.2);   glMultiTexCoord2f(GL_TEXTURE1, 0, offset); glVertex3fv(p1.fv());
	glMultiTexCoord2f(GL_TEXTURE0, 0.5, 0.2); glMultiTexCoord2f(GL_TEXTURE1, 0, offset + step); glVertex3fv(p2.fv());
	glMultiTexCoord2f(GL_TEXTURE0, 0.5, 0);   glMultiTexCoord2f(GL_TEXTURE1, 1, offset + step); glVertex3fv(p3.fv());
	glEnd();
}

void QuadClass::renderTheNormals()
{
	temp1n.x = (p0.x+p1.x+p2.x+p3.x)/4.0f; // for rendering normals
	temp1n.y = (p0.y+p1.y+p2.y+p3.y)/4.0f; // for rendering normals
	temp1n.z = (p0.z+p1.z+p2.z+p3.z)/4.0f; // for rendering normals
	temp2n = temp1n+n0;
	glBegin(GL_LINES);
	
	glVertex3fv(temp1n.fv());
	glVertex3fv(temp2n.fv());
	
    glEnd();
}


// ------------------------ triangular prism ------------------------------

	TriClass *tri1;
	TriClass *tri2;
	QuadClass *quad1;
	QuadClass *quad2;
	QuadClass *quad3;

	TriPrismClass::TriPrismClass() // constructor
	{
		makeIt(1,1,1);
	}

	TriPrismClass::~TriPrismClass() // destructor
	{
		delete tri1;
		delete tri2;
		delete quad1;
		delete quad2;
		delete quad3;
	}

	TriPrismClass::TriPrismClass(GLfloat height, GLfloat width, GLfloat length) // constructor
	{
		makeIt(height, width, length);
	}

	void TriPrismClass::makeIt(GLfloat height, GLfloat width, GLfloat length)
	{
		vec3 p1 =  vec3(0,height/2,length/2);
		vec3 p2 =  vec3(-width/2,-height/2,length/2);
		vec3 p3 =  vec3(width/2,-height/2,length/2);

		vec3 p4 =  vec3(0,height/2,-length/2);
		vec3 p5 =  vec3(-width/2,-height/2,-length/2);
		vec3 p6 =  vec3(width/2,-height/2,-length/2);

		tri1 = new TriClass(p1,p2,p3);
		tri2 = new TriClass(p6,p5,p4);
		
		quad1 = new QuadClass(p4,p5,p2,p1);
		quad2 = new QuadClass(p3,p6,p4,p1);


		quad3 = new QuadClass(p2,p5,p6,p3);
	}

	void TriPrismClass::computeNormal()
	{
		tri1->computeNormal();
		tri2->computeNormal();
		quad1->computeNormal();
		quad2->computeNormal();
		quad3->computeNormal();
	}

	void TriPrismClass::renderWithNormals()
	{
		tri1->renderWithNormals();
		tri2->renderWithNormals();
		quad1->renderWithNormals();
		quad2->renderWithNormals();
		quad3->renderWithNormals();
	}

	void TriPrismClass::renderTheNormals()
	{
		tri1->renderTheNormals();
		tri2->renderTheNormals();
		quad1->renderTheNormals();
		quad2->renderTheNormals();
		quad3->renderTheNormals();
	}




// ------------------------ Quadrangler prism ------------------------------

	QuadPrismClass::QuadPrismClass() // constructor
	{
		// Back plane clockwise
		vec3 p0 = vec3(-0.5, 0.5, 0.5);
		vec3 p1 = vec3(0.5, 0.5, 0.5);
		vec3 p2 = vec3(0.5, -0.5, 0.5);
		vec3 p3 = vec3(-0.5, -0.5, 0.5);
		
		// Front plane clockwise
		vec3 p4 = vec3(-0.5, 0.5, -0.5);
		vec3 p5 = vec3(0.5, 0.5, -0.5);
		vec3 p6 = vec3(0.5, -0.5, -0.5);
		vec3 p7 = vec3(-0.5, -0.5, -0.5);


		makeIt(p0, p1, p2, p3, p4, p5, p6, p7); 
	}

	QuadPrismClass::QuadPrismClass(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5, vec3 v6, vec3 v7)
	{
		makeIt(v0, v1, v2, v3, v4, v5, v6, v7);
	}

	QuadPrismClass::~QuadPrismClass()
	{
		if (quadBack != NULL) { delete quadBack; quadBack = NULL; }
			

		delete quadFront;
		delete quadLeft;
		delete quadRight;
		delete quadTop;
		delete quadBot;


	}

	void QuadPrismClass::makeIt(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5, vec3 v6, vec3 v7)
	{
		quadBack  = new QuadClass(v3, v7, v6, v2);
		quadFront = new QuadClass(v0, v1, v5, v4);
		quadLeft  = new QuadClass(v0, v4, v7, v3);
		quadRight = new QuadClass(v2, v6, v5, v1);
		quadTop   = new QuadClass(v0, v3, v2, v1);
		quadBot   = new QuadClass(v4, v5, v6, v7);



	}

	void QuadPrismClass::computeNormal()
	{
		quadBack->computeNormal();
		quadFront->computeNormal();
		quadLeft->computeNormal();
		quadRight->computeNormal();
		quadTop->computeNormal();
		quadBot->computeNormal();

	}

	void QuadPrismClass::renderWithNormals()
	{
		quadBack->renderWithNormals();
		quadFront->renderWithNormals();
		quadLeft->renderWithNormals();
		quadRight->renderWithNormals();
		quadTop->renderWithNormals();
		quadBot->renderWithNormals();


	}

	void QuadPrismClass::renderTheNormals()
	{
		quadBack->renderTheNormals();
		quadFront->renderTheNormals();
		quadLeft->renderTheNormals();
		quadRight->renderTheNormals();
		quadTop->renderTheNormals();
		quadBot->renderTheNormals();

	}

	TriPrismClass2::TriPrismClass2()
	{
		// Back plane clockwise
		vec3 p0 = vec3(-0.5, 0.5, 0.5);
		vec3 p1 = vec3(0.5, 0.5, 0.5);
		vec3 p2 = vec3(0.5, -0.5, 0.5);

		// Front plane clockwise
		vec3 p3 = vec3(-0.5, 0.5, -0.5);
		vec3 p4 = vec3(0.5, 0.5, -0.5);
		vec3 p5 = vec3(0.5, -0.5, -0.5);

		makeIt(p0, p1, p2, p3, p4, p5);

	}

	TriPrismClass2::TriPrismClass2(vec3 p0, vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 p5)
	{
		makeIt(p0, p1, p2, p3, p4, p5);
	}

	void TriPrismClass2::makeIt(vec3 p0, vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 p5)
	{
		tri1 = new TriClass(p0, p2, p1); // the triangle at back
		tri2 = new TriClass(p3, p4, p5); // the triangle at front
		quad1 = new QuadClass(p3, p0, p1, p4);  // top quad
		quad2 = new QuadClass(p1, p2, p5, p4);  // left
		quad3 = new QuadClass(p3, p5, p2, p0);  // right

	}

	TriPrismClass2::~TriPrismClass2()
	{
	//base class handles delete pointers

	}


// end