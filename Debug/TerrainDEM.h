// Digital Elevation Model

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"
#include "vec3.h"
#include "fileOperations.h"

#ifndef DEMclassz_def 
#define DEMclassz_def 

class DEMTerrain
{
public:

	BOOLEAN terNorm = TRUE; 
	GLfloat xDist;
	GLfloat zDist;

	vec3 triangles[257][257][3];
	vec3 triangleNormals[257][257];
	vec3 vertexNormals[257][257];

	vec3 vertex[3];

	GLfloat minZVal;
	GLfloat maxZVal;

	vec3 waterColour;
	vec3 highColour;
	vec3 lowColour;
	vec3 normalColour;

	vec3 n0;

	vec3 temp1n; // for rendering normals
	vec3 temp2n;

	DEMTerrain(); // constructor

	int readFile(char *fname);
	void displayCurrentValues();
	void scaleValues(float f);
	void setMinMax();

	void setHeightColour(GLfloat height);
	GLfloat linear_interp(GLfloat vMin, GLfloat vMax, GLfloat val, GLfloat tMin, GLfloat tMax);

	void renderDots();
	void renderWater();
	void renderLines();
	void renderSmooth();
	void computeTriangles();
	void computeTriNormals();
	void computeTriNormalsVal();
	void newBool();
	//void renderWithSmooth();
	void renderTheNormals();
	void rendNorm();
	void renderSmoothLine();
	void renderWaterLine();

};

#endif
// end 