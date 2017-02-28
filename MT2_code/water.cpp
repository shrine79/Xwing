#include "water.h"
#include "skybox.h"
#include "main.h"

//#define QUICK_SIN
water::water()
{
	meshPtr = NULL;
	normalMeshPtr = NULL;
	sinPtr = new sineTab(); 
	setUp(20, 20, 0, 1, 1, glutGet(GLUT_ELAPSED_TIME), 1, NULL); 
}


water::~water()
{
	if (meshPtr != NULL)
	{
		delete[] meshPtr;
		meshPtr = NULL;
	}

	if (normalMeshPtr != NULL)
	{
		delete[] normalMeshPtr;
		normalMeshPtr = NULL;
	}
	
	delete sinPtr; 
}

void water::setUp(int xstepsZ, int zstepsZ, GLfloat theta, GLfloat lamda, GLfloat frequency, GLfloat time, 
	GLfloat magnitude, terrain * ptr)
{
	xsteps = xstepsZ;
	zsteps = zstepsZ;
	Theta = theta; 
	Lamda = lamda; 
	Freq = frequency;  
	FreqZ = Freq / 800;
	FreqX = Freq / 500;
	Time = time;
	Mag = magnitude;
	offsetX = 0;
	offsetZ = 0;
	tPtr = ptr;
	if (meshPtr != NULL)
	{
		delete[] meshPtr;
		meshPtr = NULL;
	}
	meshPtr = new vec3[(WATER_WAVE_RANGE + 3) * (WATER_WAVE_RANGE + 3)];

	if (normalMeshPtr != NULL)
	{
		delete[] normalMeshPtr;
		normalMeshPtr = NULL;
	}
	normalMeshPtr = new vec3[(WATER_WAVE_RANGE + 3) * (WATER_WAVE_RANGE + 3)];
	
	// initialize vertex mesh with points on SURFACE plane. 
	// initialize normal mesh with up right vector. 
	for (int i = 0; i < (WATER_WAVE_RANGE + 3) * (WATER_WAVE_RANGE + 3); i++)
	{
		meshPtr[i] = vec3(0, SURFACE, 0); 
		normalMeshPtr[i] = vec3(0, 1, 0);
	}


	xdelta = SKY_LEN / xsteps;
	zdelta = SKY_LEN / zsteps;

}

void water::update(GLfloat time)
{
	Time = time;
}

GLfloat water::GetYfromX(GLfloat x)
{
	GLfloat retVal = x  / Lamda;
	//retVal -= Freq * Time/1000 + Theta;
	retVal -= Theta;
#ifdef QUICK_SIN
	return 0.3 * Mag * sinPtr->getSin(retVal); 
#else
	return 0.3 * Mag * sin(retVal * 2 * M_PI);
#endif
}


// Larger wave running at opposite direction against x
GLfloat water::GetYfromXLarge(GLfloat x)
{
	GLfloat retVal = x / (Lamda * 5);
	//GLfloat retVal = x / (Lamda * 50);
	retVal += FreqX * Time - Theta;
	retVal -= Theta;
#ifdef QUICK_SIN
	return 0.8 * Mag * sinPtr->getSin(retVal);
#else
	return 0.8 * Mag * sin(retVal * 2 * M_PI);
#endif
}
 
// Small wave running at opposite direction agaisnt z, high frequency, low wavelength
GLfloat water::GetYfromZ(GLfloat z)
{
	GLfloat retVal = z / (Lamda * 2);
	retVal -= FreqZ * Time + Theta;
	//retVal -= Theta;
#ifdef QUICK_SIN
	return 0.8 * Mag * sinPtr->getSin(retVal);
#else
	return 0.8 * Mag * sin(retVal * 2 * M_PI);
#endif
}

// diagnal wave
GLfloat water::GetYfromZSmall(GLfloat x, GLfloat z)
{
	GLfloat retVal = sqrt(z*z + x*x) / (Lamda * 0.4);
	retVal += Freq * Time / 200 - Theta;
	//retVal -= Theta;
#ifdef QUICK_SIN
	return 0.2 * Mag * sinPtr->getSin(retVal);
#else
	return 0.2 * Mag * sin(retVal * 2 * M_PI);
#endif
}


inline GLfloat water::GetY(GLfloat x, GLfloat z)
{

	GLfloat x2 = planePos.x - x;
	GLfloat z2 = planePos.z - z;

	GLfloat dist_to_eye_sq = x2 * x2 + z2 * z2;
	if (dist_to_eye_sq > 6000)
		return 0;
	else if (dist_to_eye_sq <= 3000)
		return GetYfromXLarge(x) + GetYfromZ(z);
	else
		return (GetYfromXLarge(x) + GetYfromZ(z)) * 3000 / (dist_to_eye_sq);


}


int water::render() // CW rendering
{
	if (showTex)
	{
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_FLAT);
		glBindTexture(GL_TEXTURE_2D, TexHandles[5]);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 1, 1);
		glEnable(GL_BLEND);
	}
	else
		glDisable(GL_TEXTURE_2D);

	glDisable(GL_CULL_FACE);	// Disable culling for water rendering to allow under water view.
	glColor3f(0.0, 0.2, 0.8 );

	glPushMatrix();

	vec3 p0, p1, p2, p3, n0, n1;
	// Render a plain water surface 
	p0 = vec3( 0.5 * SKY_LEN, SURFACE, -0.25 * SKY_LEN); 
	p1 = vec3(-0.5 * SKY_LEN, SURFACE, -0.25 * SKY_LEN);
	p2 = vec3(-0.5 * SKY_LEN, SURFACE,  0.75 * SKY_LEN);
	p3 = vec3( 0.5 * SKY_LEN, SURFACE,  0.75 * SKY_LEN);

	glDepthMask(GL_FALSE);
	glBegin(GL_QUADS);
		glNormal3fv(vec3(0, 1, 0).fv()); // normal is always up
		glTexCoord2f(0, 0);	glVertex3fv(p0.fv());
		glTexCoord2f(1, 0);	glVertex3fv(p1.fv());
		glTexCoord2f(1, 1);	glVertex3fv(p2.fv());
		glTexCoord2f(0, 1);	glVertex3fv(p3.fv());
	glEnd(); 
	glDepthMask(GL_TRUE);

	// Render another layer of water surface with dynamic wave for proximity. 
	
	offsetX = planePos.x - WATER_WAVE_RANGE;
	offsetZ = planePos.z - WATER_WAVE_RANGE;
	GLfloat coordX, coordZ;

	glBegin(GL_TRIANGLES);

	for (int x = 0; x < xsteps; x++)
		for (int z = 0; z < zsteps; z++)
		{
			coordX = xdelta*x + offsetX;
			coordZ = zdelta*z + offsetZ;
			if (tPtr->getHeight(coordX, coordZ) > SURFACE + 4) continue;

			p0 = vec3(coordX, GetY(coordX, coordZ), coordZ);
			p1 = vec3(coordX + xdelta, GetY(coordX + xdelta, coordZ), coordZ);
			p2 = vec3(coordX + xdelta, GetY(coordX + xdelta, coordZ + zdelta), coordZ + zdelta);
			p3 = vec3(coordX, GetY(coordX, coordZ + zdelta), coordZ + zdelta);

			n0 = polygonNormal(p0, p3, p1);
			n1 = polygonNormal(p3, p2, p1);

			//draw first triangle
			glNormal3fv(n0.fv());
			glTexCoord2f(0, 1);	glVertex3fv(p0.fv());
			glTexCoord2f(1, 0);	glVertex3fv(p3.fv());
			glTexCoord2f(1, 1);	glVertex3fv(p1.fv());

			
			//draw second triangle
			glNormal3fv(n1.fv());
			glTexCoord2f(0, 1); glVertex3fv(p3.fv());
			glTexCoord2f(1, 0);	glVertex3fv(p2.fv());
			glTexCoord2f(1, 1);	glVertex3fv(p1.fv());

		}

	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	if (hiddenCull)
		glEnable(GL_CULL_FACE);
	return true;
}

int water::renderSmooth()  // CW rendering with smooth normals
{
	if (showTex)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_NORMALIZE);
		glBindTexture(GL_TEXTURE_2D, TexHandles[5]);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 1, 1);
		glEnable(GL_BLEND);
	}
	else
		glDisable(GL_TEXTURE_2D);

	glDisable(GL_CULL_FACE);	// Disable culling for water rendering to allow under water view.
	glDepthMask(GL_FALSE);
	glColor3f(0.0, 0.2, 0.8);



	// round eye point to wave grid. 
	int centre_gridx = static_cast<int>(eyePoint[0] / xdelta); 
	int centre_gridz = static_cast<int>(eyePoint[2] / zdelta);

	offsetX = centre_gridx - WATER_WAVE_RADIUS;
	offsetZ = centre_gridz - WATER_WAVE_RADIUS;

	GLfloat coordx, coordz;

	// The first pass populates the mesh with height values from sin waves. 
	// The mesh is padded with zero normals. 
	for (int x = 1; x <= WATER_WAVE_RANGE + 1; x++)
		for (int z = 1; z <= WATER_WAVE_RANGE + 1; z++)
		{
			coordx = xdelta * (offsetX + x);
			coordz = zdelta * (offsetZ + z);

			meshPtr[x + z * (WATER_WAVE_RANGE + 3)] = vec3(coordx, GetY(coordx, coordz) + SURFACE, coordz);
		}


	// The second pass populates the  normal mesh with calculated vectors. 
	for (int x = 1; x <= WATER_WAVE_RANGE + 1; x++)
		for (int z = 1; z <= WATER_WAVE_RANGE + 1; z++)
		{
			int i = x + z * (WATER_WAVE_RANGE + 3);
			int upper = x + (z - 1) * (WATER_WAVE_RANGE + 3);
			int left = (x - 1) + z * (WATER_WAVE_RANGE + 3);
			int right = (x + 1) + z * (WATER_WAVE_RANGE + 3);
			int lower = x + (z + 1) * (WATER_WAVE_RANGE + 3);
			vec3 n0 = polygonNormal(meshPtr[i], meshPtr[upper], meshPtr[left]);
			vec3 n1 = polygonNormal(meshPtr[i], meshPtr[right], meshPtr[upper]);
			vec3 n2 = polygonNormal(meshPtr[i], meshPtr[lower], meshPtr[right]);
			vec3 n3 = polygonNormal(meshPtr[i], meshPtr[left],  meshPtr[lower]);
			
			normalMeshPtr[i] = (n0 + n1 + n2 + n3) / 4.0f;  // do not have normalization

		 //   // Debug: draw normal for each vertice
			//glDisable(GL_LIGHTING);
			//glBegin(GL_LINES);
			//glVertex3fv(meshPtr[i].fv());
			//glVertex3fv((meshPtr[i] + normalMeshPtr[i]).fv());
			//glEnd();
			//glEnable(GL_LIGHTING);
		}
	// Render the vectex with smoothed normals. 
	glPushMatrix();

	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLES);

	for (int x = 1; x <= WATER_WAVE_RANGE; x++) // do not draw the last element in the array as it contains invalid data. stop iteration at element - 2
		for (int z = 1; z <= WATER_WAVE_RANGE; z++)
		{
			int i0 = x + z * (WATER_WAVE_RANGE + 3);
			int i1 = (x + 1) + z * (WATER_WAVE_RANGE + 3);
			int i2 = (x + 1) + (z + 1) * (WATER_WAVE_RANGE + 3);
			int i3 = x + (z + 1) * (WATER_WAVE_RANGE + 3);

			//draw first triangle
			glNormal3fv(normalMeshPtr[i3].fv()); glTexCoord2f(0, 1); glVertex3fv(meshPtr[i3].fv());
			glNormal3fv(normalMeshPtr[i1].fv()); glTexCoord2f(1, 1); glVertex3fv(meshPtr[i1].fv());
			glNormal3fv(normalMeshPtr[i0].fv()); glTexCoord2f(0, 0); glVertex3fv(meshPtr[i0].fv());

			//draw second triangle
			glNormal3fv(normalMeshPtr[i2].fv()); glTexCoord2f(0, 0); glVertex3fv(meshPtr[i2].fv());
			glNormal3fv(normalMeshPtr[i1].fv()); glTexCoord2f(1, 1); glVertex3fv(meshPtr[i1].fv());
			glNormal3fv(normalMeshPtr[i3].fv()); glTexCoord2f(1, 0); glVertex3fv(meshPtr[i3].fv());

		}

	glEnd();

	vec3 p0, p1, p2, p3;

	p0 = vec3(0.5 * SKY_LEN, SURFACE, -0.25 * SKY_LEN);
	p1 = vec3(-0.5 * SKY_LEN, SURFACE, -0.25 * SKY_LEN);
	p2 = vec3(-0.5 * SKY_LEN, SURFACE, 0.75 * SKY_LEN);
	p3 = vec3(0.5 * SKY_LEN, SURFACE, 0.75 * SKY_LEN);
	GLfloat leftOfWaveBox = meshPtr[WATER_WAVE_RANGE + 4].x;
	GLfloat rightOfWaveBox = meshPtr[(WATER_WAVE_RANGE + 3) * (WATER_WAVE_RANGE + 2) - 2].x;
	GLfloat botOfWaveBox = meshPtr[WATER_WAVE_RANGE + 4].z;
	GLfloat topOfWaveBox = meshPtr[(WATER_WAVE_RANGE + 3) * (WATER_WAVE_RANGE + 2) - 2].z; 

	glBegin(GL_QUADS);
	
	// Draw 4 quads to fill the gap between skybox and the wave square
	glNormal3fv(vec3(0, 1, 0).fv()); // normal is always up
	glTexCoord2f(0, 0);	glVertex3fv(p0.fv());
	glTexCoord2f(1, 0);	glVertex3f(p0.x, SURFACE, botOfWaveBox);
	glTexCoord2f(1, 1);	glVertex3f(p1.x, SURFACE, botOfWaveBox);
	glTexCoord2f(0, 1);	glVertex3f(p1.x, SURFACE, p1.z);

	glNormal3fv(vec3(0, 1, 0).fv()); // normal is always up
	glTexCoord2f(0, 0);	glVertex3fv(p3.fv());
	glTexCoord2f(1, 0);	glVertex3f(p2.x, SURFACE, p2.z);
	glTexCoord2f(1, 1);	glVertex3f(p2.x, SURFACE, topOfWaveBox);
	glTexCoord2f(0, 1);	glVertex3f(p3.x, SURFACE, topOfWaveBox);

	// Draw 4 quads to fill the gap between skybox and the wave square
	glNormal3fv(vec3(0, 1, 0).fv()); // normal is always up
	glTexCoord2f(0, 0);	glVertex3f(p0.x, SURFACE, botOfWaveBox);
	glTexCoord2f(1, 0);	glVertex3f(p3.x, SURFACE, topOfWaveBox);
	glTexCoord2f(1, 1);	glVertex3f(rightOfWaveBox, SURFACE, topOfWaveBox);
	glTexCoord2f(0, 1);	glVertex3f(rightOfWaveBox, SURFACE, botOfWaveBox);

	// Draw 4 quads to fill the gap between skybox and the wave square
	glNormal3fv(vec3(0, 1, 0).fv()); // normal is always up
	glTexCoord2f(0, 0);	glVertex3f(leftOfWaveBox, SURFACE, botOfWaveBox);
	glTexCoord2f(1, 0);	glVertex3f(leftOfWaveBox, SURFACE, topOfWaveBox);
	glTexCoord2f(1, 1);	glVertex3f(p2.x, SURFACE, topOfWaveBox);
	glTexCoord2f(0, 1);	glVertex3f(p2.x, SURFACE, botOfWaveBox);


	glEnd();
	glDepthMask(GL_TRUE);


	glShadeModel(GL_FLAT);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_NORMALIZE);
	glPopMatrix();

	if(hiddenCull)
		glEnable(GL_CULL_FACE);

	return true;

}



int water::renderLines()
{
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	vec3 p0, p1, p2, p3, n0, n1;

	glPolygonMode(GL_BACK, GL_LINE);
	glPolygonMode(GL_FRONT, GL_LINE);

	glBegin(GL_TRIANGLES);
	
	GLfloat coordX, coordZ;
	for (int x = 0; x < xsteps; x++)
		for (int z = 0; z < zsteps; z++)
		{
			coordX = xdelta*x + offsetX;
			coordZ = zdelta*z + offsetZ;
			if (tPtr->getHeight(coordX, coordZ) > SURFACE + 4) continue;

			p0 = vec3(coordX, GetY(coordX, coordZ), coordZ);
			p1 = vec3(coordX + xdelta, GetY(coordX + xdelta, coordZ), coordZ);
			p2 = vec3(coordX + xdelta, GetY(coordX + xdelta, coordZ + zdelta), coordZ + zdelta);
			p3 = vec3(coordX, GetY(coordX, coordZ + zdelta), coordZ + zdelta);

			n0 = polygonNormal(p0, p3, p1);
			n1 = polygonNormal(p3, p2, p1);

			//draw first triangle
			glNormal3fv(n0.fv());
			glTexCoord2f(0, 1);	glVertex3fv(p0.fv());
			glTexCoord2f(1, 0);	glVertex3fv(p3.fv());
			glTexCoord2f(1, 1);	glVertex3fv(p1.fv());


			//draw second triangle
			glNormal3fv(n1.fv());
			glTexCoord2f(0, 1); glVertex3fv(p3.fv());
			glTexCoord2f(1, 0);	glVertex3fv(p2.fv());
			glTexCoord2f(1, 1);	glVertex3fv(p1.fv());

		}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();
	return true;
}