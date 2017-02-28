#include "hud.h"
#include "utility.h"
#include "missile.h"
#include "main.h"


hud::hud(GLuint chPtr, GLuint cpPtr, GLuint lcPtr, tie * tPtr)
{
	crossHairHandle = chPtr; 
	//crossHairPos = vec3(INI_WIN_WIDTH / 2, INI_WIN_HEIGHT / 2 , HUD_FRUSTUM_DIST);
	GLint screen_size[4];
	glGetIntegerv(GL_VIEWPORT, screen_size); 
	crossHairPos = vec3(screen_size[2]/2, screen_size[3]/2, HUD_FRUSTUM_DIST);
	targetPtr = tPtr;
	compassHandle = cpPtr;
	lockingCircleHandle = lcPtr;
}


hud::~hud()
{
}

void hud::setup2D()
{
}

void hud::renderCrossHair()
{
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(crossHairPos.x - HUD_CROSSHAIR_SIZE, crossHairPos.y - HUD_CROSSHAIR_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex2f(crossHairPos.x + HUD_CROSSHAIR_SIZE, crossHairPos.y - HUD_CROSSHAIR_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex2f(crossHairPos.x + HUD_CROSSHAIR_SIZE, crossHairPos.y + HUD_CROSSHAIR_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex2f(crossHairPos.x - HUD_CROSSHAIR_SIZE, crossHairPos.y + HUD_CROSSHAIR_SIZE);
	glEnd();
}

void hud::renderCompass()
{
	// rotate compass upright
	// calculate the angle to roll
	GLfloat temp = vec2PlaneAngle(lineOfSightPlane, vec3(0, 1, 0), vec3(upVec));
	if (upVec[1] < 0)
		temp = 180 - temp;
	
	glPushMatrix();

		// translate to origin, which is the left botton corner in orthographic mode, then rotate, and translate back to original position.
		glTranslatef(crossHairPos.x, crossHairPos.y, 0);
		glRotatef(temp, 0, 0, 1);
		glTranslatef(-crossHairPos.x, -crossHairPos.y, 0);

		glDisable(GL_BLEND);
		// render the up right pointer
		vec3 upPtrPos = crossHairPos + vec3(0, HUD_UP_INDICATOR_OFFSET, 0);

		glDisable(GL_TEXTURE_2D);
		glColor3f(1, 1, 0);
		glBegin(GL_TRIANGLES);
		glVertex2f(upPtrPos.x, upPtrPos.y);
		glVertex2f(upPtrPos.x + 10, upPtrPos.y + 20);
		glVertex2f(upPtrPos.x - 10, upPtrPos.y + 20);
		glEnd();

	glPopMatrix();

	// render the compass
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, compassHandle);
		glPushMatrix();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(crossHairPos.x - HUD_COMPASS_SIZE, crossHairPos.y - HUD_COMPASS_SIZE);
		glTexCoord2f(1.0, 0.0); glVertex2f(crossHairPos.x + HUD_COMPASS_SIZE, crossHairPos.y - HUD_COMPASS_SIZE);
		glTexCoord2f(1.0, 1.0); glVertex2f(crossHairPos.x + HUD_COMPASS_SIZE, crossHairPos.y + HUD_COMPASS_SIZE);
		glTexCoord2f(0.0, 1.0); glVertex2f(crossHairPos.x - HUD_COMPASS_SIZE, crossHairPos.y + HUD_COMPASS_SIZE);
		glEnd();
	glPopMatrix();

	// render the locking circle
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, lockingCircleHandle);
		glTranslatef(crossHairPos.x, crossHairPos.y, 0);
		glRotatef(lockingCircleRot, 0, 0, 1);
		glTranslatef(-crossHairPos.x, -crossHairPos.y, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(crossHairPos.x - HUD_LOCKING_CIRCLE_SIZE, crossHairPos.y - HUD_LOCKING_CIRCLE_SIZE);
		glTexCoord2f(1.0, 0.0); glVertex2f(crossHairPos.x + HUD_LOCKING_CIRCLE_SIZE, crossHairPos.y - HUD_LOCKING_CIRCLE_SIZE);
		glTexCoord2f(1.0, 1.0); glVertex2f(crossHairPos.x + HUD_LOCKING_CIRCLE_SIZE, crossHairPos.y + HUD_LOCKING_CIRCLE_SIZE);
		glTexCoord2f(0.0, 1.0); glVertex2f(crossHairPos.x - HUD_LOCKING_CIRCLE_SIZE, crossHairPos.y + HUD_LOCKING_CIRCLE_SIZE);
		glEnd();
	glPopMatrix();
}

void hud::renderTargetIndicator()
{
	glPushMatrix();
	// render the target indicator

	// calculate the projection of the target vector
	vec3 tarVec = normalize(targetPtr->getPos() - vec3(eyePoint));

	//draw target indicator if necessary 
	if (targetPtr->isActive())	 
	{
		if(dot(tarVec, lineOfSightNorm) < 0.92 || length(targetPtr->getPos() - planePos) > MISSILE_LOCK_RANGE)
		{
			// Target is out of lock prism, draw red triangle to indicate the relative direction of target
			// reset lock timer
			radarLockTimer = 0;

			// normalized projection on hud plane
			vec3 indicatorVec = projectVecOnPlane(lineOfSightNorm, tarVec);
			// find the angle between the upVec and the indicator vector
			GLfloat temp = 180 * acos(dot(vec3(upVec), normalize(indicatorVec))) / M_PI;

			// check the relative position of upVec and indicator vector.
			GLfloat tempVec = dot(normalize(cross(upVec, indicatorVec)), lineOfSightNorm);
			if (tempVec > 0.05)
				temp = -temp;
			else if (tempVec <= 0.05 && tempVec > -0.05)
				temp = 0;


			// translate to origin, which is the left botton corner in orthographic mode, then rotate, and translate back to original position.
			glTranslatef(crossHairPos.x, crossHairPos.y, 0);
			glRotatef(temp, 0, 0, 1);
			glTranslatef(-crossHairPos.x, -crossHairPos.y, 0);

			// render the up right pointer
			vec3 tarPtrPos = crossHairPos + vec3(0, HUD_TARGET_INDICATOR_OFFSET, 0);

			glDisable(GL_TEXTURE_2D);
			glColor3f(1, 0, 0);
			glBegin(GL_TRIANGLES);
			glVertex2f(tarPtrPos.x, tarPtrPos.y);
			glVertex2f(tarPtrPos.x - 10, tarPtrPos.y - 20);
			glVertex2f(tarPtrPos.x + 10, tarPtrPos.y - 20);
			glEnd();
		}
		else
		{
			// Our radar is trying to lock the target, show some indication
			radarLockTimer += glutGet(GLUT_ELAPSED_TIME) - displayTimestamp;
			lockingCircleRot += display_time_interval * HUD_LOCKING_CIRCLE_SPD; 
			lockingCircleRot = fmodf(lockingCircleRot, 360);
			
			if (radarLockTimer > MISSILE_LOCK_TIME)
			{
				// lock succeed. ear mark the target in perspective mode, not here
				radarLockTimer = MISSILE_LOCK_TIME;
			}
		}
		
	}

	glPopMatrix();
}

void hud::render()
{
	glColor3f(1, 1, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, crossHairHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// Apply addictive blending
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND); 

	
	// render the cross hair
	renderCrossHair();

	// render the arrow pointing target. 
	renderTargetIndicator();

	// render the compass and upright indicator
	renderCompass();

	

}