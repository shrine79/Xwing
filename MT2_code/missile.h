#ifndef _MISSILE_
#define _MISSILE_


#include "main.h"
#include "particle.h"
#include "primitive.h"
#include "lightHouse.h"
#include "terran.h"
#include "projectile.h"
#include "utility.h"
#include "glut.h"

#define MAX_MISSILE_SPD 6
#define INIT_MISSILE_SPD 3
#define MAX_MISSILE_ANGLE_SPD 15
#define MAX_MISSILE_ACC 0.2
#define MAX_MISSILE_LIFE 2000
#define MISSILE_LEN 2
#define MISSILE_RAD 0.2
#define MISSILE_HIT_RANGE 4
#define MISSILE_DAMAGE 5
#define MISSILE_LOCK_RANGE 120
#define MISSILE_LOCK_TIME 2000

class missile : public projectile
{
protected: 
	particleLauncher * partPtr; 

public:
	missile(tie * tiePtr, terrain * terPtr, GLint hit_range_v);
	~missile();
	void launch(particleLauncher * ptr); 
	int update();
	void reset();
	void render();
	
	GLboolean locked;
	inline const vec3& getPos();
	inline const vec3& getVel(); // get velocity
	inline GLboolean isActive();
};

const vec3& missile::getVel()
{
	return velocity;
}

const vec3& missile::getPos()
{
	return pos; 
}

GLboolean missile::isActive()
{
	return Active;
}

#endif // !_MISSILE_
