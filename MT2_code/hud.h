#ifndef _HUD_
#define _HUD_
#define HUD_FRUSTUM_DIST 10
#define HUD_CROSSHAIR_SIZE 50
#define HUD_COMPASS_SIZE 400
#define HUD_LOCKING_CIRCLE_SIZE (HUD_COMPASS_SIZE - 80)
#define HUD_UP_INDICATOR_OFFSET (HUD_COMPASS_SIZE - 30)
#define HUD_TARGET_INDICATOR_OFFSET (HUD_COMPASS_SIZE - 50)
#define HUD_LOCKING_CIRCLE_SPD 0.1f


#include "billboard.h"
#include "tie.h"
#include "vec3.h"

/* The HUD(Head UP Display) class for xwing */ 
class hud
{
protected: 
	vec3 crossHairPos; 
	tie * targetPtr; 
	GLuint crossHairHandle;  // the cross hair texture for laser gun in the centre
	GLuint compassHandle; // the compass texture
	GLuint lockingCircleHandle; // the radar locking circle. 
	GLfloat lockingCircleRot; // the angle of rotation for locking circle. 

	void setup2D();
	void renderCrossHair(); 
	void renderCompass(); 
	void renderTargetIndicator(); 
public:
	hud(GLuint chPtr, GLuint cpPtr, GLuint lcPtr, tie * tPtr);
	~hud();

	void render(); 
};

#endif // !_HUD_
