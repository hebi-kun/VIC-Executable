#ifndef triprismclassz_def 
#define triprismclassz_def 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"

#include "vec3.h"

class TriClassSmooth
{
public:

	vec3 p0;
	vec3 p1;
	vec3 p2;
	vec3 n0;
	vec3 c0;
	vec3 c1;
	vec3 c2;

	vec3 temp1n; // for rendering normals
	vec3 temp2n;

	TriClassSmooth(); // constructor
	TriClassSmooth(vec3 v0, vec3 v1, vec3 v2, vec3 color0, vec3 color1, vec3 color2); // constructor
	void computeNormal();
	//void renderWithNormals();
	//void renderTheNormals();
	void renderWithSmooth();

};

class TriClass
{
public:

	vec3 p0;
	vec3 p1;
	vec3 p2;
	vec3 n0;

	vec3 temp1n; // for rendering normals
	vec3 temp2n;

	TriClass(); // constructor
	TriClass(vec3 v0, vec3 v1, vec3 v2); // constructor
	void computeNormal();
	void renderWithNormals();
	void renderTheNormals();
};

class QuadClass
{
public:

	vec3 p0;
	vec3 p1;
	vec3 p2;
	vec3 p3;
	vec3 n0;

	vec3 temp1n; // for rendering normals
	vec3 temp2n;

	QuadClass(); // constructor
	QuadClass(vec3 v0, vec3 v1, vec3 v2, vec3 v3 ); // constructor
	void computeNormal();
	void renderWithNormals();
	void renderTheNormals();
};

class TriPrismClass
{
public:

	TriClass *tri1;
	TriClass *tri2;
	QuadClass *quad1;
	QuadClass *quad2;
	QuadClass *quad3;

	TriPrismClass(); // constructor
	~TriPrismClass(); // destructor
	TriPrismClass(GLfloat height, GLfloat width, GLfloat length); // constructor
	void makeIt(GLfloat height, GLfloat width, GLfloat length);
	void computeNormal();
	void renderWithNormals();
	void renderTheNormals();
};

#endif
