#ifndef _LASER_
#define _LASER_

#define LASER_LEN 6
#define LASER_WIDTH 5
#define LASER_INTERVAL 150
#define LASER_HIT_RANGE 4
#define LASER_MAX_LIFE 1000
#define LASER_SPD 20
#define LASER_DAMAGE 1
//#define LASER_SPD 25

#include "projectile.h"
#include "billboard.h"
#include "glut.h"


class laser : public projectile
{
protected:
	static billboard * bbPtr; 
	static GLint fireTimeStamp; 
	GLfloat headAngle_m; // laser head angle
	vec3 rotAxis_m; // the axis for the rotation above. 
	
public:
	static inline void setBillBorad(billboard * ptr); // set bill board instance for all laser. 
	
	virtual void render(); 
	laser(tie * tiePtr, terrain * terPtr, GLint hit_range_v);
	virtual GLboolean launch(const vec3& dir, const vec3& pos_v);
	inline vec3& getPos(); 
	virtual ~laser();
};

vec3& laser::getPos()
{
	return pos; 
}

void laser::setBillBorad(billboard * ptr)
{
	bbPtr = ptr;
}



#endif // !_LASER_
