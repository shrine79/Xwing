#ifndef _main_
#define _main_

#include "vec3.h"
#include "CubeClass.h"
#include "tringularPrism.h"
#include "billboard.h"
#include "glext.h"

#define MaxTexture 24
#define WorldSize 220
#define SURFACE 2.0f
#define HIGH_HEIGHT 15
#define GRASS_HEIGHT 3
#define HEIGHT_MARGIN 1.5
#define HEIGHT_SCALE 0.015
#define NEAR_PLANE 0.5
#define FAR_PLANE 300
//#define SUN_ORG_X 0
//#define SUN_ORG_Y -50
//#define SUN_ORG_Z -200
//#define SUN_AXIS_X 0
//#define SUN_AXIS_Y -0.2
//#define SUN_AXIS_Z -1
#define SUN_ORG_X  200
#define SUN_ORG_Y  0
#define SUN_ORG_Z  0
#define SUN_AXIS_X -0.5
#define SUN_AXIS_Y 1
#define SUN_AXIS_Z -0.1
#define SUN_START_POS_X 0
#define SUN_START_POS_Y 0
#define SUN_START_POS_Z 300
#define SUN_SIZE 70
#define MOON_SIZE 50
#define TREE_SIZE 5
#define PLANE_TO_EYE_DIST 8.0
#define PLANE_TO_EYE_DROP 1.5
//#define PLANE_TO_EYE_DIST 5.0
//#define PLANE_TO_EYE_DROP 0.5
#define MAX_MISSILE_NUM 5
#define MAX_PARTICLE_SYS 30
#define MAX_LASER_NUM 20
#define INI_WIN_HEIGHT 1000
#define INI_WIN_WIDTH 2000



extern CubeClass *cube; 
extern GLUquadricObj *quadric1;
extern GLUquadricObj *quadric2;
extern GLUquadricObj *quadric3;
extern GLUquadricObj *quadric4;
//extern lightHouse * lightHousePtr;

extern TriClass *triangle;
extern QuadClass *quad;
extern TriPrismClass *triPrism;

extern GLuint TexHandles[MaxTexture];

extern const GLfloat red[];
extern const GLfloat blue[];
extern const GLfloat green[];
extern const GLfloat cyan[];
extern const GLfloat magenta[];
extern const GLfloat yellow[];
extern const GLfloat brown[];
extern const GLfloat black[];
extern const GLfloat white[];

extern GLfloat global_amb[4];


extern int showWireFrame; 
extern int showTex;
extern int showNormals;
extern int hiddenCull;
extern int flatNormal;
extern int hiddenDepth;
extern int tessellation;
extern int ModulateTex;
extern int clockWise;
extern bool eLight;
extern float adjY;
extern int lerpMat;
extern GLfloat eyePoint[]; 
extern GLfloat lookAtPoint[];
extern GLfloat upVec[];
extern GLfloat camSpeed;
extern vec3 planePos;
extern vec3 BBpoints[];
extern int display_current_time; 
extern int last_time;
extern int displayTimestamp;
extern int display_time_interval;
extern billboard * smokePtr;
extern billboard * flamePtr;
extern GLfloat headAngle;
extern vec3 planeRotateAxis;
extern GLfloat rollAdjAngle;
extern vec3 lineOfSightPlane;
extern vec3 lineOfSight;
extern vec3 lineOfSightNorm;
extern GLuint radarLockTimer;
//extern particleLauncher partPool[];

//extern int ModualteTex;

extern PFNGLACTIVETEXTUREARBPROC glActiveTexture;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2f;
extern PFNGLBLENDFUNCSEPARATEINGRPROC glBlendFuncSeparate;
//extern PFNGLBLENDEQUATIONPROC glBlendEquation;

void moveToLookAt(GLfloat dist); 


#endif