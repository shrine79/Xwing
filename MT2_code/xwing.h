#ifndef _X_WING_
#define _X_WING_

#include "vec3.h"
#include "lighting.h"
#include "tringularPrism.h"
#include "primitive.h"
#include "main.h"
#include "glut.h"

#define LEN0 2.0
#define LEN1 11.0
#define LEN2 9.0
#define LEN3 17.0
#define WING_POINT_X 4.0
#define WING_POINT_Y 2.5
#define WING_POINT_Z 13.0
#define ENGINE_RAD 2.0
#define ENGINE_LEN 10
#define WING_W0 10
#define WING_W1 5
#define WING_LEN 20.0
#define WING_THICK 0.3
#define GUN_LEN 14
#define GUN_RAD 0.4
#define COCKPIT_LEN0 5
#define COCKPIT_LEN1 2.5
#define COCKPIT_LEN2 16
#define JET_LEN 7
#define JET_RAD 1.3
#define XWING_SCALE 0.08
#define XWING_WING_ANGLE 15.0
#define XWING_WING_DISPLACE WING_LEN + ENGINE_RAD + GUN_RAD
#define XWING_THRUST_DISC_NUM 12
#define XWING_MAX_SPD 1
#define XWING_MIN_SPD 0.1


class xwing
{
protected:
	vec3 wingPoint;
	QuadPrismClass * s1Ptr, *s2Ptr, *s3Ptr, *s4Ptr, *s5Ptr, *s6Ptr, *s7Ptr, *wingPtr;
	QuadClass * flamePtr; 
	billboard * thrustPtr;
	GLfloat s0[4], s1[4], s2[4], s3[4], s4[4], s5[4], s6[4], s7[4], s8[4], ws0[4], ws1[4];
	GLfloat flameLen; 
	GLdouble thrustDiscOffset; // the offset to impelment the move of thrust disc. 
	void makeBody();
	QuadPrismClass* makePrism(GLfloat* front, GLfloat* back);
	QuadPrismClass* makeSingleSidePrism(GLfloat* front, GLfloat* back);
	void renderWingMod(); 
	void renderFlame(const vec3& pos);
public:
	GLboolean active; 

	void render(); 
	xwing(billboard * ptr);
	~xwing();
};

#endif
