#ifndef _PARTICLE_
#define _PARTICLE_


#include "billboard.h"
#include "lighting.h"
#include "glut.h"
#include "vec4.h"

#define MAX_PARTICLE 60

// random float value 0 to max (macro for speed)
//#define rndToMaxf(max) ((float)(rand()%(int)max))
#define rndToMaxf(max) (fmodf(static_cast<float>(rand()), max))

enum particle_type
{
	render_BB = 0,
	render_dot,
	render_line
};

class particle
{
public:
	particle();
	~particle();

	bool active;   // active Y or n
	int time;      // ticks of activity
	vec3 pos;      // position
	vec3 prevPos;  // prevPosition
	vec3 velosity; // how fast do i move
	vec4 colour;
	float pSize_x, pSize_y;   // size 
	float angle;  // the current orientation in degree
	float angleSpd; // the angle speed of spinning
	
	void renderDot(); 
	void renderLine();
	void renderTex(billboard * ptr); 
};

class particleLauncher
{
protected:
	/*********************************************************************************/
	/* type 0: particle affected by delta size,				 delta color			 */
	/* type 1: type 0 factor plus gravity and air resistence					     */
	/* type 2: type 1 factor plus spin and damp factor	            				 */
	/* type 3  type 2 factor plus wind and dieIfless							     */
	/*********************************************************************************/
	int typeOfPsystem;     
	int debugType; // type for debug purpose. 
	bool active;
	int activeticks;       // num of ticks for the particle system to be active -1 lasts forever
	bool particlesStillActive; // true if any emitted particle still active. 
	billboard * billPtr;    // billboard for type 2 systems
	particle_type p_type;   // the type of the particles to be drawn. could be BB, dot and line
	vec3 pos;               // location of start  
	vec3 orient;			// some particle system is not symetric and has its orientation. 
	GLfloat emissionRate;   // number of particles per tick (can be fractional)
	GLfloat emmisionRateRandom; // a max random number which we div by 100 and add to emision rate
	GLfloat residualEmmision; // how many to emit this turn - to allow for fractional emission rates
	int maxParticles;       // no more than this please
	int numParticles;       // the actual number 
	GLfloat particleSize_x, particleSize_y;   // size in points. Dot mode utilizes particleSize_x only. 
	GLfloat particleSizeDelta_x, particleSizeDelta_y;   // size in points change per tick, Dot mode utilizes particleSizeDelta_x only. 
	int max_ticks;          // after this many ticks die
	vec3 dieIfLess;      // kill the particl if its less than any of these values
	vec4 colour;     // particle color
	vec4 colourChange; // colour change + added each tick
	vec4 colourMax;  // 1,1,1 = white
	vec4 colourMin;  // min color; 
	vec3 gravity;    // velosity change due to gravity
	GLfloat size_gravity; // this gravity apples to the bill board size along -y axis
	vec3 wind;       // movement due to wind
	vec3 initialVelosity;
	vec3 initialVelosityRandom; // max random number which we dive by 100 and add to initial velosity 
	vec3 airResistance;    // how it slows always positive
	vec3 absTerminalVelosity; // max speed always positive
	GLfloat initialSpinAngle;  // initial angle in degrees
	GLfloat initialSpinAngleSpd; // initial angle spead of spinning in degrees
	GLfloat initialSpinAngleRandom; // initial random angle of spinning in degrees 
	GLfloat initialSpinAngleSpdRandom; // initial random angle spead of spinning in degrees
	GLfloat damp; // the damping factor to slow down change in size
	GLfloat spinDamp; // the damping factor to slow down spinning 
	bool firstTick; // indicate is the first tick of current active particle system
	bool notRand; // system doen't add randomness after 1st tick if this is true;
	int genCounter; // used to track how many particles has been generated in current tick. 

	// particle data
	particle p[MAX_PARTICLE];
	vec3 oldVel[MAX_PARTICLE];

	void drawDot();  // draw particle as dot
	void drawLine(); // draw particle as line segment
	void drawTex();  // draw particle as texture

	void updateSystem0(); // update system according to the influential factors in system type 0. 
	void updateSystem1(); // update system according to the influential factors in system type 1.
	void updateSystem2(); // update system according to the influential factors in system type 2.
	void updateSystem3(); // update system according to the influential factors in system type 3.

	void addRand(particle& p); //internal function to add randomness

public: 
	bool isActive(); 
	bool isParticleActive(); 
	inline void enable(); 
	inline void disable(); 
	void reset(); 
	void initSmokeSystem(billboard * bPtr); // Missile trace smoke
	void initSmokeSystem1(billboard * bPtr); // TIE dropping smoke
	void initSmokeSystem2(billboard * bPtr); // smoke at impact location 
	void initExplodeSystem1(billboard * bPtr, GLuint p_num, GLint wireMode, vec3 ori);  // explosion mode with particle dot and smoke. 
	void initExplodeSystem2(billboard * bPtr, GLuint p_num, GLint wireMode);			// explosion mode with flame
	void initExplodeSystem3(billboard * bPtr, GLuint p_num, GLint wireMode);			// explosion mode on ground with particle dot.
	void initExplodeSystem4(billboard * bPtr, GLuint p_num, vec3 ori);					// explosion mode for laser on target
	void initExplodeSystem5(billboard * bPtr, GLuint p_num, GLint wireMode);			// explosion mode for laser on water surface. 
	void initExplodeSystem6(billboard * bPtr, GLuint p_num, vec3 ori);					// explosion mode for laser on terrain
	void kill();  // kill active particle system
	void start(GLint tick); // start particle system
	void setPos(vec3 newPos); 
	void update();  // updatae particle system
	
	void drawPsystem(); // render all particles


	particleLauncher();
	~particleLauncher();
};

inline bool particleLauncher::isParticleActive()
{
	return particlesStillActive;
}

inline bool particleLauncher::isActive()
{
	return active; 
}

inline void particleLauncher::enable()
{
	active = TRUE; 
	firstTick = TRUE; 
}

inline void particleLauncher::disable()
{
	active = FALSE;
}


#endif