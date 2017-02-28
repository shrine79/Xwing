
/****	First 3D program : the spinning cube
	Written by Rob Cox and Hugh Fisher				****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <random>
#include <time.h>

#include "glut.h"
#include "utility.h"
#include "glUtils.h"

#include "water.h"
#include "segment.h"
#include "hud.h"
#include "Axes.h"
#include "CubeClass.h"
#include "tringularPrism.h"
#include "2DText.h"
#include "grid.h"
#include "lighting.h"
#include "boat.h"
#include "lightHouse.h"
#include "primitive.h"
#include "skybox.h"
#include "fileOperations.h"
#include "quaternion.h"
#include "terran.h"
#include "xwing.h"
#include "laser.h"
#include "missile.h"
#include "tie.h"
#include "main.h"


#pragma warning(disable : 4996) // Reduce unsafe warning messages

#define framesPerSecond 60
#define timerMSecs ((int)1000/framesPerSecond)  // compute time in miliseconds for a frame
bool timeForNextFrame=false;

#define ESC	  27

#define cmdRed		 1
#define cmdGreen	 2
#define cmdExit		99

static int	WinWidth, WinHeight;
static int	AppMenu;

static GLfloat	spinAngle;
GLfloat spinIncrement=0.4f; // slow

GLfloat	eyePoint[3];
GLfloat	lookAtPoint[3];
GLfloat	upVec[3];//
static GLfloat  rolledDegree; // degree of angle the camera has rolled so far
static GLfloat  tiltedDegree;
static GLfloat	yawedDegree;
static GLfloat  fov; // field of view
static GLfloat	nearPlane;
static GLfloat	farPlane;


static GLfloat	color3[3];
static const GLfloat	red[] = { 1.0, 0.0, 0.0 };
static const GLfloat	blue[] = { 0.0, 1.0, 0.0 };
static const GLfloat	green[] = { 0.0, 0.0, 1.0 };
static const GLfloat	cyan[] = { 0.0, 1.0, 1.0 };
static const GLfloat	magenta[] = { 1.0, 0.0, 1.0 };
static const GLfloat	yellow[] = { 1.0, 1.0, 0.0 };
static const GLfloat	brown[] = { 0.6, 0.4, 0.0 };
static const GLfloat	black[] = { 0.0, 0.0, 0.0 };
static const GLfloat	white[] = { 1.0, 1.0, 1.0 };



PFNGLACTIVETEXTUREARBPROC glActiveTexture = NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2f = NULL;
PFNGLBLENDFUNCSEPARATEINGRPROC glBlendFuncSeparate = NULL;

segment *wharfSegment; 
boat * boatPtr; 
xwing * planePtr;
tie * tiePtr; 
missile * missilePtrs[MAX_MISSILE_NUM]; 
laser * laserPtrs[MAX_LASER_NUM];
CubeClass *cube; // pointer to a cube object
Grid *grid; // pointer to a cube object
water *waterPtr; // pointer to a water object
lightHouse * lightHousePtr; // pointer to a light house.
billboard * treePtr;
billboard * smokePtr;
billboard * flamePtr;
billboard * laserPtr;
billboard * waterBumpPtr; 
billboard * thrustFlamePtr; 
skybox * sbPtr; 
hud * hudPtr; 
terrain * terrainPtr; 
GLUquadricObj *quadric1;
GLUquadricObj *quadric2;  
GLUquadricObj *quadric3;    
GLUquadricObj *quadric4;    
Axes *axes; 
TriClass *triangle;
QuadClass *quad;
TriPrismClass *triPrism;

GLfloat boatSpeed = 0;
GLfloat boatx = -11;
GLfloat boatz = -6;
GLfloat boaty = 3.0;

GLfloat lightHouseX = 0;
GLfloat lightHouseY = 0;
GLfloat lightHouseZ = 0;

GLfloat sun_origin[3] = { SUN_ORG_X, SUN_ORG_Y, SUN_ORG_Z};
GLfloat sun_axis[3] = { SUN_AXIS_X, SUN_AXIS_Y, SUN_AXIS_Z };

GLfloat jettyX = 0; 
GLfloat jettyY = 0;
GLfloat jettyZ = 0;
GLfloat jettyAngle = -90;

GLuint TexHandles[MaxTexture];
GLfloat fog_color[] = {0.3, 0.45, 0.5, 1.0};
GLfloat fog_density = 0.3;
GLfloat water_color[] = { 0.1, 0.6, 0.7, 1.0 };
GLfloat water_density = 0.2 ;
GLfloat global_amb[4] = { 0, 0, 0, 1 };

// Laser fire sequence
GLint nextLaserGunToFire = 0;
vec3 debugPoint;  // debug 

// Plane orientation
GLfloat headAngle; 
GLfloat rollAdjAngle;
vec3 planeRotateAxis;

// input status for plane movement
enum controlKey
{
	up_arrow = 0,
	down_arrow,
	left_arrow,
	right_arrow,
	a_key,
	d_key,
	space_key,
	w_key,
	s_key,
	invalid_key
};

bool controlKeyDown[invalid_key];
GLuint radarLockTimer = 0;	// the target is consider locked by radar when counter reaches certain time

float adjY = 1; //debug
int frame_cnt = 0;
int display_current_time = 0;
int display_time_interval = 0; 
int last_time = 0;
int fps = 0;
GLfloat camSpeed = XWING_MIN_SPD;
int displayTimestamp = 0; 
vec3 planePos = vec3(0.0f); 
vec3 lineOfSightPlane = vec3(0.0f);
vec3 lineOfSight = vec3(0.0f);
vec3 lineOfSightNorm = vec3(0.0f);
vec3 BBpoints[8];

int showAxes=TRUE;
int showNormals=FALSE;
int showWireFrame=FALSE;
int showSpin=FALSE;
int showTex = TRUE;
int ModulateTex = TRUE;
int fog = TRUE;


int hiddenDepth=TRUE;
int hiddenCull=TRUE;
int clockWise=FALSE; // start CCW
int addLighting = FALSE;
int flatNormal = FALSE;
int tessellation = TRUE;
int cameraCntr = TRUE;
int freeTravel = FALSE; // the mode no collision detection, no x wing rendering. 
int lerpMat = TRUE;

GLfloat angle = 90;

int mode=5; // mode 2 = 2D, 3=3D , 5=Both
Text2D *drawText2d;
char dispString[255] = "2=2D (also F1-help)  3=3D  5=2D+3D";

// particle system pool 
particleLauncher partPool[MAX_PARTICLE_SYS]; 

// Function prototypes needed because they are used before its declared
void drawScene();



/****		Window events		****/

static void seteyePoint()
{
 	glLoadIdentity();

	// move eye point according to speed, line of sight and time elapsed
	//int temp = glutGet(GLUT_ELAPSED_TIME);
	//moveToLookAt((float)(( temp - displayTimestamp) / 1000) * camSpeed);
	moveToLookAt(camSpeed);
	gluLookAt(eyePoint[0], eyePoint[1], eyePoint[2],
		lookAtPoint[0],lookAtPoint[1],lookAtPoint[2],
		upVec[0],upVec[1],upVec[2]);
}

static void setProjection3D()
{
  GLfloat aspect;
  /* Set projection Mode */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  aspect = (float)WinWidth / (float)WinHeight;
  if (showWireFrame)
	  gluPerspective(fov, aspect, nearPlane, 2000);
  else
	  gluPerspective(fov, aspect, nearPlane, farPlane);

  /* Back to normal */
  glMatrixMode(GL_MODELVIEW);
  seteyePoint();
  
  if (hiddenDepth)
  {
	// Now do z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthRange(nearPlane,farPlane);
	glClearDepth(farPlane);			// Clear the depth buffer to far plane value. 
  }
  else
  {
	glDisable(GL_DEPTH_TEST);
  }

  if (hiddenCull)
  {
	  glEnable(GL_CULL_FACE);
	  glCullFace(GL_BACK);
	  if (clockWise) glFrontFace(GL_CW);
	  else glFrontFace(GL_CCW);
  }
  else
  {
	  glDisable(GL_CULL_FACE);
  }
}

static void setProjection2D()
{
  //GLfloat aspect;
  /* Set projection Mode */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //aspect = (float)WinWidth / (float)WinHeight;
  glOrtho(0, WinWidth, 0, WinHeight, 0, 1.0f);
  /* Back to normal but identity*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);
}


void setupLightHouse()
{
	if (eLight1) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);
	lightHousePtr->update();
}



void drawGrid()
{
	if (showTex)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D); 

	setMaterial(6);
	// put grid around origin
	glPushMatrix();
	glTranslatef(-0.5 * grid->getSizeX(), 0.0, -0.5 * grid->getSizeZ());

	if (showWireFrame)
		grid->renderLines(); 
	else
		grid->render();
	//grid->renderLines();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}



// Main code start here ------------------------------------------------------

void findGunPoints(vec3& p, int idx)
{
	GLfloat disp_x = XWING_WING_DISPLACE;
	disp_x *= cos(XWING_WING_ANGLE * M_PI / 180);
	GLfloat disp_y = XWING_WING_DISPLACE;
	disp_y *= sin(XWING_WING_ANGLE * M_PI / 180);

	// find gun points before rasterization. 
	switch (idx)
	{
	case 0:
		p = planePos + XWING_SCALE * (vec3(WING_POINT_X, WING_POINT_Y, WING_POINT_Z) + vec3(disp_x, disp_y, 0));
		break;
	case 1:
		p = planePos + XWING_SCALE * (vec3(WING_POINT_X, -WING_POINT_Y, WING_POINT_Z) + vec3(disp_x, -disp_y, 0));
		break;
	case 2:
		p = planePos + XWING_SCALE * (vec3(-WING_POINT_X, WING_POINT_Y, WING_POINT_Z) + vec3(-disp_x, disp_y, 0));
		break;
	case 3: 
		p = planePos + XWING_SCALE * (vec3(-WING_POINT_X, -WING_POINT_Y, WING_POINT_Z) + vec3(-disp_x, -disp_y, 0));
		break;
	default:
		printf("unexpected index of gun point: %d \n", idx); 
		break;
	}

	// Rotate gun point to align with plane head
	p = quaternionRotation(p, planePos, planeRotateAxis, headAngle);
	// Roll the gun points to align with UP vector. 
	p = quaternionRotation(p, planePos, -lineOfSightPlane, rollAdjAngle);
}




void yawCam(GLfloat degree)
{
	// the calculation is done in world space, not eye space. 
	// calculate the rotation axis
	vec3 rotateAxis = vec3(upVec);

	// make the lookAtPoint into a point quaternion where w = 0, i = x, j = y, k = z; 
	// translate rotation origin to eyepoint
	quaternion p;
	p.setVector(vec3(lookAtPoint) - vec3(eyePoint));

	// make the unit rotation quaternion
	quaternion quat = quaternion(degree, rotateAxis[0], rotateAxis[1], rotateAxis[2]);
	// build a conjugate of rotation quaternion
	quaternion quat_conj = quat.getConjugated();

	// calculate the lookAtPoint after the rotation about rotation axis
	quaternion pr = quat * p * quat_conj;

	// translate origin back to world origin
	vec3 new_lookAtPoint = pr.getVector() + vec3(eyePoint);

	// rotate the world
	lookAtPoint[0] = new_lookAtPoint.x;
	lookAtPoint[1] = new_lookAtPoint.y;
	lookAtPoint[2] = new_lookAtPoint.z;

	//yawedDegree += degree;
	//yawedDegree = fmod(yawedDegree, 360);
}

void rollCam(GLfloat degree)
{
	// the calculation is done in world space, not eye space. 

	// calculate the rotation axis
	vec3 rotateAxis = normalize(vec3(lookAtPoint) - vec3(eyePoint));

	// make the lookAtPoint into a point quaternion where w = 0, i = x, j = y, k = z; 
	// translate rotation origin to eyepoint
	quaternion p;
	p.setVector(vec3(upVec));

	// make the unit rotation quaternion
	quaternion quat = quaternion(degree, rotateAxis[0], rotateAxis[1], rotateAxis[2]);
	// build a conjugate of rotation quaternion
	quaternion quat_conj = quat.getConjugated();

	// calculate the lookAtPoint after the rotation about rotation axis
	quaternion pr = quat * p * quat_conj;

	// calculate the new up vector after the rotation
	vec3 new_upv = normalize(pr.getVector());
	upVec[0] = new_upv.x;
	upVec[1] = new_upv.y;
	upVec[2] = new_upv.z;

	//rolledDegree += degree; 
	//rolledDegree  = fmod(rolledDegree, 360);
}

void tiltCam(GLfloat degree) // camera look up and down. 
{
	// the calculation is done in world space, not eye space. 

	// calculate the rotation axis
	vec3 rotateAxis = normalize(cross(vec3(upVec), vec3(lookAtPoint) - vec3(eyePoint)));

	// make the lookAtPoint into a point quaternion where w = 0, i = x, j = y, k = z; 
	// translate rotation origin to eyepoint
	quaternion p;
	p.setVector(vec3(lookAtPoint) - vec3(eyePoint));

	// make the unit rotation quaternion
	quaternion quat = quaternion(degree, rotateAxis[0], rotateAxis[1], rotateAxis[2]);
	// build a conjugate of rotation quaternion
	quaternion quat_conj = quat.getConjugated(); 
	
	// calculate the lookAtPoint after the rotation about rotation axis
	quaternion pr = quat * p * quat_conj;

	// calculate the new up vector after the rotation
	vec3 upv = cross(vec3(lookAtPoint) - vec3(eyePoint) , rotateAxis);
	upv = normalize(upv); 
	upVec[0] = upv.x; 
	upVec[1] = upv.y;
	upVec[2] = upv.z;


	// translate origin back to world origin
	vec3 new_lookAtPoint = pr.getVector() + vec3(eyePoint); 
	// rotate the world
	lookAtPoint[0] = new_lookAtPoint.x;
	lookAtPoint[1] = new_lookAtPoint.y;
	lookAtPoint[2] = new_lookAtPoint.z;

	//tiltedDegree += degree;
	//tiltedDegree = fmod(tiltedDegree, 360);
}

void moveToLookAt(GLfloat dist)
{
	// move along the line of sight
	vec3 newEyePoint = vec3(eyePoint) + dist * lineOfSightNorm;  // change length 
	eyePoint[0] = newEyePoint.x;
	eyePoint[1] = newEyePoint.y;
	eyePoint[2] = newEyePoint.z;
	lookAtPoint[0] += lineOfSightNorm.x;
	lookAtPoint[1] += lineOfSightNorm.y;
	lookAtPoint[2] += lineOfSightNorm.z;
}

bool init = true;

void initScene() 
{

	treePtr = new billboard(TexHandles[10], semi_orient, bb_blend_mode::alpha,  bb_orientation::verticle, TREE_SIZE, TREE_SIZE);
	smokePtr = new billboard(TexHandles[15], omni_orient, bb_blend_mode::alpha, bb_orientation::verticle, 1, 1);
	flamePtr = new billboard(TexHandles[16], omni_orient, bb_blend_mode::alpha, bb_orientation::verticle, 1, 1);
	laserPtr = new billboard(TexHandles[17], rotated_multi_orient, bb_blend_mode::addictive, bb_orientation::horizental, LASER_WIDTH, LASER_LEN);
	waterBumpPtr = new billboard(TexHandles[18], multi_orient_with_end, bb_blend_mode::addictive, bb_orientation::verticle, 1, 1);
	waterBumpPtr->setMultiEnd(TexHandles[19]); 

	thrustFlamePtr = new billboard(TexHandles[22], multi_orient, bb_blend_mode::addictive, bb_orientation::horizental, 1, 1);

	/* Load terran height map here*/
	terrainPtr = new terrain("X20_23.txt", WorldSize, vec3(35, -1, 35), HEIGHT_SCALE, treePtr);
	
	cube = new CubeClass();
	grid = new Grid();
	waterPtr = new water();
	
	lightHousePtr = new lightHouse();
	wharfSegment = new segment();
	waterPtr->setUp(200, 200, 0.1, 5, 0.1, glutGet(GLUT_ELAPSED_TIME), 0.5, terrainPtr);
	grid->setUp(80, 80, 40, 40);

	boatPtr = new boat(); 
	planePtr = new xwing(thrustFlamePtr);
	//planePtr = new xwing(waterBumpPtr);
	
	// Load TIE fighter and route map
	tiePtr = new tie(terrainPtr);
	tiePtr->pushRoutePoint(vec3(-30, 100, -0));
	tiePtr->pushRoutePoint(vec3(-10,  80, -20));
	tiePtr->pushRoutePoint(vec3( 20,  45, -50));
	tiePtr->pushRoutePoint(vec3( 40,  30, -40));
	tiePtr->pushRoutePoint(vec3( 80,  55,  20));
	tiePtr->pushRoutePoint(vec3(100, 100,  80));
	tiePtr->pushRoutePoint(vec3( 90, 110,  150));
	tiePtr->pushRoutePoint(vec3( 20,  70,  150));
	tiePtr->pushRoutePoint(vec3(  0,  80,  70));
	tiePtr->enable();

	/* Up head display initialization */
	hudPtr = new hud(TexHandles[20], TexHandles[21], TexHandles[23], tiePtr);

	// initialize missile pool
	for (int i = 0; i < MAX_MISSILE_NUM; i++)
	{
		missilePtrs[i] = new missile(tiePtr, terrainPtr, MISSILE_HIT_RANGE);
	}

	for (int k = 0; k < MAX_LASER_NUM; k++)
	{
		laserPtrs[k] = new laser(tiePtr, terrainPtr, LASER_HIT_RANGE);
	}
	// set global laser billboar for all laser objects
	laser::setBillBorad(laserPtr); 

	// initialize control key status
	for (int j = 0; j < controlKey::invalid_key; j++)
	{
		controlKeyDown[j] = FALSE; 
	}

	//init=false;
}

void delScene()
{
	delete cube;
	delete grid;
	for (int i = 0; i < MAX_MISSILE_NUM; i++)
	{
		delete missilePtrs[i];
	}
}

void drawLightHouse()
{
	lightHousePtr->render();
}



void drawSkybox()
{
	if (showTex)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	// set far plane to be far enough to include sky box
	GLfloat aspect = (float)WinWidth / (float)WinHeight;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, nearPlane, 600);

	glMatrixMode(GL_MODELVIEW);
	sbPtr->render();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, nearPlane, farPlane);

	glMatrixMode(GL_MODELVIEW);

}

void drawTree()
{
	terrainPtr->renderTree();
}

void drawWater()
{
	glPushMatrix();
	setMaterial(5);
	waterPtr->update(glutGet(GLUT_ELAPSED_TIME));

	if (showWireFrame)
	{
		waterPtr->renderLines();/* Wireframe mode */
	}
	else
	{
		if (flatNormal)
			waterPtr->render(); /* fill mode, flat shading */
		else
			waterPtr->renderSmooth();  /* fill mode, smooth shading */
	}
	glPopMatrix();
}


void drawWharf()
{
	glPushMatrix();
	glTranslatef(7, 0 , 0); 

	/* Assemble wharf segments here*/
	wharfSegment->render();

	glPushMatrix();

	glTranslatef(0, 0, wharfSegment->length_v);
	wharfSegment->render();
	glTranslatef(0, 0, wharfSegment->length_v);
	wharfSegment->render();

	// Draw branch
	glPushMatrix();
	glTranslatef(-(wharfSegment->length_v + wharfSegment->width_v), 0.0, 0.0);

	glPushMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	wharfSegment->render();
	glPopMatrix();

	glTranslatef(wharfSegment->width_v, 0.0, wharfSegment->length_v);
	glPushMatrix();
	glRotatef(0, 0.0, 1.0, 0.0);
	wharfSegment->render();
	glPopMatrix();

	glTranslatef(0.0, 0.0, wharfSegment->length_v);
	wharfSegment->render();

	glPopMatrix();
	// End of branch


	glTranslatef(0, 0, wharfSegment->length_v);
	wharfSegment->render();
	glTranslatef(0, 0, wharfSegment->length_v);
	wharfSegment->render();

	glPopMatrix();
	glPopMatrix();

}

void drawLand()
{
	if (showTex)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3fv(blue);
	glTranslatef(0.0, 1.25, -1 * (wharfSegment->length_v + 5));
	glScalef(30.0, 3.5, 10.0);
	setMaterial(2);
	primitive::drawCubeWithNormals(TexHandles[1], 10); 
	glPopAttrib();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void drawTIE()
{
	tiePtr->render(); 
}

void drawPlane()
{
	// Calculate the line of sight. 
	lineOfSight = vec3(lookAtPoint) - vec3(eyePoint); // line of sight
	lineOfSightNorm = normalize(lineOfSight);

	// Calculate plane location by move it forward alone sight of line and down along the negative camera up vector. 
	planePos = vec3(eyePoint) + PLANE_TO_EYE_DIST * lineOfSightNorm;  // change length 
	planePos -= vec3(upVec) * PLANE_TO_EYE_DROP;

	// Calculate the line of sight from plane's perspective
	lineOfSightPlane = normalize(vec3(lookAtPoint) - planePos);

	// Calculat the normalized rotation axis for plane_head
	planeRotateAxis = normalize(cross(vec3(0, 0, -1), lineOfSightPlane));
	// Calculate the rotation angle between plane_head and -Z axis in degree. 
	headAngle = 180 * acos(dot(lineOfSightPlane, vec3(0, 0, -1))) / M_PI;
	// Calculate plane_up vector with same quaternion rotation. 
	vec3 planeUpVecNew = quaternionRotation(vec3(0, 1, 0), vec3(0, 0, 0), planeRotateAxis, headAngle);

	// Calculate the absolute value of angle between plane_up and camera_up vector in degree 
	rollAdjAngle = 180 * acos( (dot(vec3(upVec), normalize(planeUpVecNew)) ) ) / M_PI; 

	glPushMatrix();
		// Translate the origin to the calculated plane position. 
		glTranslatef(planePos.x, planePos.y, planePos.z);
		glRotatef(headAngle, planeRotateAxis.x, planeRotateAxis.y, planeRotateAxis.z);

		// acos() funciton returns a value in range of (0, pi). so we need to further determine the rotation axis to be Z axis or -Z axis
		vec3 adjAxis = normalize(cross(vec3(upVec), planeUpVecNew));
		// Align the plane up vector to camera up vector.
		//GLfloat tempV = dot(adjAxis, lineOfSightPlane);
		GLfloat tempV = dot(adjAxis, lineOfSightNorm);
		if (tempV < -0.001)
			//inverse roll adjustment angle
			rollAdjAngle = -rollAdjAngle;
		else if (tempV < 0.001)
			rollAdjAngle = 0; 

			glRotatef(rollAdjAngle, 0, 0, 1);

		planePtr->render(); 

	glPopMatrix();


	//Debug: draw camera up vector, plane up vector, and accmulated plane roll vector. 
	//glLineWidth(3);
	//glDisable(GL_LIGHTING); 
	//glBegin(GL_LINES);
	//glColor3f(1, 0, 0);
	//setMaterial(1);
	//glVertex3fv(planePos.fv());
	//glVertex3fv((planePos + 2 * vec3(upVec)).fv());   //the camera up vector

	//glColor3f(0, 1, 0); // green 
	//setMaterial(2);
	//glVertex3fv(planePos.fv());
	//glVertex3fv((planePos + 3 * rollPlusRot).fv());   // the plane model vector

	//setMaterial(3);
	//glColor3f(0, 0, 1); // blue
	//glVertex3fv(planePos.fv());
	//glVertex3fv((planePos + 5 * planeUpVecNew).fv());  // the world up vector
	//
	//glEnd();
	//if (eLight) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);
	
}


// Find the next free particle system.
// return a pointer to the free particle system
// return NULL pointer if failed to locate one
particleLauncher * findFreePartSys(particleLauncher * pool)
{
	particleLauncher * ptr = NULL;
	for (int j = 0; j < MAX_PARTICLE_SYS; j++)
	{
		if (!pool[j].isActive() && !pool[j].isParticleActive())
		{
			ptr = &(pool[j]);
			break;
		}
	}
	return ptr;
}

void fireLaser()
{
	// do nothing if max laser number reached
	for (int i = 0; i < MAX_LASER_NUM; i++)
	{
		if (!laserPtrs[i]->Active)
		{
			vec3 p;
			findGunPoints(p, nextLaserGunToFire);
			if(laserPtrs[i]->launch(lineOfSightPlane, p))
				nextLaserGunToFire = (nextLaserGunToFire + 1) % 4; 
		}
	}
}

void launchMissile()
{
	// do nothing if max missile number reached. 
	for (int i = 0; i < MAX_MISSILE_NUM; i++)
	{
		if (!missilePtrs[i]->isActive())
		{
			missilePtrs[i]->launch(findFreePartSys(partPool));
			break; 
		}
	}
}

void drawMissile()
{
	for (int i = 0; i < MAX_MISSILE_NUM; i++)
	{
		if (missilePtrs[i]->isActive())
		{
			missilePtrs[i]->render();
		}
	}
}

void drawParticle()
{
	for (int i = 0; i < MAX_PARTICLE_SYS; i++)
	{
		if (partPool[i].isParticleActive() )
		{
			// Draw particle
			partPool[i].drawPsystem();
		}
	}
}

void drawLaser()
{
	for (int i = 0; i < MAX_LASER_NUM; i++)
	{
		if (laserPtrs[i]->Active)
		{
			laserPtrs[i]->render();
		}
	}
}

void drawBoat()
{
	glColor3f(0.3, 0.3, 0.3);
	glPushMatrix();

	setMaterial(4);
	glScalef(0.2, 0.2, 0.2);
	boatPtr->renderWithNormals();


	glPopMatrix();	
	
}

void drawBoatAt(GLfloat x, GLfloat y, GLfloat z, GLfloat angle)
{
	GLfloat tempOffset = boatPtr->getCentreOffsetZ() / 5; // Adjust boat scalling factor
	glPushMatrix();
	glTranslatef(x, y ,z + tempOffset);
	glRotatef(angle, 0, 1, 0);	
	glTranslatef(0.0, 0.0, -tempOffset);
	drawBoat();
	glPopMatrix();
}


void drawTerrain()
{
	if(showWireFrame)
		terrainPtr->renderLines();
	else
	{
		if (flatNormal)
			terrainPtr->render();
		else
			terrainPtr->renderSmooth();
	}

}

void drawTarget()
{
	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(0, 50, -20);
	glScalef(2, 2, 2);
	primitive::drawSphere(GL_FLAT);
	glPopMatrix();
}

void drawScene1(vec3 origin, GLfloat angle)
{

	glPushMatrix();
	glTranslatef(origin.x, origin.y, origin.z);
	glRotatef(angle, 0, 1, 0);
	drawLand();
	drawLightHouse();
	drawWharf();
	glPopMatrix();

	glPushMatrix();
	
	drawWater();
	drawTree();
	glPopMatrix();

	//////debug: draw lookAtPoint as the last thing in the rendering
	//glPushMatrix();
	//glDisable(GL_DEPTH_TEST);
	//glColor3fv(red);

	//glTranslatef(lookAtPoint[0], lookAtPoint[1], lookAtPoint[2]);

	//glDisable(GL_TEXTURE_2D);
	//primitive::drawSphere(GL_FLAT);
	//glEnable(GL_DEPTH_TEST);
	//
	//glPopMatrix();

}



// Main Student code end here ------------------------------------------------------



void moveBoat()
{
	if (boatSpeed != 0)
	{
		rotate2D(boatx, boatz, angle, boatSpeed, &boatx, &boatz);
	}
}

void setFog(void)
{
	if (fog)
	{
		if (eyePoint[1] < SURFACE)
		{
			// underwater scene
			glFogi(GL_FOG_MODE, GL_EXP);
			glFogfv(GL_FOG_COLOR, water_color);
			glFogf(GL_FOG_DENSITY, water_density);
			glHint(GL_FOG_HINT, GL_DONT_CARE);
			glFogf(GL_FOG_START, nearPlane + 40);
			glFogf(GL_FOG_END, nearPlane + 60);
		}
		else
		{
			//aerial scene
			glFogi(GL_FOG_MODE, GL_LINEAR);
			glFogf(GL_FOG_DENSITY, fog_density);
			glHint(GL_FOG_HINT, GL_DONT_CARE);
			glFogf(GL_FOG_START, nearPlane + 60);
			glFogf(GL_FOG_END, nearPlane + 110);
			if (sbPtr->isNowDay()) { glFogfv(GL_FOG_COLOR, (vec3(global_amb)/2).fv()); }
			else { glFogfv(GL_FOG_COLOR, fog_color); }

		}
		

		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG); 
	}
}

void drawScene()
{
	//if (init) initScene();
	setupLightHouse();
	glPushMatrix();
	drawSkybox();
	setFog();
	drawBoatAt(boatx, boaty, boatz, angle);
	drawTerrain();
	drawScene1(vec3(jettyX, jettyY, jettyZ), jettyAngle);
	
	glDisable(GL_FOG); // Disable fog for air combat
	if (!freeTravel)
	{
		drawPlane();
		drawMissile();
		drawParticle();
		drawLaser(); 
	}

	drawTIE(); 
	glPopMatrix();


	//debug
	glDisable(GL_FOG);
	
	moveBoat();
}


static void drawWorld ()
{
	if (showWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* Wireframe mode */
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); /* fill mode */
	}

	glPushMatrix();

   if (showSpin)
	{
		glRotatef(spinAngle,0,1,0);
	}
  
	if (showAxes)
	  {
		  axes->render();
	  }
    if(lpos == 1)
		setupLights();	
	else 
	{
		vec3 tempv = sbPtr->getSunPos();
		float temp[4] = { tempv.x, tempv.y, tempv.z, 1.0f }; 
		setupLights(temp, sbPtr->isNowDay());
	}
		
	
    
    drawScene();

  glPopMatrix();

}

void draw2DStuff()
{
	glColor3f(1,1,0);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glRasterPos2f(30,60);
	

	drawText2d->drawstr(10, 620, "FPS %d", fps);
	drawText2d->drawstr(10, 590, "p - Camera Control Mode:  %s", cameraCntr? "ON":"OFF");
	drawText2d->drawstr(10, 560, "%s %8.2f,%8.2f,%8.2f, ","EyePoint",eyePoint[0],eyePoint[1],eyePoint[2]);
	drawText2d->drawstr(10, 530, "Free Travel Mode:  %s", freeTravel ? "ON" : "OFF");

	if (cameraCntr)
	{
		drawText2d->drawstr(10, 500, "CamSpeed %f", camSpeed);
		drawText2d->drawstr(10, 470, "Line of Sight : X = %8.2f, Y = %8.2f, Z = %8.2f,", lineOfSightNorm.x, lineOfSightNorm.y, lineOfSightNorm.z);
		//drawText2d->drawstr(10, 440, "look at point Position : X = %8.2f, Y = %8.2f, Z = %8.2f,", lookAtPoint[0], lookAtPoint[1], lookAtPoint[2]);
		hudPtr->render();
	}
	else 
	{
		drawText2d->drawstr(10, 500, "%s", "2 - 2D mode");
		drawText2d->drawstr(10, 470, "%s", "3 - 3D mode");
		drawText2d->drawstr(10, 440, "%s", "5 - 2D & 3D mode");
		drawText2d->drawstr(10, 410, "%s", "w - wire frame / fill");
		drawText2d->drawstr(10, 380, "%s", "g - go (move boat)");
		drawText2d->drawstr(10, 200, "%s", "Space = start / stop spin");
		drawText2d->drawstr(10, 170, "%s", "F2 - restore start view");
		drawText2d->drawstr(10, 110, "%s", "/ - spin speed");
		drawText2d->drawstr(10, 90, "%s", "+,- and = sets angle variable");
	}
	


	if(eLight) glEnable(GL_LIGHTING);
	if(showTex)glDisable(GL_TEXTURE_2D);
  }

static void display()
{
  display_current_time = glutGet(GLUT_ELAPSED_TIME);
  display_time_interval = display_current_time - displayTimestamp;

  if (hiddenDepth) {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}
  else {glClear(GL_COLOR_BUFFER_BIT);}

  if (ModulateTex)
	  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  else
	  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);


  if (mode == 3 || mode == 5)
	{
		setProjection3D();
		seteyePoint();

		//setupLights();
		drawWorld();

		
	}
   
  if (mode == 2 || mode == 5)
	{
		setProjection2D();
		draw2DStuff();
	}
  /* Check everything OK and update screen */
  CheckGL();
  glutSwapBuffers();
  frame_cnt++;
  
  if (display_current_time - last_time > 1000)
  {
	  fps = frame_cnt * 1000 / (display_current_time - last_time);
	  last_time = display_current_time;
	  frame_cnt = 0; 
  }

  displayTimestamp = display_current_time;
}

static void resize (int width, int height)
{
  /* Save for event handlers */
  WinWidth  = width;
  WinHeight = height;
  
  /* Reset view in window. */
  glViewport(0, 0, WinWidth, WinHeight);
}


/****		User events		****/


static void menuChoice (int item)
{
  switch (item) {
    case cmdRed:
        color3[0]=1; // R
        color3[1]=0; // G
        color3[2]=0; // B
      break;
    case cmdGreen:
        color3[0]=0; // R
        color3[1]=1; // G
        color3[2]=0; // B
      break;
    case cmdExit:
      exit(0);
      break;
    default:
      break;
  }
}



static void planeMovementHandler()
{
	/* These roll/yaw/rotate were implemented with quaternion rotation. 
	   Hence no Gimble lock involved in each step. 
	   
	   There is a coordinate system change between quaternion roll/yaw/rotate, 
	   Hence no Gimble lock involved in this function */
	
	// don't move camera if plane is down. 
	if(!freeTravel && !planePtr->active) return; 

	// Do rolling first 
	if (controlKeyDown[left_arrow]) rollCam(-2.5f); 
	if (controlKeyDown[right_arrow]) rollCam(2.5f);

	// Do tilt/pitch the next
	if (controlKeyDown[up_arrow]) tiltCam(3.0f);
	if (controlKeyDown[down_arrow]) tiltCam(-3.0f);

	// Do yawing the last
	if (controlKeyDown[d_key]) yawCam(-1.0f);
	if (controlKeyDown[a_key]) yawCam(1.0f);

	// update plane speed
	if (controlKeyDown[w_key]) camSpeed += 0.05;
	if (controlKeyDown[s_key]) camSpeed -= 0.05;
	camSpeed = min(camSpeed, XWING_MAX_SPD);
	camSpeed = max(camSpeed, XWING_MIN_SPD);
}


/* In most GUI systems we would write just one event handler
   for all kinds of keystrokes. In GLUT, we need one for the
   standard ASCII keys and one for special cursor or function
   style keys that vary from system to system. Because the
   GLUT special key code range overlaps with ASCII lowercase,
   it isn't safe to use the same function for both.        */

static void asciiKey (unsigned char key, int x, int y)
{
  if (key == ESC) menuChoice(cmdExit);
  if (key == 'p') { cameraCntr = !cameraCntr; }

  if (!cameraCntr)
  {
	  if (key == '2') { mode = 2; strcpy(dispString, "2D Mode"); }
	  if (key == '3') { mode = 3; strcpy(dispString, "3D Mode"); }
	  if (key == '5') { mode = 5; strcpy(dispString, "2D+3D Mode"); }

	  if (key == 'a') { showAxes = !showAxes; }
	  if (key == 'n') { showNormals = !showNormals; }
	  if (key == 'l') { eLight = !eLight; }
	  if (key == '7') { eLight0 = !eLight0; }
	  if (key == '8') { eLight1 = !eLight1; }
	  if (key == '6') { ambientLight = !ambientLight; }
	  if (key == '9') { lpos = 1; }
	  if (key == '0') { lpos = 0; }

	  if (key == 't') { showTex = !showTex; }
	  if (key == 'f') { fog = !fog; }
	  if (key == 'm') { ModulateTex = !ModulateTex; }
	  if (key == 'k') { tessellation = !tessellation; }

	  if (key == ' ') { showSpin = !showSpin; }
	  if (key == 'd') { hiddenDepth = !hiddenDepth; }
	  if (key == 's') { flatNormal = !flatNormal; }
	  if (key == 'w') { showWireFrame = !showWireFrame; }
	  if (key == '.') { eyePoint[0] = eyePoint[0] + 2; eyePoint[2] = eyePoint[2] + 2; }
	  if (key == ',') { eyePoint[0] = eyePoint[0] - 2; eyePoint[2] = eyePoint[2] - 2; }

	  if (key == 'c') { hiddenCull = !hiddenCull; }
	  if (key == 'i') { adjY += 0.3; }
	  if (key == 'o') { adjY -= 0.3; }
	  if (key == '@') { clockWise = !clockWise; }
	  if (key == '/') { spinIncrement = spinIncrement + 1; if (spinIncrement > 4) { spinIncrement = 0.4f; } }
	  

	  if (key == '=') { if (angle < 50) angle = 90; else angle = 0; }
	  if (key == '+') { angle = angle + 2; angle = (GLfloat)((int)angle % 360); }
	  if (key == '-') { angle = angle - 2; angle = (GLfloat)((int)angle % 360); }
	  if (key == 'g') { if (boatSpeed == 0) boatSpeed = -0.2; else boatSpeed = 0; }
	  if (key == 'x') { lerpMat = !lerpMat; }
  }
  else
  {
	  if (key == '0' && !freeTravel) { launchMissile(); }
	  if (key == 'w') { controlKeyDown[w_key] = TRUE;}
	  if (key == 's') { controlKeyDown[s_key] = TRUE; }
	  if (key == ' ') { controlKeyDown[space_key] = TRUE;}
	  if (key == 'd') controlKeyDown[d_key] = TRUE; 
	  if (key == 'a') controlKeyDown[a_key] = TRUE;

	  if (key == 'r') { if (!tiePtr->isActive()) { tiePtr->reset(); tiePtr->enable(); } }

	  if (key == 'f') { freeTravel = !freeTravel; }
  }
}

static void asciiKeyUpHandler(unsigned char key, int x, int y)
{
	if (cameraCntr)
	{
		if (key == 'd') controlKeyDown[d_key] = FALSE;
		if (key == 'a') controlKeyDown[a_key] = FALSE;
		if (key == 's') controlKeyDown[s_key] = FALSE;
		if (key == 'w') controlKeyDown[w_key] = FALSE;
		if (key == ' ') { controlKeyDown[space_key] = FALSE; }
	}
}

static void specialKeyUpHandler(int key, int x, int y)
{
	if (cameraCntr)
	{
		if (key == GLUT_KEY_LEFT) controlKeyDown[left_arrow] = FALSE;
		if (key == GLUT_KEY_RIGHT) controlKeyDown[right_arrow] = FALSE;
		if (key == GLUT_KEY_UP) controlKeyDown[up_arrow] = FALSE;
		if (key == GLUT_KEY_DOWN) controlKeyDown[down_arrow] = FALSE;
	}
}


void setStartValues()
{
  eyePoint[0] = 0.0;
  eyePoint[1] = 50.0;
  eyePoint[2] = 50.0;
  
  lookAtPoint[0]=0;
  lookAtPoint[1]=0;
  lookAtPoint[2]=0;

  lineOfSight = vec3(lookAtPoint) - vec3(eyePoint);

  vec3 upv = cross(vec3(lookAtPoint) - vec3(eyePoint), vec3(-1, 0, 0)); 

  upv = normalize(upv); 
  upVec[0]= upv.x;
  upVec[1]= upv.y;
  upVec[2]= upv.z;

  rolledDegree = 0; 

  fov=60; // field of view
  nearPlane=NEAR_PLANE;
  farPlane=FAR_PLANE;

  spinAngle = 0.0;

  lpos = 0; 

  planePos = vec3(eyePoint); 
}

static void specialKey (int key, int x, int y)
{
  
  if (key==GLUT_KEY_F1) {mode = 2;}

  if (key==GLUT_KEY_F2)
  {
  setStartValues();
  }

  if (cameraCntr)
  {
	  if (key == GLUT_KEY_LEFT) controlKeyDown[left_arrow] = TRUE;
	  if (key == GLUT_KEY_RIGHT) controlKeyDown[right_arrow] = TRUE;
	  if (key == GLUT_KEY_UP) controlKeyDown[up_arrow] = TRUE;
	  if (key == GLUT_KEY_DOWN) controlKeyDown[down_arrow] = TRUE;

	  
	  
  }
  else
  {
	  if (key == GLUT_KEY_LEFT) eyePoint[0] = eyePoint[0] + 2;
	  if (key == GLUT_KEY_RIGHT) eyePoint[0] = eyePoint[0] - 2;
	  if (key == GLUT_KEY_UP) eyePoint[2] = eyePoint[2] - 2;
	  if (key == GLUT_KEY_DOWN) eyePoint[2] = eyePoint[2] + 2;
	  if (key == GLUT_KEY_PAGE_UP) eyePoint[1] = eyePoint[1] + 2;
	  if (key == GLUT_KEY_PAGE_DOWN) eyePoint[1] = eyePoint[1] - 2;
  }


  

}

void exitFunction(void)
{
  //delete cube;
  //delete boatFront1;
  //delete boatFront2;
  //delete boatRear;
  //delete boatPtr;
  delete axes;
  delete drawText2d; 
  delete triangle;
  delete quad;
  delete triPrism;
  delScene(); 
  gluDeleteQuadric(quadric1);
  gluDeleteQuadric(quadric2);
  gluDeleteQuadric(quadric3);
  gluDeleteQuadric(quadric4);
//  gluDeleteQuadric(quadric2);
}


/****		Startup			****/
static void initEnv(void)
{
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
}


static void initGraphics (void)
{
  // Using wgl to get and save gl extension API function address. 
  glActiveTexture = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTexture"); 
  glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2f"); 
  glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEINGRPROC) wglGetProcAddress("glBlendFuncSeparate");
  //glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation");
  /* Black background */
  glClearColor(0, 0, 0, 0);

  /* colour green */
  color3[0]=0;
  color3[0]=1;
  color3[0]=0;

 
  glEnable(GL_DEPTH_TEST);

  /* Needed for vertex arrays */
  //glEnableClientState(GL_VERTEX_ARRAY);
  
  /* Popup menu attached to right mouse button */
  AppMenu = glutCreateMenu(menuChoice);
  glutSetMenu(AppMenu);
  glutAddMenuEntry("Red", cmdRed);
  glutAddMenuEntry("Green", cmdGreen);
  glutAddMenuEntry("----", 0);
  glutAddMenuEntry("Exit", cmdExit);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  
  /* Start values */
   setStartValues();

   glEnable(GL_TEXTURE_2D);

   
   glGenTextures(MaxTexture - 1, TexHandles);

   /* Load textures and bind to handles */
   getBMPTexture(findFile("wood_texture.bmp"), TexHandles, GL_TRUE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("brickwork_texture.bmp"), TexHandles + 1, GL_FALSE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("oak.bmp"), TexHandles + 2, GL_FALSE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("canvas.bmp"), TexHandles + 3, GL_FALSE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("pirate.bmp"), TexHandles + 4, GL_TRUE, GL_RGBA, GL_BGRA_EXT);
   getBMPTexture(findFile("water.bmp"), TexHandles + 5, GL_FALSE, GL_RGBA, GL_BGRA_EXT);
   getBMPTexture(findFile("concrete.bmp"), TexHandles + 6, GL_TRUE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("test1.bmp"), TexHandles + 7, GL_TRUE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("new_sun.bmp"), TexHandles + 8, GL_TRUE, GL_RGBA, GL_BGRA_EXT);
   getBMPTexture(findFile("moon.bmp"), TexHandles + 9, GL_TRUE, GL_RGBA, GL_BGRA_EXT);
   getBMPTexture(findFile("pine.bmp"), TexHandles + 10, GL_TRUE, GL_RGBA, GL_BGRA_EXT);
   getBMPTexture(findFile("spaceship.bmp"), TexHandles + 11, GL_TRUE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("spacemetal.bmp"), TexHandles + 12, GL_TRUE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("black.bmp"), TexHandles + 13, GL_TRUE, GL_RGB, GL_BGR_EXT);
   //getBMPTexture(findFile("jetBurn.bmp"), TexHandles + 14, GL_TRUE, GL_RGBA, GL_BGRA_EXT);
   getBMPTexture(findFile("thrustDisc.bmp"), TexHandles + 14, GL_TRUE, GL_RGBA, GL_BGRA_EXT);
   getBMPTexture(findFile("smoke.bmp"), TexHandles + 15, GL_TRUE, GL_RGBA, GL_BGRA_EXT);
   getBMPTexture(findFile("flame.bmp"), TexHandles + 16, GL_TRUE, GL_RGBA, GL_BGRA_EXT);
   getBMPTexture(findFile("laser_orange.bmp"), TexHandles + 17, GL_TRUE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("water_bump.bmp"), TexHandles + 18, GL_TRUE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("water_bump_bot.bmp"), TexHandles + 19, GL_TRUE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("crosshair.bmp"), TexHandles + 20, GL_TRUE, GL_RGB, GL_BGR_EXT);
   getBMPTexture(findFile("compass.bmp"), TexHandles + 21, GL_TRUE, GL_RGB, GL_RGB);
   getBMPTexture(findFile("thrustFlame.bmp"), TexHandles + 22, GL_TRUE, GL_RGBA, GL_BGRA_EXT);
   getBMPTexture(findFile("locking_circle.bmp"), TexHandles + 23, GL_TRUE, GL_RGBA, GL_BGRA_EXT);
   //getBMPTrans(findFile("laser.bmp"), TexHandles + 17);

   // instantiate skybox here before set lighting conditions
   sbPtr = new skybox(TexHandles[7], TexHandles[8], TexHandles[9], vec3(sun_axis), vec3(sun_origin));


   

  menuChoice(cmdGreen);

  //cube = new CubeClass();
  triangle = new TriClass();
  quad = new QuadClass();
  triPrism = new TriPrismClass(1,2,3);
  axes = new Axes();
  drawText2d = new Text2D();
  quadric1=gluNewQuadric();
  quadric2=gluNewQuadric();
  quadric3=gluNewQuadric();
  quadric4=gluNewQuadric();

  atexit (exitFunction);
}


void timer(int value)
{
timeForNextFrame=true;
glutTimerFunc(timerMSecs,timer,0);
}

/****		Main control		****/


static void idleRoutine(void)
{
  if (timeForNextFrame)
  {
	  spinAngle += spinIncrement;
	if (spinAngle > 360.0)
			spinAngle -= 360.0;

	/**************** missile update *********************/
	for (int i = 0; i < MAX_MISSILE_NUM; i++)
	{
		int tempI = missilePtrs[i]->update();
		if (tempI > PROJ_NORMAL)
		{
			if (tempI > PROJ_DIE)
			{
				// prepare to launch particle system
				particleLauncher * pPtr = findFreePartSys(partPool);
				if (tempI > PROJ_COLLIDE_WATER)
				{
					if (tempI == PROJ_COLLIDE_TERRAIN)
					{
						// The misslie has collided with terrain explode
						pPtr->initExplodeSystem1(flamePtr, 6, showWireFrame, vec3(0, 1, 0));
					}
					else
						// The misslie has collided with target explode
					{
						pPtr->initExplodeSystem1(flamePtr, 6, showWireFrame, normalize(missilePtrs[i]->getVel()));
					}
					pPtr->setPos(missilePtrs[i]->getPos());
					pPtr->enable(); // take the particle system 

					pPtr = findFreePartSys(partPool);
					pPtr->initExplodeSystem2(flamePtr, 6, showWireFrame);
					pPtr->setPos(missilePtrs[i]->getPos());
					pPtr->enable(); // take the particle system 
				}
				else 
				{
					// The misslie has hit water surface, draw water bump
				}
				
				// Hit the TIE fighter
				if (tempI == PROJ_HIT && tiePtr->hit(MISSILE_DAMAGE))
				{
					// Add smoke and drop the plane
					pPtr = findFreePartSys(partPool);
					pPtr->initSmokeSystem1(flamePtr);
					pPtr->setPos(tiePtr->getPos());
					pPtr->enable();
					tiePtr->drop(pPtr);
				}
			}

			// Reset missile if not in normal state
			missilePtrs[i]->reset();
		}
	}

	/*************** particle update ********************/
	for (int j = 0; j < MAX_PARTICLE_SYS; j++)
	{
	 partPool[j].update();

	}

	/*********** Laser update ************/
	for (int k = 0; k < MAX_LASER_NUM; k++)
	{
		int tempI = laserPtrs[k]->update();

		if (tempI > PROJ_NORMAL)
		{
			if (tempI > PROJ_DIE)
			{
				particleLauncher * pPtr = findFreePartSys(partPool);
				if (tempI == PROJ_COLLIDE_WATER)
				{
					// The laser hits the water surface
					pPtr->initExplodeSystem5(waterBumpPtr, 1, showWireFrame);
					vec3 hitPos = laserPtrs[k]->getPos(); 
					hitPos.y = SURFACE; 
					pPtr->setPos(laserPtrs[k]->getPos());
					pPtr->enable(); // take the particle system 
				}
				else if (tempI == PROJ_COLLIDE_TERRAIN)
				{
					// The laser has collided with terrain, show ending effects
					pPtr->initExplodeSystem6(flamePtr, 20, laserPtrs[k]->hitNormal);
					pPtr->setPos(laserPtrs[k]->getPos());
					pPtr->enable(); // take the particle system 
				}
				else
				{
					// The laser has collided with target, show ending effects
					pPtr->initExplodeSystem4(flamePtr, 20, lineOfSightPlane);
					pPtr->setPos(laserPtrs[k]->getPos());
					pPtr->enable(); // take the particle system 
				}

				if (tempI == PROJ_HIT && tiePtr->hit(LASER_DAMAGE))
				{
					// Add smoke and drop the plane
					pPtr = findFreePartSys(partPool);
					pPtr->initSmokeSystem1(flamePtr);
					pPtr->setPos(tiePtr->getPos());
					pPtr->enable();
					tiePtr->drop(pPtr);
				}
			}
			// Reset laser if not in normal state
			laserPtrs[k]->reset(); 
		}
	}


	/**********Tie Update ************/
	if (tiePtr->update())
	{
		// if TIE hits ground, explode. 
		particleLauncher * pPtr = findFreePartSys(partPool);

		// explosion particle on ground
		pPtr->initExplodeSystem3(flamePtr, 12, showWireFrame);
		pPtr->setPos(tiePtr->getPos());
		pPtr->enable(); // take the particle system 

		// explosion smoke on ground
		pPtr = findFreePartSys(partPool);
		pPtr->initExplodeSystem2(flamePtr, 6, showWireFrame);
		pPtr->setPos(tiePtr->getPos());
		pPtr->enable(); // take the particle system 

		// generate smoke on the location of impact for a while
		pPtr = findFreePartSys(partPool);
		pPtr->initSmokeSystem2(flamePtr);
		pPtr->setPos(tiePtr->getPos());
		pPtr->enable(); // take the particle system 

	}
		
	/**************** Xwing update **********************/
	// responde to input
	planeMovementHandler();

	// NO collision detection in Free Travel mode
	if (!freeTravel)
	{
		// collision detection 
		if (planePtr->active && terrainPtr->boxHitGround(planePos, 3, 2, 4))
		{
			// if Xwing hits ground, explode. 
			particleLauncher * pPtr = findFreePartSys(partPool);

			pPtr->initExplodeSystem3(flamePtr, 12, showWireFrame);
			pPtr->setPos(planePos);
			pPtr->enable(); // take the particle system 

			pPtr = findFreePartSys(partPool);
			pPtr->initExplodeSystem2(flamePtr, 6, showWireFrame);
			pPtr->setPos(planePos);
			pPtr->enable(); // take the particle system 

							// render x-wing inactive
			planePtr->active = FALSE;
			// stop camera
			camSpeed = 0;
		}
	}



	/************** fire laser canonn ***************/
	// if SHIFT key is hold down 
	if (controlKeyDown[space_key])
	{
		// Trying to fire a pulse laser beam if possible
		fireLaser();
	}


	glutPostRedisplay();
	timeForNextFrame=false;

  }

}

int main (int argc, char * argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

  glutInitWindowSize(INI_WIN_WIDTH, INI_WIN_HEIGHT);
  glutInitWindowPosition(50, 50);
  glutCreateWindow("Cube");

  initEnv(); 
  initGraphics();
  srand(time(0));	// seed randomness with current system time.
  initScene();

  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  
  glutKeyboardFunc(asciiKey);
  glutKeyboardUpFunc(asciiKeyUpHandler); 

  glutSpecialFunc(specialKey);
  glutSpecialUpFunc(specialKeyUpHandler); 
  
  glutIdleFunc(idleRoutine);
  glutTimerFunc(timerMSecs,timer,0);
  
  glutMainLoop();
  /* Should never get here, but keeps compiler happy */
  return 0;
}
