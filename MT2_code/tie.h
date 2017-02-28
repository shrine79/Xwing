#ifndef _TIE_
#define _TIE_

#define MAX_ROUTE_POINTS 20
#define MAX_TIE_ANGLE_SPD 40
#define BASIC_TIE_SPD 2.5
#define TIE_WING_SPAN 1.5f
#define TIE_L0 1.5f
#define TIE_L1 1.0f
#define TIE_L2 6.0f
#define TIE_W0 1.0f
#define TIE_W1 1.0f
#define TIE_WING_ANGLE 45
#define TIE_DROP_GRAVITY 0.2f
#define TIE_DROP_LIFE 1000
#define TIE_MAX_DROP_SPD 3
#define TIE_HP 10
#define TIE_LOCK_MARKER_INI_SIZE 300
#define TIE_LOCK_MARKER_END_SIZE 3
#define TIE_MARK_ANIMATION_TIME 500
#define TIE_MARK_ROT_SPD 0.3f
#define TIE_MARK_SCALE_SPD 0.6f

#include "vec3.h"
#include "quaternion.h"
#include "primitive.h"
#include "particle.h"
#include "terran.h"
#include "lighting.h"
#include "glUtils.h"
#include "glut.h"

class tie
{
protected: 
	GLboolean active; 
	GLboolean falling; 
	QuadClass centre_wing, side_wing; 

	// ideally it should be a single linked list from STD library. Keep it simple and efficient here with array
	vec3 routeMap[MAX_ROUTE_POINTS];  
	GLuint nextFreeSlot; // the route map counter 
	GLuint curRoutePoint; // the route point we are heading to
	GLuint lastRoutePoint; // the route point we come from
	GLint drop_counter; // kill the dropping TIE fighter when reaches zero
	GLint hitPoint; // the life of the figher
	GLboolean isLocked; // is the TIE fighter locked by radar
	GLfloat lockMarkerRot; // the current rotation of the lock marker
	GLfloat lockMarkerSize; // the current size of lock marker. 

	vec3 pos; // current position
	vec3 posOld; // last position
	vec3 orient; // current orientation
	vec3 velocity; // current velocity
	terrain * terPtr; 
	void renderWing(); // render wing compartment

	particleLauncher * partPtr; 

public:
	void drop(particleLauncher * p); // drop tie figher.
	void pushRoutePoint(const vec3& p); // push route point onto the route map
	void render(); // draw tie fighter
	GLboolean update(); // move tie figher, Return True if explodes. 
	GLboolean hit(GLint damage); // hit the fighter by missile
	vec3 getPos(); // get position
	vec3 getVel(); 
	inline bool isActive(); 
	void reset(); // re-spawn the tie fighter
	void enable(); // start tie fighter
	void endDrop(); 


	tie(terrain * tPtr);
	~tie();
};

inline bool tie::isActive()
{
	return active; 
}

#endif // !_TIE_

