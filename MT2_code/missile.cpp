#include "missile.h"



missile::missile(tie * tiePtr, terrain * terPtr, GLint hit_range_v) : projectile(tiePtr, terPtr, hit_range_v)
{
	partPtr = NULL;
	reset();
}


missile::~missile()
{
}

void missile::reset()
{
 	locked = FALSE;
	if(partPtr != NULL) partPtr->disable();
	projectile::reset();
}

void missile::launch(particleLauncher * ptr)
{
	vec3 lineOfSight = normalize(vec3(lookAtPoint) - vec3(planePos)); // line of sight
	vec3 lineOftarget = normalize(targetPtr->getPos() - vec3(planePos));
	if (targetPtr->isActive() && radarLockTimer >= MISSILE_LOCK_TIME)
		locked = TRUE;

	// Start particle system for smoke
	partPtr = ptr;
	partPtr->initSmokeSystem(smokePtr); 
	partPtr->enable();

	projectile::launch(MAX_MISSILE_LIFE, (camSpeed + INIT_MISSILE_SPD) * lineOfSight, planePos);
}

int missile::update()
{
	GLint tempR = projectile::update(); 
	// return if missile is not in normal status. 
	if (tempR != PROJ_NORMAL)
	{
		return tempR;
	}
	
	// update the particle system for smoke 
	partPtr->setPos(pos);

	////// update the velocity ////////
	// Add current velocity of the target to predict the its move
	vec3 vector2Target = targetPtr->getPos() + targetPtr->getVel() - pos;

	// find the rotation axis (normalized), axis = 0 if missile is pointing at target
	vec3 axis = cross(normalize(velocity), normalize(vector2Target));
	GLfloat diff = length(axis);
	GLfloat theta, tickToHit; 

	theta = 180 * acos(dot(normalize(velocity), normalize(vector2Target))) / M_PI;
	theta = Clamp(theta, -MAX_MISSILE_ANGLE_SPD, MAX_MISSILE_ANGLE_SPD);
	
	// find the new speed
	GLfloat acc = (1 - length(axis)) * MAX_MISSILE_ACC; 

	// find the new velocity
 	if(locked && life < MAX_MISSILE_LIFE - 300) velocity = quaternionRotation(velocity, vec3(0.0f), axis, theta);
	if (length(velocity) <= MAX_MISSILE_SPD)
		velocity += acc * normalize(velocity);

	return PROJ_NORMAL;
}

void missile::render()
{
	if (showWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* Wireframe mode */
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); /* fill mode */
	}

	glDisable(GL_TEXTURE_2D);

	glColor3f(0.5, 0.3, 0.6);

	 

	// find orientation axis 
	vec3 rotateAxis = normalize(cross(vec3(0, 0, -1), velocity));

	// find the orientation angle
	GLfloat headAngle = 180 * acos(dot(normalize(velocity), vec3(0, 0, -1))) / M_PI;

	glPushMatrix();
		// Determine rasterization of missile
		glTranslatef(pos.x, pos.y, pos.z);

		glRotatef(headAngle, rotateAxis.x, rotateAxis.y, rotateAxis.z);

		glPushMatrix();
			// draw missile body
			glRotatef(90, -1, 0, 0);
			glPushMatrix();
				glScalef(1, MISSILE_LEN, 1);
				primitive::drawConeWithEnds(MISSILE_RAD, MISSILE_RAD, TexHandles[12], GLU_SMOOTH);
			glPopMatrix();

			// draw missile head
			glPushMatrix();
				glTranslatef(0, MISSILE_LEN, 0);
				glScalef(1, MISSILE_LEN, 1);
				primitive::drawConeWithEnds(MISSILE_RAD, MISSILE_RAD * 0.2f, TexHandles[12], GLU_SMOOTH);
			glPopMatrix();

		glPopMatrix(); 
	
	glPopMatrix();



	if (showTex)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	
}