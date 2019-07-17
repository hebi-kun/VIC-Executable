#ifndef cubeclass_def 
#define cubeclass_def 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"


class CubeClass
{
public:
	CubeClass(); // constructor
	void render(); // draw it
	void renderCW(); // draw it CW
	void renderCCW(); // draw it CCW
	void renderWithNormals();
	void renderTheNormals();

};

#endif
