#ifndef _PROJECTILE_
#define _PROJECTILE_

#include "main.h"
#include "terran.h"
#include "tie.h"
#include "glut.h"


#define PROJ_INACTIVE 0
#define PROJ_NORMAL 1
#define PROJ_DIE 2
#define PROJ_COLLIDE_WATER 3
#define PROJ_COLLIDE_TERRAIN 4
#define PROJ_HIT 5
#define PROJ_DETECT 40
#define PROJ_LIFE 1000

class projectile
{
protected:
	int life, hit_range;
	GLfloat timeStamp;
	
	vec3 pos, velocity, oldPos;
	GLint time_interval;
	tie * targetPtr;
	terrain * terrainPtr;
	// Basic launch function. 
	virtual void launch(const int& life_v, const vec3& vel_v, const vec3& pos_v);

public:
	bool Active;
	vec3 hitPos; // the position where hit/collision occured. It's valid only after collision detection and before reset. 
	vec3 hitNormal; // the normal on the hit surface.  It's valid only after collision detection and before reset. 

	// The pure virtual function to be implemented in child class. 
	virtual void render() = 0;
	// reset internal status. 
	void reset(); 
	// Basic update
	int update();
	
	projectile(tie * tiePtr, terrain * terPtr, GLint hit_range_v);
	~projectile();
};

#endif // !_PROJECTILE_
