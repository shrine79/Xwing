#include "skybox.h"
#include "projectile.h"



projectile::projectile(tie * tiePtr, terrain * terPtr, GLint hit_range_v) 
	: targetPtr(tiePtr), terrainPtr(terPtr), hit_range(hit_range_v)
{
	reset();
}


projectile::~projectile()
{
}

void projectile::reset()
{
	Active = FALSE;
	life = PROJ_LIFE;
	timeStamp = 0;
	velocity = vec3(0.0f); 
	pos = vec3(0.0f);
	oldPos = pos;
	time_interval = 0;
}

void projectile::launch(const int& life_v, const vec3& vel_v, const vec3& pos_v)
{
	Active = TRUE;
	life = life_v;
	velocity = vel_v; 
	timeStamp = glutGet(GLUT_ELAPSED_TIME);
	pos = pos_v;
}


/* return 0 if not acitve 
   otherwise return 1 */
int projectile::update()
{
	// Test active status and update time
	if (Active == FALSE) { return PROJ_INACTIVE; }
	else
	{
		time_interval = glutGet(GLUT_ELAPSED_TIME) - timeStamp;
		life -= time_interval;

		timeStamp += time_interval;
	}

	// test the living condition
	if (life <= 0)
	{
		reset();
		return PROJ_DIE;
	}

	// move to the new position
	oldPos = pos; 
	pos = pos + velocity;

	// Boundary test, projectile die if went out of sky box. 
	if (pos.x > 0.5 * SKY_LEN || pos.x < -0.5 * SKY_LEN || pos.z > 0.75 * SKY_LEN || pos.z < -0.25 * SKY_LEN)
	{
		reset(); 
		return PROJ_DIE;
	}

	/***** collision detection with target ****
	The collision detection happens in two stages:
	stage 1: test the distance between current position of projectile and the target
			 If the distance is less than the detect range, proceed to stage 2
	stage 2: test the distance between the target and the line segment, which is drawn from 
			 the old position of the projectile to the current position. 
	Positive result is confirmed when the distance is less than the hit range. 
	*/
	if (targetPtr->isActive() && length(pos - targetPtr->getPos()) < PROJ_DETECT)
	{
		// Comes in detect range, measure the distance between the moving trace and target. 
		if(dist_point_to_line_seg(oldPos, pos, targetPtr->getPos(), pos) < hit_range)
			// hit the target, kill missile
			// Trigger explosion
			return PROJ_HIT;
	}

	// Collision detection with terrain
	if (terrainPtr->hitGround(pos, oldPos, pos, hitNormal))
	{
		if (pos.y >= SURFACE - 1) { return PROJ_COLLIDE_TERRAIN; }
		else { return PROJ_COLLIDE_WATER; }
	}
	else if (pos.y < SURFACE)
	{
		// move projectile back to collision point. 
 		GLfloat s = (oldPos.y - SURFACE) / (oldPos.y - pos.y); 
		pos = oldPos + velocity * s; 
		pos.y += 0.2;
		return PROJ_COLLIDE_WATER;
	}
	
	return PROJ_NORMAL;
}
