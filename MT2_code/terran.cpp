

#include "main.h"

#include "terran.h"
#include "fileOperations.h"
// -----------------------------------------------------------------------------------

// X22_0112.TXT

//GLfloat xsize;
//GLfloat zsize;
//int xsteps;
//int zsteps;

//materialClass* mat;
//materialClass* matLines;

void terrain::setUp(GLfloat worldSizeX, GLfloat worldSizeZ, int mapSizeX, int mapSizeZ)
{
	xsize = worldSizeX;
	zsize = worldSizeZ;
	xsteps = mapSizeX;
	zsteps = mapSizeZ;
	xdelta = xsize / (xsteps - 1); // minus 1 to get the correct number of intervals among map points. 
	zdelta = zsize / (zsteps - 1); // minus 1 to get the correct number of intervals among map points. 
}

void terrain::setMaterialHeight(GLfloat height)
{
	if (lerpMat)
	{
		if (height >= highHeight + HEIGHT_MARGIN)
			matHigh->doMaterial();
		else if (height < highHeight + HEIGHT_MARGIN && height >= highHeight - HEIGHT_MARGIN)
			mixMat(materialClass(), *matHigh, *matLand, (height - (highHeight - HEIGHT_MARGIN)) / (HEIGHT_MARGIN * 2)).doMaterial();
		else
		{
			if (height >= grassHeight + HEIGHT_MARGIN)
				matLand->doMaterial();
			else if (height < grassHeight + HEIGHT_MARGIN && height >= grassHeight - HEIGHT_MARGIN)
				mixMat(materialClass(), *matLand, *matBeach, (height - (grassHeight - HEIGHT_MARGIN)) / (HEIGHT_MARGIN * 2)).doMaterial();
			else
				matBeach->doMaterial();
		}
	}
	else 
	{
		if (height >= highHeight)
			matHigh->doMaterial();
		else
		{
			if (height > grassHeight)
				matLand->doMaterial();
			else
				matBeach->doMaterial();
		}
	}
	
}



GLfloat terrain::getHeight(GLfloat x, GLfloat z)
{
	int indX0, indZ0;
	mapCoord2Ind(x, z, indX0, indZ0); 
	GLfloat h0 = map[indX0][indZ0];

	int indX1 = indX0, indZ1 = indZ0++;
	clampInd2Edge(indX1, indZ1);
	GLfloat h1 = map[indX1][indZ1];

	int indX2 = indX0++, indZ2 = indZ0++;
	clampInd2Edge(indX2, indZ2);
	GLfloat h2 = map[indX2][indZ2];

	int indX3 = indX0++, indZ3 = indZ0;
	clampInd2Edge(indX3, indZ3);
	GLfloat h3 = map[indX3][indZ3];
 
	// Calculate average height from four corner values. 
	return (h0 + h1 + h2 + h3) / 4 - origin.y;
}

vec3 terrain::getNormal(GLfloat x, GLfloat z)
{
	int indX0, indZ0;
	mapCoord2Ind(x, z, indX0, indZ0);
	vec3 n0 = normal_map[indX0][indZ0];

	int indX1 = indX0, indZ1 = indZ0++;
	clampInd2Edge(indX1, indZ1);
	vec3 n1 = normal_map[indX1][indZ1];

	int indX2 = indX0++, indZ2 = indZ0++;
	clampInd2Edge(indX2, indZ2);
	vec3 n2 = normal_map[indX2][indZ2];

	int indX3 = indX0++, indZ3 = indZ0;
	clampInd2Edge(indX3, indZ3);
	vec3 n3 = normal_map[indX3][indZ3];

	//Calculate the average normal from four corners
	return (n0 + n1 + n2 + n3) / 4; 
}

void terrain::generateTreeMap(void)
{
	// Generate a random tree map same size of the height field. 
	for (int x = 0; x < xsteps; x++)
		for (int z = 0; z < zsteps; z++)
		{
			if ((rand() % 100 < 25) && 
				map[x][z] > GRASS_HEIGHT &&
				map[x][z] < HIGH_HEIGHT &&
				normal_map[x][z].y > 0.5)
				treeMap[x][z] = 1; 
			else
				treeMap[x][z] = 0;
		}
}

void terrain::generate_mesh(void)
{
	GLfloat coordx, coordz;
	vec3 up, down, left, right, centre, n0, n1, n2, n3;
	for (int x = 0; x < xsteps; x++)
		for (int z = 0; z < zsteps; z++)
		{
			coordx = xdelta*x;
			coordz = zdelta*z;
			if (x == 0 && z == 0)
			{
				centre = vec3(coordx, map[x][z], coordz);
				down = vec3(coordx, map[x][z + 1], coordz + zdelta);
				right = vec3(coordx + xdelta, map[x + 1][z], coordz);
				n2 = polygonNormal(centre, down, right);
				mesh[x][z] = centre;
				normal_map[x][z] = n2; 
			}
			else if(x == 0 && z == zsteps - 1)
			{
				centre = vec3(coordx, map[x][z], coordz);
				up = vec3(coordx, map[x][z - 1], coordz - zdelta);
				right = vec3(coordx + xdelta, map[x + 1][z], coordz);
				n3 = polygonNormal(centre, right, up);
				mesh[x][z] = centre;
				normal_map[x][z] = n3;
			}
			else if (x == xsteps - 1 && z == zsteps - 1)
			{
				centre = vec3(coordx, map[x][z], coordz);
				up = vec3(coordx, map[x][z - 1], coordz - zdelta);
				left = vec3(coordx - xdelta, map[x - 1][z], coordz);
				n0 = polygonNormal(centre, up, left);
				mesh[x][z] = centre;
				normal_map[x][z] = n0;
			}
			else if (x == xsteps - 1 && z == 0)
			{
				centre = vec3(coordx, map[x][z], coordz);
				down = vec3(coordx, map[x][z + 1], coordz + zdelta);
				left = vec3(coordx - xdelta, map[x - 1][z], coordz);
				n1 = polygonNormal(centre, left, down);
				mesh[x][z] = centre;
				normal_map[x][z] = n1;
			}
			if (x == xsteps - 1 || x == 0 || z == zsteps - 1 || z == 0)
			{
				continue;
			}
			else
			{
				
				centre = vec3(coordx, map[x][z], coordz);
				up = vec3(coordx, map[x][z - 1], coordz - zdelta);
				down = vec3(coordx, map[x][z + 1], coordz + zdelta);
				left = vec3(coordx - xdelta, map[x - 1][z], coordz);
				right = vec3(coordx + xdelta, map[x + 1][z], coordz);

				n0 = polygonNormal(centre, up, left);
				n1 = polygonNormal(centre, left, down);
				n2 = polygonNormal(centre, down, right);
				n3 = polygonNormal(centre, right, up);

				normal_map[x][z] = (n0 + n1 + n2 + n3) / 4.0f;
				mesh[x][z] = centre;
				mesh[x - 1][z] = left;
				mesh[x + 1][z] = right;
				mesh[x][z - 1] = up;
				mesh[x][z + 1] = down;
			}
		}
}

bool terrain::hitGround(const vec3& pos)
{
	if (pos.y <= getHeight(pos.x, pos.z))
	{
		return TRUE;
	}
	
	return FALSE;
}


bool terrain::hitGround(const vec3& pos, const vec3& posOld, vec3& hitPos)
{
	if (pos.y <= getHeight(pos.x, pos.z))
	{
		// back trace the ray from current position to old position
		vec3 unitv = (posOld - pos) / 3; // back tracing unit vector
		vec3 tempPos = posOld; // tracing position
		int cnt = 0; // flip counter
		int dir = 1; // the direction of tracing. 1: backward; -1: forward
		do 
		{
			tempPos += unitv * dir; // trace back by 1 unit vector
			if ((dir * tempPos.y) > (dir * getHeight(tempPos.x, tempPos.z)))
			{
				// we've crossed hit point
				dir = -dir; // flip trace direction
				unitv *= 0.5; // halve the vector
				cnt++; // flip counter increase
			}

		} while( cnt < 2); // stop if we've crossed hit point enough times

		// move the trace position for the last time
		tempPos += unitv * dir;

		// update hit position
		hitPos = tempPos; 
		return TRUE;
	}
		
	return FALSE;
}

bool terrain::hitGround(const vec3& pos, const vec3& posOld, vec3& hitPos, vec3& hitNormal)
{
	if (hitGround(pos, posOld, hitPos))
	{
		hitNormal = getNormal(hitPos.x, hitPos.y);
		return TRUE;
	}

	return FALSE;
}

bool terrain::boxHitGround(const vec3& pos, GLfloat box_x, GLfloat box_y, GLfloat box_z)
{
	BBpoints[0] = vec3(pos.x - box_x, pos.y + box_y, pos.z + box_z);
	BBpoints[1] = vec3(pos.x + box_x, pos.y + box_y, pos.z + box_z);
	BBpoints[2] = vec3(pos.x + box_x, pos.y + box_y, pos.z - box_z);
	BBpoints[3] = vec3(pos.x - box_x, pos.y + box_y, pos.z - box_z);
	BBpoints[4] = vec3(pos.x - box_x, pos.y - box_y, pos.z + box_z);
	BBpoints[5] = vec3(pos.x + box_x, pos.y - box_y, pos.z + box_z);
	BBpoints[6] = vec3(pos.x + box_x, pos.y - box_y, pos.z - box_z);
	BBpoints[7] = vec3(pos.x - box_x, pos.y - box_y, pos.z - box_z);

	for (int i = 0; i < 8; i++)
	{
		if (hitGround(BBpoints[i]))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void terrain::renderSmooth()
{
	glEnable(GL_CULL_FACE);

	glColor3f(0.5, 0.3, 0.4);

	// Render the vectex with smoothed normals. 
	glPushMatrix();

	glShadeModel(GL_SMOOTH);
	


	// render vertex
	for (int x = 0; x < xsteps - 1; x++)
		for (int z = 0; z < zsteps - 1; z++)
		{
			glBegin(GL_TRIANGLES);
			//draw first triangle
			setMaterialHeight(map[x][z]);	glNormal3fv(normal_map[x][z].fv()); glTexCoord2f(0, 0); glVertex3fv((mesh[x][z] - origin).fv());
			setMaterialHeight(map[x][z + 1]); glNormal3fv(normal_map[x][z + 1].fv()); glTexCoord2f(0, 1); glVertex3fv((mesh[x][z + 1] - origin).fv());
			setMaterialHeight(map[x + 1][z]); glNormal3fv(normal_map[x + 1][z].fv()); glTexCoord2f(1, 0); glVertex3fv((mesh[x + 1][z] - origin).fv());


			//draw second triangle
			setMaterialHeight(map[x + 1][z + 1]); glNormal3fv(normal_map[x + 1][z + 1].fv()); glTexCoord2f(1, 1); glVertex3fv((mesh[x + 1][z + 1] - origin).fv());
			setMaterialHeight(map[x + 1][z]); glNormal3fv(normal_map[x + 1][z].fv()); glTexCoord2f(1, 0); glVertex3fv((mesh[x + 1][z] - origin).fv());
			setMaterialHeight(map[x][z + 1]); glNormal3fv(normal_map[x][z + 1].fv()); glTexCoord2f(0, 1); glVertex3fv((mesh[x][z + 1] - origin).fv());

			glEnd();

			
			// Debug: draw normal for each vertice
			if (showNormals)
			{
				glBegin(GL_LINES);
				glVertex3fv((mesh[x][z] - origin).fv());
				glVertex3fv(((mesh[x][z] - origin) + normal_map[x][z]).fv());
				glEnd();
			}
		}
	
	glPopMatrix();

	if (!hiddenCull)
		glDisable(GL_CULL_FACE);
}

int terrain::render()
{
	glEnable(GL_CULL_FACE);

	glPushMatrix();
	
	vec3 p0, p1, p2, p3, n0, n1;


	for (int x = 0; x<xsteps - 1; x++)
		for (int z = 0; z<zsteps - 1; z++)
		{
			
			p0 = vec3(xdelta*x, map[x][z], zdelta*z) - origin;
			p1 = vec3(xdelta*x + xdelta, map[x + 1][z], zdelta*z) - origin;
			p2 = vec3(xdelta*x, map[x][z + 1], zdelta*z + zdelta) - origin;
			p3 = vec3(xdelta*x + xdelta, map[x + 1][z + 1], zdelta*z + zdelta) - origin;
			n0 = polygonNormal(p0, p2, p1);
			n1 = polygonNormal(p3, p1, p2);

			glBegin(GL_TRIANGLES);

			GLfloat adj = n0.y * adjY;
			glNormal3fv(n0.fv());
			setMaterialHeight(map[x][z] - adj);		  glVertex3fv(p0.fv());
			setMaterialHeight(map[x][z + 1] - adj);	  glVertex3fv(p2.fv());
			setMaterialHeight(map[x + 1][z] - adj);	  glVertex3fv(p1.fv());

			adj = n1.y * adjY;
			glNormal3fv(n1.fv());
			setMaterialHeight(map[x + 1][z + 1] - adj); glVertex3fv(p3.fv());
			setMaterialHeight(map[x + 1][z] - adj);     glVertex3fv(p1.fv());
			setMaterialHeight(map[x][z + 1] - adj);     glVertex3fv(p2.fv());
			
			glEnd();

			

			// Debug: draw normal for each vertice
			if (showNormals)
			{
				glBegin(GL_LINES);
				vec3 p = (p0 + p1 + p2) / 3; 
				glVertex3fv(p.fv());
				glVertex3fv( (p + n0).fv() );

				p = (p3 + p1 + p2) / 3;
				glVertex3fv(p.fv());
				glVertex3fv((p + n1).fv());

				glEnd();
			}
		}




	glPopMatrix();

	if (!hiddenCull)
		glDisable(GL_CULL_FACE);

	return true;
}

int terrain::renderTree()
{
	vec3 p0;
	glDepthMask(GL_FALSE);
	for (int x = 0; x<xsteps - 1; x++)
		for (int z = 0; z < zsteps - 1; z++)
		{

			// Draw trees according to treemap
			if (treeMap[x][z] == 1)
			{
				p0 = vec3(xdelta*x, map[x][z] + TREE_SIZE *(1.2 - normal_map[x][z].y * 0.8), zdelta*z) - origin;
				treePtr->setPos(p0);
				treePtr->render();
			}
		}
	glDepthMask(GL_TRUE);
	return true;
}


int terrain::renderLines()
{
	glPushMatrix();

	matLines->doMaterial();

	glBegin(GL_TRIANGLES);
	glNormal3f(0, 1, 0); // yes veronica all normals point up (students need to fix this)
	for (int x = 0; x<xsteps - 1; x++)
		for (int z = 0; z<zsteps - 1; z++)
		{
			vec3 p0, p1, p2, p3; 
			p0 = vec3(xdelta*x, 0.01 + map[x][z], zdelta*z) - origin;
			p1 = vec3(xdelta*x + xdelta, 0.01 + map[x + 1][z], zdelta*z) - origin;
			p2 = vec3(xdelta*x, 0.01 + map[x][z + 1], zdelta*z + zdelta) - origin;
			p3 = vec3(xdelta*x + xdelta, 0.01 + map[x + 1][z + 1], zdelta*z + zdelta) - origin;
			
			glVertex3fv(p0.fv()); 
			glVertex3fv(p2.fv());
			glVertex3fv(p1.fv());

			glVertex3fv(p3.fv());
			glVertex3fv(p1.fv());
			glVertex3fv(p2.fv());

		}

	glEnd();
	glPopMatrix();
	return true;
}


terrain::terrain(char * filename, int worldSize, vec3 point_O, GLfloat heightScale, billboard* tree)
{
	mat = new materialClass();
	matLines = new materialClass();
	mat->setMaterialJade();
	matLines->setMaterialBlackRubber();

	matHigh = new materialClass();
	matBeach = new materialClass();
	matLand = new materialClass();

	matHigh->setMaterialWhiteStuff();
	matBeach->setMaterialYellowSand();
	matLand->setMaterialGreenGrass();
	treePtr = tree;

	//readHeightField(findFile("X20_23.txt"), &mapWidth, &mapHeight, map);
	readHeightField(findFile(filename), &mapWidth, &mapHeight, map);

	setUp(worldSize, worldSize, mapWidth, mapHeight);

	highHeight = HIGH_HEIGHT;
	grassHeight = GRASS_HEIGHT;
	origin = point_O.fv();

	for (int i = 0; i < mapWidth; i++)
		for (int j = 0; j < mapHeight; j++)
			map[i][j] *= heightScale;

	generate_mesh();
	generateTreeMap();
}

terrain::~terrain()
{
	delete mat;
	delete matLines;
	delete matBeach;
	delete matHigh;
	delete matLand;
}

// end
