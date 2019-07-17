// Stack of textured cubes

// Digital Elevation Model

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"
#include "vec3.h"
#include "fileOperations.h"

#ifndef CubesClassz_def 
#define CubesClassz_def

void readTextureFile(); // static load routine prototype

class Cubes1Class
{
// single textured cube

public:
	int texId; 
	Cubes1Class(); // constructor
	void render(); // draw it
	void renderWithTex();
	void renderTheNormals();
};


class CubesClass
{
// 1,2,3 or more textured cubes
#define MAX_NUM_CUBES 10
public:

	int numOfCubes;

    Cubes1Class *lst[MAX_NUM_CUBES]; // the cubes
	GLfloat angle[MAX_NUM_CUBES]; // the cubes rotations
	
	CubesClass(int num);   // constructor
	~CubesClass();   // destructor
	void countDown(GLfloat unTwist);
	void windUp(GLfloat twist);
	void render();  // draw it
};

#endif
// end 