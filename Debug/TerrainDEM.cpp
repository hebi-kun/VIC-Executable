// class DEMTerrain

#include "TerrainDEM.h"

DEMTerrain::DEMTerrain() // constructor
{
	xDist=1;
	zDist=1;
	minZVal=0;
	maxZVal=0;

	waterColour = vec3(0.1f,0.1f,0.6f);
	highColour = vec3(0.8f,0.2f,0.1f);
	lowColour = vec3(0,0.8f,0.2f);
	normalColour = vec3(0.2, 0.2, 0.2);
}

int DEMTerrain::readFile(char *fname)
{
	// see fileoperations.cpp for where its stored
	int rc = readHeightField(findFile(fname));
	return rc;
}

void DEMTerrain::scaleValues(float f)
{
	for(int z=0;z<heightField_depth; z++)
	{
		for(int x=0;x<heightField_width; x++)
		{
		heightField[z][x] = heightField[z][x] *f;
		}
	}
}

void DEMTerrain::displayCurrentValues()
{
	printf("Width=%d   Depth=%d \n",heightField_width,heightField_depth);
	for(int z=0;z<heightField_depth; z++)
	{
		for(int x=0;x<heightField_width; x++)
		{
		printf("%7.2f ",heightField[z][x]);
		}
		printf("\n\n");
	}
	printf("\nWidth=%d   Depth=%d \n",heightField_width,heightField_depth);
	printf("\nMinZ=%9.3f   MaxZ=%9.3f \n",minZVal,maxZVal);
}

void DEMTerrain::setMinMax()
{
	minZVal=heightField[0][0];
	maxZVal=heightField[0][0];
	for(int z=0;z<heightField_depth; z++)
	{
		for(int x=0;x<heightField_width; x++)
		{
		if (heightField[z][x] < minZVal) minZVal = heightField[z][x];
		if (heightField[z][x] > maxZVal) maxZVal = heightField[z][x];
		}
	}

}

GLfloat DEMTerrain::linear_interp(GLfloat vMin, GLfloat vMax, GLfloat val, GLfloat tMin, GLfloat tMax)
// linear interpolation (student simplified code)
//vMin and vMax or the original value range
// val is the subject value
// tMin and tMax are the target subject range
{
   GLfloat a = vMax - vMin; //find val range 
   GLfloat v = val - vMin;  // adjust for base 
   GLfloat temp = v / a;	//create a value between 0 and 1 - could have a prob if a==0
   GLfloat b = tMax-tMin;   // get target range 
   GLfloat retv = temp * b + tMin; // lol std liner equasion y=ax+b
   return retv;
}


void DEMTerrain::setHeightColour(GLfloat height)
{
	// --- Put your code here ---------------------------------------
	
	if (height > 2)
		glColor3fv(highColour.fv());
	else
	{
		if (height > 0) {
			glColor3fv(lowColour.fv());
		}

		else {
			glColor3fv(waterColour.fv());
		}

	}
	
	// ------------- end --------------------------------------------
	
}


void DEMTerrain::renderDots()
{
	GLfloat xx,yy,zz;

	glBegin(GL_POINTS);
	for(int z=0;z<heightField_depth; z++)
	{
		for(int x=0;x<heightField_width; x++)
		{
		xx=x*xDist;
		yy=heightField[z][x];
		zz=z*zDist;
		setHeightColour(yy);
		glVertex3f(xx,yy,zz);
		}
	}
	glEnd();
}

void DEMTerrain::computeTriNormalsVal()
{
	polygonNormal(n0, vertex[0], vertex[2], vertex[1]);
}

void DEMTerrain::renderWater()
{
	GLfloat xx = heightField_depth / heightField_width;
	GLfloat zz = heightField_depth / heightField_width;
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	// --- Put your code here ---------------------------------------
	//
	for (int z = 0; z<heightField_depth; z++)
		for (int x = 0; x<heightField_width; x++)
		{
			glBegin(GL_TRIANGLES);
			glColor3fv(waterColour.fv());
			glVertex3f(xx*x, 0.1, zz*z);
			glVertex3f(xx*x + xx, 0.1, zz*z);
			glVertex3f(xx*x, 0.1, zz*z + zz);
			glVertex3f(xx*x + xx, 0.1, zz*z + zz);
			glVertex3f(xx*x, 0.1, zz*z + zz);
			glVertex3f(xx*x + xx, 0.1, zz*z);
			glEnd();
		}

	// ------------- end --------------------------------------------
	
}

void DEMTerrain::renderWaterLine()
{
	GLfloat xx = heightField_depth / heightField_width;
	GLfloat zz = heightField_depth / heightField_width;
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	// --- Put your code here ---------------------------------------
	//
	for (int z = 0; z<heightField_depth; z++)
		for (int x = 0; x<heightField_width; x++)
		{
			glBegin(GL_TRIANGLES);
			glColor3fv(waterColour.fv());
			glVertex3f(xx*x, 0.1, zz*z);
			glVertex3f(xx*x + xx, 0.1, zz*z);
			glVertex3f(xx*x, 0.1, zz*z + zz);
			glVertex3f(xx*x + xx, 0.1, zz*z + zz);
			glVertex3f(xx*x, 0.1, zz*z + zz);
			glVertex3f(xx*x + xx, 0.1, zz*z);
			glEnd();
		}

	// ------------- end --------------------------------------------

}

void DEMTerrain::renderTheNormals()
{
	temp1n.x = (vertex[0].x + vertex[1].x + vertex[2].x) / 3.0f; // for rendering normals
	temp1n.y = (vertex[0].y + vertex[1].y + vertex[2].y) / 3.0f; // for rendering normals
	temp1n.z = (vertex[0].z + vertex[1].z + vertex[2].z) / 3.0f; // for rendering normals
	temp2n = temp1n + n0;
	glBegin(GL_LINES);
	glColor3fv(normalColour.fv());
	glVertex3fv(temp1n.fv());
	glVertex3fv(temp2n.fv());

	glEnd();
}

void DEMTerrain::renderSmooth()
{
	//GLfloat xx0,yy0,zz0;
	GLfloat xx1, yy1, zz1;
	GLfloat xx2, yy2, zz2;
	GLfloat xx3, yy3, zz3;

	GLfloat xx0 = heightField_depth / heightField_width;
	GLfloat zz0 = heightField_depth / heightField_width;
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_BACK, GL_FILL);
	glPolygonMode(GL_FRONT, GL_FILL);

	// --- Put your code here ---------------------------------------
	//
	for (int x = 0; x < heightField_depth - 1; x++)
	{
		for (int z = 0; z < heightField_width - 1; z++)
		{
			glBegin(GL_TRIANGLES);

			vertex[0] = vec3(xx0*x, heightField[x][z], zz0*z);
			vertex[1] = vec3(xx0*x + xx0, heightField[x + 1][z], zz0*z);
			vertex[2] = vec3(xx0*x, heightField[x][z + 1], zz0*z + zz0);

			glNormal3fv(n0.fv());
			setHeightColour(heightField[x][z]);
			glVertex3f(xx0*x, heightField[x][z], zz0*z);
			setHeightColour(heightField[x + 1][z]);
			glVertex3f(xx0*x + xx0, heightField[x + 1][z], zz0*z);
			setHeightColour(heightField[x][z + 1]);
			glVertex3f(xx0*x, heightField[x][z + 1], zz0*z + zz0);
			computeTriNormalsVal();


			vertex[0] = vec3(xx0*x + xx0, heightField[x + 1][z + 1], zz0*z + zz0);
			vertex[1] = vec3(xx0*x, heightField[x][z + 1], zz0*z + zz0);
			vertex[2] = vec3(xx0*x + xx0, heightField[x + 1][z], zz0*z);

			glNormal3fv(n0.fv());
			setHeightColour(heightField[x + 1][z + 1]);
			glVertex3f(xx0*x + xx0, heightField[x + 1][z + 1], zz0*z + zz0);
			setHeightColour(heightField[x][z + 1]);
			glVertex3f(xx0*x, heightField[x][z + 1], zz0*z + zz0);
			setHeightColour(heightField[x + 1][z]);
			glVertex3f(xx0*x + xx0, heightField[x + 1][z], zz0*z);
			computeTriNormalsVal();

			glEnd();
		}
	}

}

void DEMTerrain::renderSmoothLine()
{
	//GLfloat xx0,yy0,zz0;
	GLfloat xx1, yy1, zz1;
	GLfloat xx2, yy2, zz2;
	GLfloat xx3, yy3, zz3;

	GLfloat xx0 = heightField_depth / heightField_width;
	GLfloat zz0 = heightField_depth / heightField_width;
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_BACK, GL_LINE);
	glPolygonMode(GL_FRONT, GL_LINE);

	// --- Put your code here ---------------------------------------
	//
	for (int x = 0; x < heightField_depth - 1; x++)
	{
		for (int z = 0; z < heightField_width - 1; z++)
		{
			glBegin(GL_TRIANGLES);

			vertex[0] = vec3(xx0*x, heightField[x][z], zz0*z);
			vertex[1] = vec3(xx0*x + xx0, heightField[x + 1][z], zz0*z);
			vertex[2] = vec3(xx0*x, heightField[x][z + 1], zz0*z + zz0);

			glNormal3fv(n0.fv());
			setHeightColour(heightField[x][z]);
			glVertex3f(xx0*x, heightField[x][z], zz0*z);
			setHeightColour(heightField[x + 1][z]);
			glVertex3f(xx0*x + xx0, heightField[x + 1][z], zz0*z);
			setHeightColour(heightField[x][z + 1]);
			glVertex3f(xx0*x, heightField[x][z + 1], zz0*z + zz0);
			computeTriNormalsVal();


			vertex[0] = vec3(xx0*x + xx0, heightField[x + 1][z + 1], zz0*z + zz0);
			vertex[1] = vec3(xx0*x, heightField[x][z + 1], zz0*z + zz0);
			vertex[2] = vec3(xx0*x + xx0, heightField[x + 1][z], zz0*z);

			glNormal3fv(n0.fv());
			setHeightColour(heightField[x + 1][z + 1]);
			glVertex3f(xx0*x + xx0, heightField[x + 1][z + 1], zz0*z + zz0);
			setHeightColour(heightField[x][z + 1]);
			glVertex3f(xx0*x, heightField[x][z + 1], zz0*z + zz0);
			setHeightColour(heightField[x + 1][z]);
			glVertex3f(xx0*x + xx0, heightField[x + 1][z], zz0*z);
			computeTriNormalsVal();

			glEnd();
		}
	}

}
void DEMTerrain::rendNorm() 
{
	GLfloat xx0 = heightField_depth / heightField_width;
	GLfloat zz0 = heightField_depth / heightField_width;
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_BACK, GL_FILL);
	glPolygonMode(GL_FRONT, GL_FILL);
	for (int x = 0; x < heightField_depth - 1; x++)
	{
		for (int z = 0; z < heightField_width - 1; z++)
		{
			glNormal3fv(n0.fv());
			vertex[0] = vec3(xx0*x, heightField[x][z], zz0*z);
			vertex[1] = vec3(xx0*x + xx0, heightField[x + 1][z], zz0*z);
			vertex[2] = vec3(xx0*x, heightField[x][z + 1], zz0*z + zz0);

			computeTriNormalsVal();
			renderTheNormals();

			glNormal3fv(n0.fv());
			vertex[0] = vec3(xx0*x + xx0, heightField[x + 1][z + 1], zz0*z + zz0);
			vertex[1] = vec3(xx0*x, heightField[x][z + 1], zz0*z + zz0);
			vertex[2] = vec3(xx0*x + xx0, heightField[x + 1][z], zz0*z);

			computeTriNormalsVal();
			renderTheNormals();

		}
	}
	// ------------- end --------------------------------------------
}

// end