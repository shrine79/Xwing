// terrain.h 
#ifndef _TERRAN_
#define _TERRAN_

#include "Lighting.h"
#include "billboard.h"

#define MAXWIDTH 257
#define MAXDEPTH 257
#define SEARCH_WIDTH 3

class terrain
{
protected:

	GLfloat xsize;
	GLfloat zsize;
	int xsteps, zsteps;
	GLfloat xdelta, zdelta;
	vec3 origin; 

	GLfloat map[MAXWIDTH][MAXDEPTH];
	GLuint treeMap[MAXWIDTH][MAXDEPTH];
	vec3 normal_map[MAXWIDTH][MAXDEPTH];
	vec3 mesh[MAXWIDTH][MAXDEPTH];

	void generate_mesh(void); 
	inline void mapCoord2Ind(GLfloat x, GLfloat z, int& indx, int& indz); // map world coordinates to height field index (left bottom)
	inline void clampInd2Edge(int& indX, int& indZ); 

public:

	int mapWidth, mapHeight;

	billboard* treePtr; 
	materialClass* mat;
	materialClass* matLines;

	materialClass* matHigh;
	materialClass* matBeach;
	materialClass* matLand;

	GLfloat highHeight;
	GLfloat grassHeight;

	GLfloat getHeight(GLfloat x, GLfloat z); 
	vec3 getNormal(GLfloat x, GLfloat z); // return the normal of according to coordinate on x-z plane
	void setUp(GLfloat xsizeZ, GLfloat zsizeZ, int xstepsZ, int zstepsZ);
	void setMaterialHeight(GLfloat height);

	int render();
	void renderSmooth();
	int renderLines();
	void generateTreeMap(void); 
	int renderTree();
	// return True if the postion is below the ground level. The cheapest version
	bool hitGround(const vec3& pos);

	// return True if the postion is below the ground level. Store estimated hit position in hitPos
	bool hitGround(const vec3& pos, const vec3& posOld, vec3& hitPos); 
	// the version also store the normal of the hit surface in hitNormal
	bool hitGround(const vec3& pos, const vec3& posOld, vec3& hitPos, vec3& hitNormal);

	// create a bounding box around position and return true if any corner of the 
	// bounding box below ground level. 
	bool boxHitGround(const vec3& pos, GLfloat box_x, GLfloat box_y, GLfloat box_z);

	terrain(char * filename, int worldSize, vec3 point_o, GLfloat heightScale, billboard* tree);
	~terrain();

};

void terrain::clampInd2Edge(int& indX, int& indZ)
{
	// Clamp coordinates to map edges
	if (indX < 0) indX = 0;
	if (indZ < 0) indZ = 0;

	if (indX >= xsteps)
		indX = xsteps - 1;
	if (indZ >= zsteps)
		indZ = zsteps - 1;
}


void terrain::mapCoord2Ind(GLfloat x, GLfloat z, int& indx, int& indz)
{
	indx = (int)floorf((x + origin.x) / xdelta);
	indz = (int)floorf((z + origin.z) / zdelta);

	clampInd2Edge(indx, indz); 
}

#endif