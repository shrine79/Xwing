#include "skybox.h"

static GLfloat sb_Verts[8][3] = {
	{ -0.5 ,  0.5 , -0.25  },   /* 0 left top rear */
	{ 0.5 ,  0.5 , -0.25  },	/* 1 right top rear */
	{ 0.5 , -0.5 , -0.25  },	/* 2 right bottom rear */
	{ -0.5 , -0.5 , -0.25  },	/* 3 left bottom rear */
	{ -0.5 ,  0.5 ,  0.75  },	/* 4 left top front */
	{ 0.5 ,  0.5 ,  0.75  },	/* 5 right top front */
	{ 0.5 , -0.5 ,  0.75  },	/* 6 right bottom front */
	{ -0.5 , -0.5 ,  0.75  }	/* 7 left bottom front */
};

static GLuint Faces[6][4] = {
	7, 4, 5, 6,	/* front */
	6, 5, 1, 2, /* left */
	3, 0, 4, 7, /* right */
	5, 4, 0, 1,	/* top */
	2, 3, 7, 6,	/* bottom */
	2, 1, 0, 3, /* rear */
};

static GLfloat Normals[6][3] = {
	0, 0, -1, 	/* front */
	1, 0, 0, 	/* left */
	-1, 0, 0, 	/* right */
	0, -1, 0, 	/* top */
	0, 1, 0,   /* bottom */
	0, 0,  1   /* rear */
};

static GLfloat sb_texture[6][2] = {
	0.50, 0.25,  /* front*/
	0.25, 0.25,  /* left*/
	0.75 ,0.25,  /* right*/
	0.25, 0.50,  /* top */
	0.25, 0.0,   /* bottom */
	1.00, 0.25   /* rear */
};

void skybox::rotate(GLfloat degree)
{
	
	sunPos = quaternionRotation(sunPos, sunOrigin, sunAxis, degree); 
}


skybox::skybox(GLuint sky_h, GLuint sun_h, GLuint moon_h, vec3 axis, vec3 origin)
{
	skyHandle = sky_h;
	sunHandle = sun_h;
	moonHandle = moon_h;
	length = SKY_LEN; 
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++)
			sb_Verts[i][j] *= SKY_LEN;

	sunPos = vec3(SUN_START_POS_X, SUN_START_POS_Y, SUN_START_POS_Z);
	sunPtr = new billboard(sun_h, omni_orient, bb_blend_mode::alpha, bb_orientation::verticle, 1, 1);
	sunPtr->setPos(sunPos);
	sunPtr->setupTex(sun_h, SUN_SIZE, SUN_SIZE);
	sunAxis = axis; 
	sunOrigin = origin;
	oldTime = glutGet(GLUT_ELAPSED_TIME);
	spinSpd = 5; 
	isDay = TRUE;
}


skybox::~skybox()
{
}

void skybox::render()
{
	glShadeModel(GL_FLAT);
	glDisable(GL_LIGHT0);
	glColor3fv(white);
	glBindTexture(GL_TEXTURE_2D, skyHandle);

	if (showWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT, GL_FILL);

	setMaterial(8); 

	vec3 p0, p1, p2, p3, pn;
	for (int face = 0; face < 6; face++)
	{
		glBegin(GL_QUADS);
		p0 = vec3(sb_Verts[Faces[face][0]]);
		p1 = vec3(sb_Verts[Faces[face][1]]);
		p2 = vec3(sb_Verts[Faces[face][2]]);
		p3 = vec3(sb_Verts[Faces[face][3]]);

		glNormal3fv(Normals[face]);

		glTexCoord2f(sb_texture[face][0] + 0.25, sb_texture[face][1]);               glVertex3fv(p0.fv());
		glTexCoord2f(sb_texture[face][0] + 0.25, sb_texture[face][1] + 0.25);	     glVertex3fv(p1.fv());
		glTexCoord2f(sb_texture[face][0], sb_texture[face][1] + 0.25);				 glVertex3fv(p2.fv());
		glTexCoord2f(sb_texture[face][0], sb_texture[face][1]);						 glVertex3fv(p3.fv());
		
		glEnd();

		if (showNormals) 
		{
			glBegin(GL_LINES);

			pn = (p0 + p1 + p2 + p3) / 4;
			glVertex3fv(pn.fv());
			glVertex3fv((pn + vec3(Normals[face]) * 100).fv());

			glEnd();
		}
		
	}
	
	glDisable(GL_DEPTH_TEST); 
	renderSun();
	if (hiddenDepth) glEnable(GL_DEPTH_TEST); 

	if (eLight0) glEnable(GL_LIGHT0); 
}

void skybox::renderSun(void)
{
	//rotate sun to new position
	curTime = glutGet(GLUT_ELAPSED_TIME);
	GLfloat degree = spinSpd * (curTime - oldTime) / 1000;
	oldTime = curTime;
	// if sun has set, reset sun to rise
	if (sunPos.y < -40)
	{
		sunPos = vec3(SUN_START_POS_X, SUN_START_POS_Y, SUN_START_POS_Z);
		isDay = !isDay;
		if (isDay) { sunPtr->setupTex(sunHandle, SUN_SIZE, SUN_SIZE);  }
		else{ sunPtr->setupTex(moonHandle, MOON_SIZE, MOON_SIZE); }
	}

	setSunMat();
	rotate(degree); 
	sunPtr->setPos(sunPos);
	sunPtr->render();
}

vec3 skybox::getSunPos(void)
{
	return sunPos; 
}