#include "tie.h"
#include "missile.h"
#include <stdio.h>


tie::tie(terrain * tPtr)
{
	reset();
	nextFreeSlot = 0;
	isLocked = FALSE;
	centre_wing = QuadClass(vec3(-TIE_L0 / 2, -TIE_W0 / 2,  0),
								 vec3(TIE_L0 / 2,-TIE_W0 / 2,  0),
								 vec3(TIE_L0 / 2,TIE_W0 / 2,   0),
								 vec3( -TIE_L0 / 2,  TIE_W0 / 2,0));

	side_wing = QuadClass(vec3( -TIE_L1/2,  -TIE_W1, 0), 
						  vec3(  TIE_L1 / 2,-TIE_W1, 0), 
						  vec3(  TIE_L2 / 2, 0      , 0), 
						  vec3( -TIE_L0 / 2,0      , 0));

	terPtr = tPtr; 
}


tie::~tie()
{
}

void tie::renderWing()
{
	glPushMatrix();
		//setMaterial(9);
		glRotatef(90, 0, 1, 0); 
		glPushMatrix();
			glScalef(0.3, 0.3, TIE_WING_SPAN); 
			primitive::drawCylinder(GL_FLAT, TexHandles[12]);
		glPopMatrix();
		glDisable(GL_CULL_FACE);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		//setMaterial(2);
		centre_wing.renderWithNormalsSingleTex(0, 3);
		
		glPushMatrix();
			glTranslatef(0, -TIE_W0/2, 0);
			glRotatef(TIE_WING_ANGLE, -1, 0, 0);
			side_wing.renderWithNormalsSingleTex(0, 3);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0, TIE_W0 / 2, 0);
			glRotatef(-TIE_WING_ANGLE, -1, 0, 0);
			glRotatef(180, -1, 0, 0);
			side_wing.renderWithNormalsSingleTex(0, 3);
		glPopMatrix();
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		
		if (hiddenCull)
			glEnable(GL_CULL_FACE);

	glPopMatrix();
}

void tie::render()
{
	if (!active) return;

	
	glPushMatrix();

		glTranslatef(pos.x, pos.y, pos.z);

		/**************** draw the tie fighter **********/
		glPushMatrix();
			// Find orientation of TIE fighter
			vec3 rotateAxis = normalize(cross(vec3(0, 0, -1), velocity));
			GLfloat headAngle = 180 * acos(dot(normalize(velocity), vec3(0, 0, -1))) / M_PI;
			GLfloat t = length(velocity); 
			glRotatef(headAngle, rotateAxis.x, rotateAxis.y, rotateAxis.z);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			setMaterial(9);
			glScalef(1.5, 1.5, 1.5);
			primitive::drawSphere(GL_FLAT);
			glPushMatrix();
				glTranslatef(-TIE_WING_SPAN, 0, 0);
				renderWing();
			glPopMatrix();

			glPushMatrix();
				glTranslatef( TIE_WING_SPAN, 0, 0);
				glRotatef(180, 0, 0, 1);
				renderWing();
			glPopMatrix();
		glPopMatrix();

		/********* Draw markers around TIE fighter *************/
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		// Draw bounding box
		glPushMatrix();
			glColor3f(0, 1, 0);
			glScalef(8, 8, 8);
			int temp = tessellation;
			tessellation = FALSE;
			glLineWidth(1);
			primitive::drawWireCubeWithNormals();
			tessellation = temp;
		glPopMatrix();

		// Draw lock marker
		if (radarLockTimer >= MISSILE_LOCK_TIME - TIE_MARK_ANIMATION_TIME)
		{
			// Start lock marker animation. 
			if (isLocked == FALSE)
			{
				lockMarkerRot = 0;
				lockMarkerSize = TIE_LOCK_MARKER_INI_SIZE;
			}

			// lock marker animation factors
			lockMarkerRot += display_time_interval * TIE_MARK_ROT_SPD;
			lockMarkerRot = fmodf(lockMarkerRot, 360);
			if (lockMarkerSize > TIE_LOCK_MARKER_END_SIZE)
				lockMarkerSize *= TIE_MARK_SCALE_SPD;
			else
				lockMarkerSize = TIE_LOCK_MARKER_END_SIZE;

			glPushMatrix();
				glColor3f(1, 0.5, 0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_DEPTH_TEST);
				removeModelRot();  // remove rotation from current modelview matrix to make the marker always face the camera. 
				glRotatef(lockMarkerRot, 0, 0, -1);
				glLineWidth(2.0f); // make line thicker
				glBegin(GL_TRIANGLES);
				glVertex3f(-lockMarkerSize * 0.5, -0.57 * lockMarkerSize * 0.5, 0);
				glVertex3f(lockMarkerSize * 0.5, -0.57 * lockMarkerSize * 0.5, 0);
				glVertex3f(0, lockMarkerSize * 0.5 * 1.16, 0);
				glEnd();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_DEPTH_TEST);
			glPopMatrix();

			// TIE fighter is locked
			if (radarLockTimer >= MISSILE_LOCK_TIME)
				isLocked = TRUE;
			else
				isLocked = FALSE;
		}
		

		if (eLight) glEnable(GL_LIGHTING);
		if (showTex) glEnable(GL_TEXTURE_2D);

	glPopMatrix();
}

// drop the tie fighter
void tie::drop(particleLauncher * p)
{
	falling = TRUE; 
	partPtr = p; 
	partPtr->initSmokeSystem1(flamePtr); 
	partPtr->enable(); 
}

void tie::endDrop()
{
	partPtr->disable();
}

GLboolean tie::hit(GLint damage)
{
	if (falling) return FALSE;
	
	hitPoint -= damage;
	if (hitPoint <= 0) {return TRUE; }
	
	return FALSE;
}

// Update tie fighter. return True if it is meant to explode. 
GLboolean tie::update()
{
	GLint time_interval;
	// doing nothing if inactive. 
	if (active == FALSE) { return FALSE; }

	// move to the new position
	posOld = pos; 
	pos = pos + velocity;

	// handle falling
	if (falling)
	{
		velocity.y -= TIE_DROP_GRAVITY;
		if (length(velocity) >= TIE_MAX_DROP_SPD)
			TIE_MAX_DROP_SPD * normalize(velocity); 

		drop_counter--;
		partPtr->setPos(pos); 

		if (drop_counter == 0 || terPtr->hitGround(pos, posOld, pos) || pos.y < SURFACE)
		{
			reset();
			partPtr->kill();
			return TRUE; 
		}

		//partPtr->update(); 

		return FALSE;
	}


	// Collision detection. if the tie fighter is close enough to the route point,
	// heading to the next one
	if (dist(pos, routeMap[curRoutePoint]) < 10)
	{
 		lastRoutePoint = curRoutePoint; 
		curRoutePoint = (curRoutePoint + 1) % nextFreeSlot; 
	}

	// find the rotation axis (normalized), axis = 0 if plane is pointing at target
	vec3 axis = cross(normalize(velocity), normalize(routeMap[curRoutePoint] - pos));
	GLfloat diff = length(axis);
	GLfloat theta, tickToHit, dotRes; 
	dotRes = dot(normalize(velocity), normalize(routeMap[curRoutePoint] - pos)); 
	dotRes = Clamp(dotRes, -1, 1); 
	theta = 180 * acos(dotRes) / M_PI;
	tickToHit = (dist(routeMap[curRoutePoint], pos)) / length(velocity);
	tickToHit = Clamp(tickToHit, 1, 20); 
	theta /= tickToHit; 
	theta = Clamp(theta, -MAX_TIE_ANGLE_SPD, MAX_TIE_ANGLE_SPD);
	velocity = quaternionRotation(velocity, vec3(0.0f), axis, theta);
	velocity = ((1.5 - length(axis)) * BASIC_TIE_SPD) * normalize(velocity);

	return FALSE;
}

vec3 tie::getPos()
{
	return pos; 
}

vec3 tie::getVel()
{
	return velocity;
}

void tie::reset()
{
	falling = FALSE; 
	active = FALSE;
	hitPoint = TIE_HP;
	drop_counter = TIE_DROP_LIFE;
}

void tie::enable()
{
	active = TRUE; 
	pos = routeMap[0];
	posOld = pos;
	lastRoutePoint = 0;
	curRoutePoint = 1;
	velocity = 1.5 * BASIC_TIE_SPD * normalize(routeMap[1] - routeMap[0]);
}

void tie::pushRoutePoint(const vec3& p)
{
	if (nextFreeSlot >= MAX_ROUTE_POINTS)
		return;  // route map is full 
	routeMap[nextFreeSlot] = p; 
	nextFreeSlot++; 
}