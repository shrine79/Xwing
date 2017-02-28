#include "laser.h"
#include "primitive.h"


laser::laser(tie * tiePtr, terrain * terPtr, GLint hit_range_v)
	:projectile(tiePtr, terPtr, hit_range_v)
{
}


laser::~laser()
{
}


void laser::render()
{
	// render laser
	glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z); 
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		bbPtr->setRot(rotAxis_m, headAngle_m);
		//glRotatef(90, 1, 0, 0);  // lay down the up right laser bill board. 
		bbPtr->render(); 
		//// debug: render a ball representing laser beam
		//primitive::drawSphere(GL_FLAT);
		if (hiddenCull) glEnable(GL_CULL_FACE); 
		if (eLight) glEnable(GL_LIGHTING);
	glPopMatrix(); 
}


GLboolean laser::launch(const vec3& dir, const vec3& pos_v)
{
	GLint curTime = glutGet(GLUT_ELAPSED_TIME); 
	if ((curTime - fireTimeStamp) > LASER_INTERVAL)
	{
		projectile::launch(LASER_MAX_LIFE, (camSpeed + LASER_SPD) * dir, pos_v);
		// Latch the current plane rotation parameters 
		headAngle_m = headAngle;
		rotAxis_m = planeRotateAxis;
		fireTimeStamp = curTime;
		return TRUE; 
	}

	return FALSE;
}


// initialization of static members. 
GLint laser::fireTimeStamp = 0; 
billboard * laser::bbPtr = NULL;