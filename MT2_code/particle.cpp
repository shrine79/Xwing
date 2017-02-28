#include "particle.h"




particleLauncher::particleLauncher()
{
	reset();
}


particleLauncher::~particleLauncher()
{
}

void particleLauncher::reset()
{
	active = false;
	particlesStillActive = FALSE;
	activeticks = -1;
	typeOfPsystem = 0;       // future expansion
	debugType = 0;
	pos =  vec3(0, 0, 0);	// default position
	emissionRate = 1;      // number of particles per tick (can be fractional)
	emmisionRateRandom = 0;  // a max random number which we div by 100 and add to emision rate
	residualEmmision = 0;
	maxParticles = MAX_PARTICLE; // no more than this please
	numParticles = 0;        // the actual number 
	particleSize_x = 3;        // size in points 
	particleSize_y = 3;        // size in points 
	particleSizeDelta_x = 0;   // no change
	particleSizeDelta_y = 0;   // no change
	max_ticks = 120;         // after this many ticks die
	dieIfLess = vec3(-1, 0, -1);      // kill the particl if its less than any of these values
	v4FloatToVecM(colour, 0.1, 0.8, 0.1, 1); // particle color       
	v4FloatToVecM(colourChange, 0.06, 0.06, 0.06, 0);   // colour drift + added each tick
	v4FloatToVecM(colourMax, 1, 1, 1, 1);
	gravity = vec3(0, -0.02, 0);    // velosity change due to gravity
	size_gravity = 0; // size gravity on -y axis
	v3FloatToVec(0, 0, 0, wind);           // movement due to wind
	v3FloatToVec(0, 0, 0, initialVelosity);
	v3FloatToVec(5, 5, 5, initialVelosityRandom); // max random number which we dive by 100 and add to initial velosity 
	v3FloatToVec(0, 0, 0, airResistance);       // how it slows
	v3FloatToVec(5, 5, 5, absTerminalVelosity);    // max speed

	initialSpinAngle = 0;  // initial angle in degrees
	initialSpinAngleSpd = 0; // initial angle spead of spinning in degrees
	initialSpinAngleRandom = 0; // initial random angle of spinning in degrees 
	initialSpinAngleSpdRandom = 0; // initial random angle spead of spinning in degrees
	damp = 1.0; // the damping factor to slow down change in size
	spinDamp = 1.0; // the damping factor to slow down change in spinning
	notRand = FALSE; //system doen't add randomness after 1st tick if this is true;

	for (int i = 0; i< MAX_PARTICLE; i++)
	{
		p[i].active = false;   // active Y or n
		p[i].time = 0;      // ticks of activity
		v3FloatToVec(0, 0, 0, p[i].pos);      // position
		v3FloatToVec(0, 0, 0, p[i].velosity); // how fast do i move
		oldVel[i] = vec3(0.0f); // the velocity in the past.  
	}
}


void particleLauncher::initSmokeSystem(billboard * bPtr)
{
	typeOfPsystem = 2;	//64 x 64 bmp with alpha
	notRand = FALSE;
	emissionRate = 1.5;
	maxParticles = 50;
	emmisionRateRandom = 30;
	particleSize_x = 1;         // size in points 
	particleSize_y = 1;         // size in points 
	particleSizeDelta_x = 0.3;   // size change
	particleSizeDelta_y = 0.3;   // size change
	size_gravity = 0;
	billPtr = bPtr;
	gravity = vec3(0, 0, 0); 
	dieIfLess = vec3(-1000, -1, -1000); 
	wind = vec3(0, 0, 0); 
	activeticks = 80; 
	max_ticks = 15;
	p_type = render_BB;
	initialVelosityRandom = vec3(3, 3, 3); 
	initialVelosity = vec3(0, 0, 0); 
	initialSpinAngle = 0;
	initialSpinAngleSpd = 200;
	initialSpinAngleSpdRandom = 200;
	v4FloatToVecM(colour, 1, 0.5, 0.3, 1);
	v4FloatToVecM(colourChange, 0.0, 0.03, 0.06, -0.066);
	v4FloatToVecM(colourMax, 1, 1, 1, 1);
	airResistance = vec3(0, 0, 0); 
	gravity = vec3(0, 0, 0);
}

void particleLauncher::initSmokeSystem1(billboard * bPtr)
{
	typeOfPsystem = 1;	//64 x 64 bmp with alpha
	notRand = FALSE;
	emissionRate = 2;
	maxParticles = 60;
	emmisionRateRandom = 30;
	particleSize_x = 1.5;         // size in points 
	particleSize_y = 1.5;         // size in points 
	particleSizeDelta_x = 0.5;   // size change
	particleSizeDelta_y = 0.5;   // size change
	size_gravity = 0;
	billPtr = bPtr;
	gravity = vec3(0, 0, 0);
	dieIfLess = vec3(-1000, -1, -1000);
	wind = vec3(0, 0, 0);
	activeticks = -1;
	max_ticks = 40;
	p_type = render_BB;
	initialVelosityRandom = vec3(5, 5, 5);
	initialVelosity = vec3(0, 0, 0);
	v4FloatToVecM(colour, 1, 1, 1, 1);
	v4FloatToVecM(colourChange, -0.02, -0.05, -0.1, -0.04);
	v4FloatToVecM(colourMax, 0.8, 0.5, 0.1, 0.4);
	airResistance = vec3(0, 0, 0);
	gravity = vec3(0, 0, 0);
}

void particleLauncher::initSmokeSystem2(billboard * bPtr)
{
	typeOfPsystem = 1;	//64 x 64 bmp with alpha
	notRand = FALSE;
	emissionRate = 0.5;
	maxParticles = 50;
	emmisionRateRandom = 50;
	particleSize_x = 4;         // size in points 
	particleSize_y = 4;         // size in points 
	particleSizeDelta_x = 1;   // size change
	particleSizeDelta_y = 1;   // size change
	size_gravity = 0;
	billPtr = bPtr;
	gravity = vec3(0, 0, 0);
	dieIfLess = vec3(-1000, -1, -1000);
	wind = vec3(0, 0, 0);
	activeticks = 100;
	max_ticks = 60;
	p_type = render_BB;
	initialVelosityRandom = vec3(60, 10, 60);
	initialVelosity = vec3(0, 1.2, 0);
	v4FloatToVecM(colour, 1, 1, 1, 1);
	v4FloatToVecM(colourChange, -0.02, -0.05, -0.1, -0.02);
	v4FloatToVecM(colourMax, 0.5, 0.3, 0.1, 0.4);
	airResistance = vec3(0.1, 0.02 , 0.1);
	gravity = vec3(0, 0, 0);
}

void particleLauncher::initExplodeSystem1(billboard * bPtr, GLuint p_num, GLint wireMode, vec3 ori)
{
	typeOfPsystem = 1;	//64 x 64 bmp with alpha and particle dot
	debugType = 1;
	notRand = TRUE;
	orient = ori; 
	emissionRate = p_num;
	emmisionRateRandom = 0;
	maxParticles = p_num * 6;
	particleSize_x = 10;         // size in points
	particleSize_y = 10;         // size in points
	particleSizeDelta_x = -2;   // size change
	particleSizeDelta_y = -2;   // size change
	size_gravity = 0;
	billPtr = bPtr;
	gravity = vec3(0, -0.3, 0);
	dieIfLess = vec3(-1000, -1, -1000);
	wind = vec3(0, 0, 0);
	activeticks = 5;	// stage of explosion
	max_ticks = 55;
	if(!wireMode) p_type = render_BB;
	else p_type = render_dot;
	if (length(ori) == 0)
	{
		// omni directional explosion 
		initialVelosityRandom = vec3(1500, 1500, 1500);
		initialVelosity = ori;
	}
	else
	{
		// directional explosion
		initialVelosityRandom = vec3(1200, 500, 1200);
		initialVelosity = 10 *  ori;
	}
	
	v4FloatToVecM(colour, 1, 0.5, 0.1, 1);
	v4FloatToVecM(colourChange, -0.2, -0.4, -0.5, -0.066);
	v4FloatToVecM(colourMax, 1, 1, 1, 1);
	v4FloatToVecM(colourMin, 0, 0, 0, 0);
	airResistance = vec3(0.4, 0.4, 0.4);
	v3FloatToVec(5, 5, 5, absTerminalVelosity);    // max speed
	gravity = vec3(0, -1, 0);
}


void particleLauncher::initExplodeSystem2(billboard * bPtr, GLuint p_num, GLint wireMode)
{
	typeOfPsystem = 2;	//64 x 64 bmp with alpha and particle dot
	notRand = FALSE;
	emissionRate = p_num;
	emmisionRateRandom = 50;
	maxParticles = p_num * 4;
	particleSize_x = 4;         // size in points
	particleSize_y = 4;         // size in points
	particleSizeDelta_x = 3;   // size change
	particleSizeDelta_y = 3;   // size change
	size_gravity = 0;
	billPtr = bPtr;
	gravity = vec3(0, 0, 0);
	dieIfLess = vec3(-1000, -1, -1000);
	wind = vec3(0, 0, 0);
	activeticks = 2;	// stage of explosion
	max_ticks = 30;
	if (!wireMode) p_type = render_BB;
	else p_type = render_dot;
	initialVelosityRandom = vec3(600, 600, 600);
	initialVelosity = vec3(0, 0, 0);
	initialSpinAngle = 0;
	initialSpinAngleRandom = 9000; 
	initialSpinAngleSpd = 0;
	initialSpinAngleSpdRandom = 3000;
	spinDamp = 0.9;
	v4FloatToVecM(colour, 1, 1, 1, 1);
	v4FloatToVecM(colourChange, -0.1, -0.15, -0.2, -0.066);
	v4FloatToVecM(colourMax, 1, 1, 1, 1);
	v4FloatToVecM(colourMin, 0, 0, 0, 0);
	airResistance = vec3(0.3, 0.3, 0.3);
	v3FloatToVec(5, 5, 5, absTerminalVelosity);    // max speed
}

// generate particles moving upwards
void particleLauncher::initExplodeSystem3(billboard * bPtr, GLuint p_num, GLint wireMode)
{
	typeOfPsystem = 1;	//64 x 64 bmp with alpha and particle dot
	notRand = FALSE;
	emissionRate = p_num;
	emmisionRateRandom = 0;
	maxParticles = p_num * 5;
	particleSize_x = 10;         // size in points
	particleSize_y = 10;         // size in points
	particleSizeDelta_x = -1;   // size change
	particleSizeDelta_y = -1;   // size change
	size_gravity = 0;
	billPtr = bPtr;
	gravity = vec3(0, -0.2, 0);
	dieIfLess = vec3(-1000, -1, -1000);
	wind = vec3(0, 0, 0);
	activeticks = 2;	// stage of explosion
	max_ticks = 60;
	if (!wireMode) p_type = render_BB;
	else p_type = render_line;
	initialVelosityRandom = vec3(1000, 1500, 1000);
	initialVelosity = vec3(0, 10, 0);
	v4FloatToVecM(colour, 1, 0.5, 0.1, 1);
	v4FloatToVecM(colourChange, -0.2, -0.4, -0.5, -0.066);
	v4FloatToVecM(colourMax, 1, 1, 1, 1);
	v4FloatToVecM(colourMin, 0, 0, 0, 0);
	airResistance = vec3(0.2, 0.2, 0.2);
	v3FloatToVec(5, 5, 5, absTerminalVelosity);    // max speed
}

void particleLauncher::initExplodeSystem4(billboard * bPtr, GLuint p_num, vec3 ori)
{
	typeOfPsystem = 1;	//particle dot
	notRand = FALSE;
	emissionRate = p_num;
	emmisionRateRandom = 50;
	maxParticles = p_num * 2;
	particleSize_x = 3;         // size in points
	particleSizeDelta_x = -0.1;   // size change
	size_gravity = 0;
	billPtr = bPtr;
	orient = ori;
	gravity = vec3(0.0f);
	dieIfLess = vec3(-1000, -1, -1000);
	wind = vec3(0.0f);
	activeticks = 2;	// stage of explosion
	max_ticks = 5;
	p_type = render_line;
	initialVelosityRandom = vec3(200, 200, 200);
	vec3 tempRand = quaternionRotation(vec3(upVec), vec3(0, 0, 0), lineOfSightPlane, rndToMaxf(360)); // reflect spark around the line of sight of the plane
	initialVelosity = -5 * lineOfSightPlane + 7 * tempRand;
	v4FloatToVecM(colour, 1, 1, 1, 1);
	v4FloatToVecM(colourChange, -0.1, -0.14, -0.16, -0.001);
	v4FloatToVecM(colourMax, 1, 1, 1, 1);
	v4FloatToVecM(colourMin, 0.55, 0.17, 0.06, 0);
	airResistance = vec3(0.3, 0.3, 0.3);
	v3FloatToVec(5, 5, 5, absTerminalVelosity);    // max speed
}

void particleLauncher::initExplodeSystem5(billboard * bPtr, GLuint p_num, GLint wireMode)
{
	typeOfPsystem = 2;	// bill board
	notRand = FALSE;
	emissionRate = p_num;
	emmisionRateRandom = 0;
	maxParticles = p_num;
	particleSize_x = 5;         // size in points
	particleSize_y = 15;         // size in points
	particleSizeDelta_x = 0;   // size change
	particleSizeDelta_y = 6;   // size change
	size_gravity = -0.7;
	billPtr = bPtr;
	gravity = vec3(0.0f);
	dieIfLess = vec3(-1000, -1, -1000);
	wind = vec3(0.0f);
	activeticks = 2;	// stage of explosion
	max_ticks = 15;
	p_type = render_BB;
	initialVelosityRandom = vec3(0.0f);
	initialVelosity = vec3(0.0f);
	v4FloatToVecM(colour, 1, 1, 1, 1);
	v4FloatToVecM(colourChange, 0, 0 , 0, -0.05);
	v4FloatToVecM(colourMax, 1, 1, 1, 1);
	v4FloatToVecM(colourMin, 0.55, 0.17, 0.06, 0);
	airResistance = vec3(0.0f);
	v3FloatToVec(0, 0, 0, absTerminalVelosity);    // max speed
}

void particleLauncher::initExplodeSystem6(billboard * bPtr, GLuint p_num, vec3 ori)
{
	typeOfPsystem = 1;	//particle dot
	notRand = FALSE;
	emissionRate = p_num;
	emmisionRateRandom = 50;
	maxParticles = p_num * 2;
	particleSize_x = 3;         // size in points
	particleSizeDelta_x = -0.1;   // size change
	size_gravity = 0;
	billPtr = bPtr;
	orient = ori;
	gravity = vec3(0.0f);
	dieIfLess = vec3(-1000, -1, -1000);
	wind = vec3(0.0f);
	activeticks = 2;	// stage of explosion
	max_ticks = 5;
	p_type = render_line;
	initialVelosityRandom = vec3(200, 200, 200);
	vec3 tempRand = quaternionRotation(vec3(upVec), vec3(0, 0, 0), orient, rndToMaxf(360)); // reflect spark around local normal where the projectile hit
	initialVelosity = 10 * tempRand;
	v4FloatToVecM(colour, 1, 1, 1, 1);
	v4FloatToVecM(colourChange, -0.1, -0.14, -0.16, -0.001);
	v4FloatToVecM(colourMax, 1, 1, 1, 1);
	v4FloatToVecM(colourMin, 0.55, 0.17, 0.06, 0);
	airResistance = vec3(0.3, 0.3, 0.3);
	v3FloatToVec(5, 5, 5, absTerminalVelosity);    // max speed
}

void particleLauncher::kill()
{
	activeticks = 0; 
	residualEmmision = 0;
	active = false; 
}


void particleLauncher::start(GLint tick)
{
	activeticks = tick;
	active = true;
}

void particleLauncher::addRand(particle& p)
{
	GLfloat f;

	// Add randomness to initial velocity
	if ((int)initialVelosityRandom.x != 0)
	{
		f = (rndToMaxf(initialVelosityRandom.x) - (initialVelosityRandom.x / 2)) / 100;
		p.velosity.x = p.velosity.x + f;
	}
	if ((int)initialVelosityRandom.y != 0)
	{
		f = (rndToMaxf(initialVelosityRandom.y) - initialVelosityRandom.y / 2) / 100;
		p.velosity.y = p.velosity.y + f;
	}
	if ((int)initialVelosityRandom.z != 0)
	{
		f = (rndToMaxf(initialVelosityRandom.z) - initialVelosityRandom.z / 2) / 100;
		p.velosity.z = p.velosity.z + f;
	}

	if (initialSpinAngleRandom != 0)
	{
		f = (rndToMaxf(initialSpinAngleRandom) - initialSpinAngleRandom / 2) / 100;
		p.angle = initialSpinAngle + f;
	}

	if (initialSpinAngleSpdRandom != 0)
	{
		f = (rndToMaxf(initialSpinAngleSpdRandom) - initialSpinAngleSpdRandom / 2) / 100;
		p.angleSpd = initialSpinAngleSpd + f;
	}
}

void particleLauncher::updateSystem0() // update system according to the influential factors in system type 0.
{
	
	if (active)
	{
		residualEmmision = residualEmmision + emissionRate;
		if (emmisionRateRandom != 0) residualEmmision = residualEmmision + rndToMaxf(emmisionRateRandom) / 100;
	}

	particlesStillActive = false;
	

	for (int i = 0; i < maxParticles; i++)
	{
		if ((!p[i].active) && residualEmmision >= 1)
		{
			// create a new particle at empty slot
			residualEmmision = residualEmmision - 1;
			p[i].active = true;
			numParticles++;
			p[i].pSize_x = particleSize_x;  // initial size x
			p[i].pSize_y = particleSize_y;  // initial size y
			p[i].time = 0; //zero timer
			p[i].pos = pos; // new particle has the position of system
			p[i].prevPos = pos;
			p[i].velosity = initialVelosity;
			v4CopyM(p[i].colour, colour); // color

			if (notRand) 
			{
				if (firstTick)
				{
					//Add randomness to the particle in the first tick only
					addRand(p[i]);
					// record the velosity with randomness
					oldVel[genCounter] = p[i].velosity; 
					genCounter++;
				}
				else
				{
					// reuse the stored velocity
					p[i].velosity = oldVel[genCounter]; 
					genCounter++;
				}
			}
			else 
			{
				addRand(p[i]); 
			}
			
		}


		//... then move all the particles...
		if (p[i].active)
		{
			particlesStillActive = true;
		
			p[i].time++;
			p[i].prevPos = p[i].pos;

			if (p[i].time > max_ticks) { p[i].active = false; numParticles--; continue; } // Kill expired particle
			p[i].pos += p[i].velosity; //  move particle by velosity

			// change color
			v4AddToVecM(p[i].colour, colourChange);
			v4ClipMaxM(p[i].colour, colourMax);
			v4ClipMinM(p[i].colour, colourMin);

			// change size
			p[i].pSize_x += particleSizeDelta_x;
			p[i].pSize_y += particleSizeDelta_y;
			if (p[i].pSize_x < 0) p[i].pSize_x = 0;
			if (p[i].pSize_y < 0) p[i].pSize_y = 0;

		}
	}

}


void particleLauncher::updateSystem1()
{
	for (int i = 0; i < maxParticles; i++)
	{
		if (p[i].active) {
			v3dampMM(p[i].velosity, airResistance); // air resistance
			p[i].velosity += gravity; // gravity
			v3clipAbsM(p[i].velosity, absTerminalVelosity); // terminal velosity
			particleSizeDelta_y += size_gravity;
		}
	}
}

void particleLauncher::updateSystem2()
{
	for (int i = 0; i < maxParticles; i++)
	{
		if (p[i].active) {
			// spin the particle
			p[i].angle += p[i].angleSpd;
			p[i].angle = fmod(p[i].angle, 360); // may not need this line

			// damp the angle speed and delta in size
			p[i].angleSpd *= spinDamp;
			particleSizeDelta_x *= damp;
			particleSizeDelta_y *= damp;
		}
	}
}

void particleLauncher::updateSystem3()
{
	for (int i = 0; i < maxParticles; i++)
	{
		if (p[i].active)
		{
			p[i].pos += wind; // wind
			if (v3isLess(p[i].pos, dieIfLess)) { p[i].active = false; numParticles--; continue; }
		}
	}
}

void particleLauncher::update()
{
	if (!active && !particlesStillActive) return;
	if (activeticks>0)
	{
		activeticks--;
		if (activeticks <= 0) { active = false; residualEmmision = 0; }
	}

	// process each particle slot for this time step...
	genCounter = 0; // reset the generation counter

	// update basic system 0
	updateSystem0();

	// update system according to its type. 
	if (typeOfPsystem > 0) updateSystem1(); 
	if (typeOfPsystem > 1) updateSystem2();
	if (typeOfPsystem > 2) updateSystem3();

	firstTick = FALSE;
}

void particleLauncher::drawPsystem()
{
	switch (p_type)
	{
	case render_BB:
		drawTex();
		break;
	case render_dot:
		drawDot();
		break;
	case render_line:
		drawLine();
		break;
	default:
		printf("undefined particle type %d \n", p_type); 
		break;
	}
}

void particleLauncher::drawDot()
{
	int i;

	if (!particlesStillActive) return;
	for (i = 0; i< maxParticles; i++)
	{
		p[i].renderDot(); 
	}

}


void particleLauncher::drawLine()
{
	int i;

	if (!particlesStillActive) return;

	for (i = 0; i < maxParticles; i++)
	{
		p[i].renderLine(); 
	}
}

void particleLauncher::drawTex()
{
	int i;
	if (!particlesStillActive) return;

	glDepthMask(GL_FALSE);
	glPushAttrib(GL_CURRENT_BIT);

	for (i = 0; i < maxParticles; i++)
	{
		if (p[i].active)
		{
			p[i].renderTex(billPtr);
		}
	}
	glPopAttrib();
	glDepthMask(GL_TRUE);


}


void particle::renderTex(billboard * ptr)
{
	glColor4fv(colour);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	//glTranslatef(pos.x, pos.y, pos.z);
	ptr->setPos(pos);
	ptr->setOmniRot(angle); 
	ptr->setSize(pSize_x, pSize_y);
	ptr->render();
	glPopMatrix();
	if (eLight) glEnable(GL_LIGHTING);
}

void particle::renderDot()
{
	glColor4fv(colour);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	if (active)
	{
		glPointSize(pSize_x);
		glBegin(GL_POINTS);
		glVertex3fv(pos.fv());
		glEnd();
	}

	if (eLight) glEnable(GL_LIGHTING); 
	if (showTex) glEnable(GL_TEXTURE_2D);
}

void particle::renderLine()
{
	glColor4fv(colour);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glLineWidth(3);
	glBegin(GL_LINES);
		if (active)
		{
			glVertex3fv(prevPos.fv());
			glVertex3fv(pos.fv());
		}
	glEnd();

	if (eLight) glEnable(GL_LIGHTING);
	if (showTex) glEnable(GL_TEXTURE_2D);
}


void particleLauncher::setPos(vec3 newPos)
{
	pos = newPos; 
}

particle::particle()
{
}


particle::~particle()
{
}

