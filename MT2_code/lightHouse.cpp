#include "lightHouse.h"
#include "primitive.h"
#include "main.h"
#include "glut.h"

static float	ldif[] = { 0.5f, 0.5f, 0.5f, 1.0f };
static float	lamb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static float	lspe[] = { 0.7f, 0.7f, 0.5f, 1.0f };

lightHouse::lightHouse()
{
	setup(1, LIGHT_HOUSE_HEIGHT, 15, vec3(1, -0.4f, 0), 1);
	oldTime = 0;
}


lightHouse::~lightHouse()
{
}

void lightHouse::setup(GLfloat rad, GLfloat h, GLuint angle, vec3 dir, GLfloat spd)
{
	radius = rad;
	height = h; 
	cutOffAngle = angle; 
	beamDir = dir;
	spinSpd = spd; 
}

void lightHouse::setupBeam(GLuint angle, vec3 dir, GLfloat spd)
{
	cutOffAngle = angle;
	beamDir = dir;
	spinSpd = spd;
}

void lightHouse::update()
{
	glLightfv(GL_LIGHT1, GL_POSITION, lpos2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lamb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, ldif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lspe);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutOffAngle);

	rotateBeam();  // rotate beam 
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, beamDir.fv());

}

void lightHouse::rotateBeam()
{
	GLfloat tmpx, tmpz, yaw;
	GLint current_time = glutGet(GLUT_ELAPSED_TIME);
	yaw = spinSpd * (current_time - oldTime) / 1000;
	oldTime = current_time;
	tmpx = (beamDir.z * sin(yaw)) + (beamDir.x * cos(yaw));
	tmpz = (beamDir.z * cos(yaw)) - (beamDir.x * sin(yaw));
	beamDir.z = tmpz;
	beamDir.x = tmpx;
}

int lightHouse::render()
{
	if (showWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (showTex)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);


	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3fv(magenta);

	glTranslatef(0, 2.5f, -10);

	glPushMatrix();
	glScalef(1, height, 1); 
	primitive::drawConeWithEnds(radius, radius * 0.7f, TexHandles[6], GLU_SMOOTH); 
	glPopMatrix();

	glTranslatef(0, height, 0);
	primitive::drawConeWithEnds(radius * 0.7, radius * 0.75f, TexHandles[6], GLU_SMOOTH);

	glTranslatef(0, 1, 0);
	primitive::drawConeWithEnds(radius * 0.15, radius * 0.1, TexHandles[6], GLU_SMOOTH);

	setMaterial(7); 
	glTranslatef(0, 1, 0);
	glScalef(1, 0.2, 1);
	primitive::drawConeWithEnds(radius * 0.1, radius * 0.1, TexHandles[5], GLU_SMOOTH);


	glPopAttrib();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	return true;
}