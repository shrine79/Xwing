/****		Cube in points-polygons (polyhedron) form	****/

#include "CubeClass.h"
#include "glut.h"
#include "main.h"

static GLfloat Verts[8][3] = {
  { -0.5,  0.5, -0.5 }, /* 0 left top rear */
  {  0.5,  0.5, -0.5 },	/* 1 right top rear */
  {  0.5, -0.5, -0.5 },	/* 2 right bottom rear */
  { -0.5, -0.5, -0.5 },	/* 3 left bottom rear */
  { -0.5,  0.5,  0.5 },	/* 4 left top front */
  {  0.5,  0.5,  0.5 },	/* 5 right top front */
  {  0.5, -0.5,  0.5 },	/* 6 right bottom front */
  { -0.5, -0.5,  0.5 }	/* 7 left bottom front */
};

static GLuint Faces[6][4] = {
  4, 5, 6, 7,	/* front */
  5, 1, 2, 6,	/* right */
  0, 4, 7, 3,	/* left */
  4, 0, 1, 5,	/* top */
  7, 6, 2, 3,	/* bottom */
  1, 0, 3, 2	/* rear */
};

static GLfloat Normals[6][3] = {
  0, 0, 1, 	/* front */
  1, 0, 0, 	/* right */
  -1, 0, 0, 	/* left */
  0, 1, 0, 	/* top */
  0, -1, 0, /* bottom */
  0, 0, -1   /* rear */
};

static GLfloat spareVertex[3];

//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//  glColor3f(0.0, 1.0, 1.0);
//  glLineWidth(3);


CubeClass::CubeClass()
{
}

void CubeClass::renderCW()
{
  
  /* Draw cube in traditional OpenGL style */
  glBegin(GL_QUADS);
	for (int face = 0; face < 6; face++) 
		{
		for (int vert = 0; vert < 4; vert ++)
			glVertex3fv(Verts[Faces[face][vert]]);
		}

  glEnd();
}

void CubeClass::renderCCW()
{
  
  /* Draw cube in traditional OpenGL style */
  glBegin(GL_QUADS);
	for (int face = 0; face < 6; face++) 
		{
		for (int vert = 3; vert >= 0; vert--)
			glVertex3fv(Verts[Faces[face][vert]]);
		}

  glEnd();
}

void CubeClass::render()
{
	renderCCW();
}

void CubeClass::renderWithNormals()
{
	  
  /* Draw cube in traditional OpenGL style */
  glBegin(GL_QUADS);
	for (int face = 0; face < 6; face++) 
	{
		glNormal3fv(Normals[face]);
		
		glTexCoord2f(0.5, 0.5);
		glVertex3fv(Verts[Faces[face][3]]);
		glTexCoord2f(0.5, 0);
		glVertex3fv(Verts[Faces[face][2]]);
		glTexCoord2f(0, 0);
		glVertex3fv(Verts[Faces[face][1]]);
		glTexCoord2f(0, 0.5);
		glVertex3fv(Verts[Faces[face][0]]);
	}

  glEnd();
}

void CubeClass::renderFlat(GLfloat texScale)
{
	glShadeModel(GL_FLAT);
	glBegin(GL_QUADS);
	for (int face = 0; face < 6; face++)
	{
		glNormal3fv(Normals[face]);

		glTexCoord2f(texScale, texScale);
		glVertex3fv(Verts[Faces[face][3]]);
		glTexCoord2f(texScale, 0);
		glVertex3fv(Verts[Faces[face][2]]);
		glTexCoord2f(0, 0);
		glVertex3fv(Verts[Faces[face][1]]);
		glTexCoord2f(0, texScale);
		glVertex3fv(Verts[Faces[face][0]]);
	}
	glEnd();
}

void CubeClass::renderSmooth(GLfloat texScale)
{
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);

	for (int face = 0; face < 6; face++)
	{
		glNormal3fv(Normals[face]);
		glTexCoord2f(texScale, texScale);
		glVertex3fv(Verts[Faces[face][3]]);
		glNormal3fv(Normals[face]);
		glTexCoord2f(texScale, 0);
		glVertex3fv(Verts[Faces[face][2]]);
		glNormal3fv(Normals[face]);
		glTexCoord2f(0, 0);
		glVertex3fv(Verts[Faces[face][1]]);
		glNormal3fv(Normals[face]);
		glTexCoord2f(0, texScale);
		glVertex3fv(Verts[Faces[face][0]]);
	}
	glEnd();
}
void CubeClass::renderTessellation(GLfloat texScale)
{
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);

	vec3 sideA, sideB, stepA, stepB, stepA_cur, stepB_cur, startPoint; 
	GLfloat lenA, lenB, tessTexScaleA, tessTexScaleB;
	

	for (int face = 0; face < 6; face++)
	//for (int face = 0; face < 2; face++)
	{
		// calculate side A and side B of the quad
		sideA = vec3(&(Verts[Faces[face][0]][0])) - vec3(&(Verts[Faces[face][1]][0]));
		sideB = vec3(&(Verts[Faces[face][2]][0])) - vec3(&(Verts[Faces[face][1]][0]));
		lenA = length(sideA);
		lenB = length(sideB); 
		stepA = sideA * TESS_STEP/lenA;
		stepB = sideB * TESS_STEP/lenB;
		tessTexScaleA = texScale * TESS_STEP / lenA;
		tessTexScaleB = texScale * TESS_STEP / lenB;

		int it_A, it_B; 
		// loop along side B
		it_B = 0; // clear iterator B
		stepB_cur = stepB; //initialize current step
		do 
		{
			startPoint = vec3(&Verts[Faces[face][1]][0]) + it_B * stepB;

			if (lenB < length((it_B + 1) * stepB))
				stepB_cur = sideB - (stepB * it_B);

			// loop along side A
			it_A = 0; // clear iterator A
			stepA_cur = stepA;  //initialize current step
			do 
			{
				if (lenA < length((it_A + 1) * stepA))
					stepA_cur = sideA - (stepA * it_A);
				
				glNormal3fv(Normals[face]); glTexCoord2f(tessTexScaleA * (it_B), tessTexScaleB * (it_A)); glVertex3fv(startPoint.fv());
				glNormal3fv(Normals[face]); glTexCoord2f(tessTexScaleA * (it_B), tessTexScaleB * (it_A + 1));	  glVertex3fv((startPoint + stepA_cur).fv());
				glNormal3fv(Normals[face]); glTexCoord2f(tessTexScaleA * (it_B + 1), tessTexScaleB * (it_A + 1));		  glVertex3fv((startPoint + stepA_cur + stepB_cur).fv());
				glNormal3fv(Normals[face]); glTexCoord2f(tessTexScaleA * (it_B + 1 ), tessTexScaleB * (it_A));	  glVertex3fv((startPoint + stepB_cur).fv());


				startPoint += stepA;
				it_A++;
			} while (lenA > length(it_A * stepA));

			it_B++;
		} while (lenB > length(it_B * stepB)); 
	}
	glEnd();
}


void CubeClass::renderWithNormals(GLfloat texScale)
{
	/* Draw cube in traditional OpenGL style */
	if (flatNormal)
		renderFlat(texScale);
	else
		if (tessellation)
			renderTessellation(texScale); 
		else
			renderSmooth(texScale);
}


void  CubeClass::renderTheNormals()
{
	// this normal renderinmg routine is not general purpose and works only with the cube
	glBegin(GL_LINES);
	glColor3f(1,1,0);
	for (int face = 0; face < 6; face++) 
		{
		spareVertex[0]=Normals[face][0]/2.0f; //x
		spareVertex[1]=Normals[face][1]/2.0f; //y
		spareVertex[2]=Normals[face][2]/2.0f; //z
		glVertex3fv(spareVertex);
		glVertex3fv(Normals[face]);
		}
	glEnd();
}

// end
