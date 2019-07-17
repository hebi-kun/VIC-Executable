
/****	First 3D program : the spinning cube
	Written by Rob Cox and Hugh Fisher				****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"

//#include "utility.h"
//#include "glUtils.h"

#include "Axes.h"
#include "CubeClass.h"
#include "cubes.h"
#include "triangularPrism.h"
#include "2DText.h"
#include "grid.h"
#include "TerrainDEM.h"

#pragma warning(disable : 4996) // Reduce unsafe warning messages

#define framesPerSecond 25
#define timerMSecs ((int)1000/framesPerSecond)  // compute time in miliseconds for a frame
bool timeForNextFrame=false;

#define ESC	  27

#define cmdRed		 1
#define cmdGreen	 2
#define cmdExit		99

static int	WinWidth, WinHeight;
static int	AppMenu;

static GLfloat	spinAngle;
GLfloat spinIncrement=0.4f; // slow

static GLfloat	eyePoint[3];
static GLfloat  delta2 = 5; // bigger steps 
static GLfloat	lookAtPoint[3];
static GLfloat	upVec[3];//
static GLfloat  fov; // field of view
static GLfloat	nearPlane;
static GLfloat	farPlane;

bool autoMove = false;
GLfloat speed = 0.5;
GLfloat prox = 0.2;
GLfloat proz = 0.8;
GLfloat shipx = 0;
GLfloat shipz = 30;

static GLfloat	color3[3];

static GLfloat	red[]		= {1.0, 0.0, 0.0};
static GLfloat	blue[]		= {0.0, 1.0, 0.0};
static GLfloat	green[]		= {0.0, 0.0, 1.0};
static GLfloat	cyan[]		= {0.0, 1.0, 1.0};
static GLfloat	magenta[]	= {1.0, 0.0, 1.0};
static GLfloat	yellow[]	= {1.0, 1.0, 0.0};
static GLfloat	black[]		= {0.0, 0.0, 0.0};
static GLfloat	white[]		= {1.0, 1.0, 1.0};

CubeClass *cube; // pointer to a cube object
CubesClass *cube1; // pointer to a cube1 object
GLUquadricObj *quadric1;    
GLUquadricObj *quadric2;  
GLUquadricObj *quadric3;    
GLUquadricObj *quadric4;    
Axes *axes; //
Grid *grid; // To help with push and scale etc
TriClass *triangle;
QuadClass *quad;
TriPrismClass *triPrism;

DEMTerrain *ter;

int showGrid=TRUE;
int showAxes=TRUE;
int showNormals=TRUE;
int showWireFrame=FALSE;
int showSpin=FALSE;
int showPrimitive=0;
int showTerrain=1;
int showTower=1;
int showShip = 1;

int lighting = false;
int ambientlighting = true;
int sunlighting = false;
int streetlighting = false;

int hiddenDepth=TRUE;
int hiddenCull=FALSE;
int clockWise=FALSE; // start CCW
GLfloat angle = 0;
GLfloat angleVar = 0;

int mode=2; // mode 2 = 2D, 3=3D , 5=Both
Text2D *drawText2d;
char dispString[255] = "2=2D (also F1-help)  3=3D  5=2D+3D";

// lighting and materials variables

float ambientLight[4] = {0.35f, 0.35f, 0.35f, 1.0f}; // colour
float ambientLightOff[4] = {0.0f, 0.0f, 0.0f, 1.0f}; // colour

float sunLightPos[4] = { 1,150,10,1 }; // distant above
float sunLightDif[4] = { 1,1,1,1 };
float sunLightSpec[4] = { 1,1,1,1 }; // colour
float sunLightAmb[4] = { 0.1,0.1,0.1,1 }; // colour


// House 
//float	dif1[]    = { 0.5,0,0,1 };
//float	amb1[]    = { 0.0,0.0,0.0,1 };
//float	spe1[]    = { 0.7,0.6,0.6,1 };

float	dif0[] = { 0.3,0.3,0.1,1 };
float	amb0[] = { 0.3,0.3,0.1,1 };
float	spe0[] = { 0.3,0.3,0.1,1 };

float	dif1[] = { 0.1,0.3,0.1,1 };
float	amb1[] = { 0.1,0.3,0.1,1 };
float	spe1[] = { 0.1,0.3,0.1,1 };

float	dif2[] = { 0.2,0.5,0.2,1 };
float	amb2[] = { 0.2,0.5,0.2,1 };
float	spe2[] = { 0.2,0.5,0.2,1 };

float	dif3[] = { 0.2,0.2,0.6,1 };
float	amb3[] = { 0.2,0.2,0.6,1 };
float	spe3[] = { 0.2,0.2,0.6,1 };

float	dif4[] = { 0.1,0.1,0.1,1 };
float	amb4[] = { 0.1,0.1,0.1,1 };
float	spe4[] = { 0.1,0.1,0.1,1 };

float	dif5[] = { 0.1,0.8,0.1,1 };
float	amb5[] = { 0.1,0.6,0.1,1 };
float	spe5[] = { 0.1,0.8,0.1,1 };

//float   shiny1    = (int)128 * 0.21794872;
float   shiny1 = 30;

// Function prototypes needed because they are used before its declared
void drawScene();

/****		Window events		****/

static void seteyePoint()
{
  glLoadIdentity();
  gluLookAt(eyePoint[0], eyePoint[1], eyePoint[2],
	    lookAtPoint[0],lookAtPoint[1],lookAtPoint[2],
	    upVec[0],upVec[1],upVec[2]);
}

static void setProjection3D()
{
  GLfloat aspect;
  /* Set projection Mode */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  aspect = (float)WinWidth / (float)WinHeight;
  gluPerspective(fov, aspect, nearPlane, farPlane);
  /* Back to normal */
  glMatrixMode(GL_MODELVIEW);
  seteyePoint();

  if (hiddenDepth)
  {
	// Now do z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthRange(nearPlane,farPlane);
	glClearDepth(farPlane);
  }
  else
  {
	glDisable(GL_DEPTH_TEST);
  }

  if (hiddenCull)
  {
	  glEnable(GL_CULL_FACE);
	  glCullFace(GL_BACK);
	  if (clockWise) glFrontFace(GL_CW);
	  else glFrontFace(GL_CCW);
  }
  else
  {
	  glDisable(GL_CULL_FACE);
  }
}

static void setProjection2D()
{
  //GLfloat aspect;
  /* Set projection Mode */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //aspect = (float)WinWidth / (float)WinHeight;
  glOrtho(0, WinWidth, 0, WinHeight, -1.0f, 1.0f);
  /* Back to normal but identity*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);
}
//----------------------Movement-------------------------------
void MovePlane2D(GLfloat oldx, GLfloat oldz,
	GLfloat angle, GLfloat speed, // angle in degrees
	GLfloat *resultx, GLfloat *resultz)
{
	GLfloat tmpx, tmpz;
	GLfloat newx, newz;
	float yaw; // yaw is an angle in radians

	yaw = angle*3.1416 / 180;


	tmpx = 0;/* set to origin */
	tmpz = speed; /* move us forward by speed */

	newx = (tmpz * sin(yaw)) + (tmpx * cos(yaw));
	newz = (tmpz * cos(yaw)) - (tmpx * sin(yaw));
	tmpx = newx;
	tmpz = newz;

	newx = tmpx + oldx; // adjust because it was around the origin
	newz = tmpz + oldz;

	*resultx = newx;
	*resultz = newz;
}


// --------------------- materials ----------------------------

void setMaterial(int m)
{
if (m == 0)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb0);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif0);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spe0);
		glMaterialf(GL_FRONT, GL_SHININESS, shiny1);
	}
if (m==1) 
	{
	glMaterialfv(GL_FRONT,GL_AMBIENT,amb1);	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,dif1);	
	glMaterialfv(GL_FRONT,GL_SPECULAR,spe1);	
	glMaterialf(GL_FRONT,GL_SHININESS,shiny1);
	}
if (m==2) 
	{
	glMaterialfv(GL_FRONT,GL_AMBIENT,amb2);	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,dif2);	
	glMaterialfv(GL_FRONT,GL_SPECULAR,spe2);	
	glMaterialf(GL_FRONT,GL_SHININESS,shiny1);
	}
if (m==3) 
	{
	glMaterialfv(GL_FRONT,GL_AMBIENT,amb3);	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,dif3);	
	glMaterialfv(GL_FRONT,GL_SPECULAR,spe3);	
	glMaterialf(GL_FRONT,GL_SHININESS,shiny1);
	}
if (m==4) 
	{
	glMaterialfv(GL_FRONT,GL_AMBIENT,amb4);	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,dif4);	
	glMaterialfv(GL_FRONT,GL_SPECULAR,spe4);	
	glMaterialf(GL_FRONT,GL_SHININESS,shiny1);
	}
if (m==5) 
	{
	glMaterialfv(GL_FRONT,GL_AMBIENT,amb5);	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,dif5);	
	glMaterialfv(GL_FRONT,GL_SPECULAR,spe5);	
	glMaterialf(GL_FRONT,GL_SHININESS,shiny1);
	}

}

void setLight(int l)
{
	if (l==0)
	{
		glLightfv(GL_LIGHT0,GL_POSITION,sunLightPos);
		glLightfv(GL_LIGHT0,GL_AMBIENT,sunLightAmb);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,sunLightDif);
		glLightfv(GL_LIGHT0,GL_SPECULAR,sunLightSpec);
	}
	//if (l==1)
	//{
	//	glLightfv(GL_LIGHT1,GL_POSITION,streetLightPos);
	//	glLightfv(GL_LIGHT1,GL_AMBIENT,streetLightAmb);
	//	glLightfv(GL_LIGHT1,GL_DIFFUSE,streetLightDif);
	//	glLightfv(GL_LIGHT1,GL_SPECULAR,streetLightSpec);
	//}

}

// ---------------------- primitives ------------------------

void drawCubeWithNormals()
{
	glPushMatrix();
	cube->renderWithNormals();
	if (showNormals) {
		cube->renderTheNormals();
	}
	glPopMatrix();
}

void drawCylinder(GLenum glunormals, GLfloat cir1, GLfloat cir2, GLfloat hnum)
{	
	//
	// glunormals values are GLU_NONE, GLU_FLAT, GLU_SMOOTH.
	// you may also need to set glShadeModel(GLenum  mode) to GL_FLAT or GL_SMOOTH
	//
	glPushMatrix();
	//glTranslatef(0,0,0);
	gluQuadricOrientation(quadric1,GLU_OUTSIDE);
	gluQuadricNormals(quadric1, glunormals);   
	gluCylinder(quadric1,cir1,cir2,hnum,17,4);
	glPopMatrix();
}

void drawDisk(GLfloat innerRadius, GLenum glunormals) 
{	
	// inner raduis 0 = full disk 0.5 = no disk or circle
	glPushMatrix();
	gluQuadricOrientation(quadric3,GLU_OUTSIDE);
	gluQuadricNormals(quadric3, glunormals);
	gluDisk(quadric3,innerRadius,0.5,17,4);
	glPopMatrix();
}

void drawCone(GLfloat smallEndRadius,GLenum  glunormals) 
{	
	glPushMatrix();
	glTranslatef(0,0,-0.5);
	gluQuadricOrientation(quadric4,GLU_OUTSIDE);
	gluQuadricNormals(quadric4, glunormals);
	gluCylinder(quadric4,0.5,smallEndRadius,1,17,4);
	glPopMatrix();
}

void drawSphere(GLenum glunormals)
{	
	glPushMatrix();
	gluQuadricOrientation(quadric2,GLU_OUTSIDE);
	gluQuadricNormals(quadric2, glunormals);
	gluSphere(quadric2,0.5,13,13);
	glPopMatrix();
}

void drawTriangle()
{
	glPushMatrix();
	triangle->renderWithNormals();
	//triangle->renderTheNormals();
	glPopMatrix();
}

void drawQuad()
{
	glPushMatrix();
	quad->renderWithNormals();
	quad->renderTheNormals();
	glPopMatrix();
}

void drawTriPrism()
{
	glPushMatrix();
	triPrism->renderWithNormals();
	//triPrism->renderTheNormals();
	glPopMatrix();
}


void drawPrimitive()
{
	glPushMatrix();
	if (showPrimitive == 1) drawSphere(GLU_FLAT);
	if (showPrimitive == 2) drawCubeWithNormals();
	if (showPrimitive == 3) drawCylinder(GLU_FLAT, 0.4, 0.5, 0.2); 
	if (showPrimitive == 4) drawDisk(0.2f,GLU_FLAT); 
    if (showPrimitive == 5) drawQuad();
	if (showPrimitive == 7) drawTriPrism();
    if (showPrimitive == 6) drawTriangle();
	glPopMatrix();
}

//----------------------------------

void drawCylinder()
{
	glPushMatrix();
	glTranslatef(0, 0, -0.5);
	gluCylinder(quadric1, 0.5, 0.5, 1, 17, 4);
	glPopMatrix();
}

void drawBase()
{
	//Top Half Cone 
	setMaterial(5);
	glPushMatrix();
	glColor3f(0.6,0.6,0.6);
	glRotatef(270, 1, 0, 0);
	glTranslatef(0, 0, 1.5);
	glScalef(7, 7, 1);
	gluCylinder(quadric1, 0.5, 0.2, 1, 17, 1);
	glPopMatrix();

	//Bottom Half Cone
	setMaterial(5);
	glPushMatrix();
	glColor3f(0.6, 0.6, 0.6);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -0.5);
	glScalef(7, 7, 1);
	gluCylinder(quadric1, 0.5, 0.3, 1, 17, 1);
	glPopMatrix();

	//Middle 
	setMaterial(4);
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -1.5);
	glScalef(7, 7, 1);
	gluCylinder(quadric1, 0.5, 0.5, 1, 17, 1);
	glPopMatrix();


}

void drawBaseCone()
{

	//Top Half Cone 
	setMaterial(5);
	glPushMatrix();
	glColor3f(0.6, 0.6, 0.6);
	glRotatef(270, 0, 1, 0);
	glTranslatef(0, 1, 4);
	glScalef(7, 7, 0.5);
	gluCylinder(quadric1, 0.5, 0.2, 1, 17, 1);
	glPopMatrix();

	//Bottom Half Cone
	setMaterial(5);
	glPushMatrix();
	glColor3f(0.6, 0.6, 0.6);
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 1, -3.5);
	glScalef(7, 7, 0.5);
	gluCylinder(quadric1, 0.5, 0.2, 1, 17, 1);
	glPopMatrix();

	//Middle 
	setMaterial(4);
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 1, -4);
	glScalef(7, 7, 0.5);
	gluCylinder(quadric1, 0.5, 0.5, 1, 17, 1);
	glPopMatrix();


}


void drawProjectile() {

	setMaterial(3);
	glPushMatrix();
	glRotatef(130, 1, 0, 0);
	glTranslatef(0, prox, proz);
	glScalef(2, 2, 1);
	gluCylinder(quadric1, 0.3, 0.3, 1, 17, 3);
	glPopMatrix();

}

void drawCannon() {
	setMaterial(1);
	glPushMatrix();
	glRotatef(130, 1, 0, 0);
	glTranslatef(0, 0.2, 0.8);
	glScalef(2, 2, 1);
	gluCylinder(quadric1, 0.3, 0.3, 3, 17, 3);
	glPopMatrix();

	glPushMatrix();
	drawProjectile();
	glPopMatrix();

} 

void drawBaseBottom()
{
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, 0.5);
	glScalef(5, 5, 1);
	gluCylinder(quadric1, 0.3, 0.2, 1, 17, 1);
	glPopMatrix();

	glPushMatrix();
	drawCannon();
	glPopMatrix();


}

void drawConeShield()
{
	glPushMatrix();
	glRotatef(270, 0, 1, 0);
	glTranslatef(0, 1, 4.5);
	glScalef(5, 5, 0.5);
	gluCylinder(quadric1, 0.3, 0.2, 1, 17, 1);
	glPopMatrix();
}

void drawConeShield2()
{
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 1, 4.5);
	glScalef(5, 5, 0.5);
	gluCylinder(quadric1, 0.3, 0.2, 1, 17, 1);
	glPopMatrix();
}


void drawDisk(GLfloat innerRadius) // inner raduis 0 = full disk 0.5 = no disk or circle
{
	glPushMatrix();
	gluDisk(quadric3, innerRadius, 0.5, 17, 4);
	glPopMatrix();
}

void drawSphere()
{
	glPushMatrix();
	gluSphere(quadric2, 0.5, 13, 13);
	glPopMatrix();
}
void drawAntenna(GLfloat size)
{
	setMaterial(5);
	glPushMatrix();
	glScalef(0.2, 0.2, size);
	gluCylinder(quadric1, 0.3, 0.3, 1, 17, 1);
	glPopMatrix();
}

void drawTree()
{
	glPushMatrix();
	glTranslatef(0,1,0);
	glRotatef(270, 1, 0, 0);
	gluCylinder(quadric1, 0.6, 0, 1, 17, 1);
	glPopMatrix();

	setMaterial(0);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	gluCylinder(quadric1, 0.2, 0.2, 1, 17, 1);
	glPopMatrix();
}

void drawSStation1()
{
	glPushMatrix();
	drawBase();
	glPopMatrix();

	//First Base Cone
	glPushMatrix();
	glRotatef(angleVar, 1, 0, 0);
	drawBaseCone();
	glPopMatrix();

	//Second Base Cone
	glPushMatrix();
	glRotatef(angleVar, 1, 0, 0);
	glTranslatef(7.5, 0, 0);
	drawBaseCone();
	glPopMatrix();

	//Base Bottom 
	glPushMatrix();
	drawBaseBottom();
	glPopMatrix();

	//Cone Shields
	glPushMatrix();
	drawConeShield();
	glPopMatrix();
	glPushMatrix();
	drawConeShield2();
	glPopMatrix();

	//Base Cube
	glPushMatrix();
	glTranslatef(1, 2.5, 0);
	glScalef(1, 2, 2);
	drawCubeWithNormals();
	glPopMatrix();

	//Base Cylinder
	glPushMatrix();
	glTranslatef(-0.5, 2.5, -0.5);
	glScalef(2, 2, 2);
	glRotatef(270, 1, 0, 0);
	gluCylinder(quadric1, 0.3, 0.3, 1, 17, 1);
	glPopMatrix();

	//Antennas
	//Top Antennas
	glPushMatrix();
	glTranslatef(0.5, 2.5, 1);
	glRotatef(270, 1, 0, 0);
	drawAntenna(3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 2.5, 1);
	glRotatef(270, 1, 0, 0);
	drawAntenna(3);
	glPopMatrix();

	//Second Antennas

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 1.5, 4.5);
	drawAntenna(2);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0.5, 4.5);
	drawAntenna(1);
	glPopMatrix();

	//Third Antennas

	glPushMatrix();
	glRotatef(270, 0, 1, 0);
	glTranslatef(0, 1.5, 4.5);
	drawAntenna(2);
	glPopMatrix();

	glPushMatrix();
	glRotatef(270, 0, 1, 0);
	glTranslatef(0, 0.5, 4.5);
	drawAntenna(1);
	glPopMatrix();

	// Fourth Antennas

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0.5, 0, 0.5);
	drawAntenna(3);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(-0.5, 0, 0.5);
	drawAntenna(2);
	glPopMatrix();



}

void drawForest() {
	if (showWireFrame) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	glPushMatrix();
	glTranslatef(2, 3, 5);
	for (int k = 0; k < 20; k++) {
		glPushMatrix();
		setMaterial(2);
		glTranslatef(k + 10, 0, k + 10);
		glScalef(1, 3, 1);
		drawTree();
		glPopMatrix();

	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 3, 4);
	for (int k = 0; k < 20; k++) {
		glPushMatrix();
		setMaterial(2);
		glTranslatef(k + 10, 0, k + 10);
		glScalef(1, 3, 1);
		drawTree();
		glPopMatrix();

	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 3, 3);
	for (int k = 0; k < 20; k++) {
		glPushMatrix();
		setMaterial(2);
		glTranslatef(k + 10, 0, k + 10);
		glScalef(2, 3, 2);
		drawTree();
		glPopMatrix();

	}

	glPopMatrix();

	glPushMatrix();
	glTranslatef(17, 3, 5);
	for (int k = 0; k < 20; k++) {
		glPushMatrix();
		setMaterial(2);
		glTranslatef(k + 10, 0, k + 10);
		glScalef(2, 3, 2);
		drawTree();
		glPopMatrix();

	}
	glPopMatrix();
}

void renderCity() {
	glPushMatrix();
	glScalef(4,4,2);
	drawCubeWithNormals();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1,0,1); 
	glScalef(4, 3, 3);
	drawCubeWithNormals();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 0, 2);
	glScalef(1, 2, 1);
	drawCubeWithNormals();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 0, 4);
	glScalef(2, 2, 2);
	drawCubeWithNormals();
	glPopMatrix();

}

void renderBigCity() {
	glPushMatrix();
	glTranslatef(0, 4, 0);
	renderCity();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 5, 5);
	renderCity();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 8, 8);
	renderCity();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 10, 20);
	renderCity();
	glPopMatrix();

}

void renderFullCity() {
	if (showWireFrame) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	setMaterial(4);
	glPushMatrix();
	glTranslatef(15,3,60);
	renderBigCity();
	glPopMatrix();
}




//--------------

// Main Student code start here ------------------------------------------------------

// ****************************  Put Code here **********************************************

void drawScene1()
{
glPushMatrix();
if (showTerrain)
	{
	if (showWireFrame) {
		setMaterial(3);
		ter->renderWaterLine();
		//ter->renderDots();
		setMaterial(2);
		ter->renderSmoothLine();
	}
	else {
		setMaterial(3);
		ter->renderWater();
		//ter->renderDots();
		setMaterial(2);
		ter->renderSmooth();
	}
	if (showNormals) {
		ter->rendNorm();
	}
	glPushMatrix();
	drawForest();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 15);
	drawForest();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, 0, 15);
	glRotatef(20, 0,1,0);
	drawForest();
	glPopMatrix();

	glPushMatrix();
	renderFullCity();
	glPopMatrix();

	}

if (showTower)
	{
	if (showWireFrame) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	
	glTranslatef(60, 0, 60);
	glPushMatrix();
	cube1->countDown(6);

	glPushMatrix();
	glTranslatef(0,9,0);
	glScalef(9,9,9);
	cube1->render();
	glPopMatrix();

	cube1->countDown(4);
	glPushMatrix();
	glTranslatef(0, 18, 0);
	glScalef(9, 9, 9);
	cube1->render();
	glPopMatrix();

	cube1->countDown(3);
	glPushMatrix();
	glTranslatef(0, 27, 0);
	glScalef(9, 9, 9);
	cube1->render();
	glPopMatrix();

	cube1->countDown(2);
	glPushMatrix();
	glTranslatef(0, 36, 0);
	glScalef(9, 9, 9);
	cube1->render();
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	}


if (showShip) {
	/*glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);*/
	if (showWireFrame) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
	setMaterial(1);
	glPushMatrix();
	glTranslatef(shipx, 80, shipz);
	glScalef(3, 3, 3);
	drawSStation1();
	glPopMatrix(); 
	
	}
	glPopMatrix();


}
// Main Student code end here ------------------------------------------------------

void drawScene()
{
	glPushMatrix();
	glColor3f(0,0.7f,0.7f);
	
	drawScene1();
	glPopMatrix();
}


static void drawWorld ()
{
	if (showWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* Wireframe mode */
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); /* fill mode */
	}
   
	glPushMatrix();

   if (showSpin)
	{
		glRotatef(spinAngle,0,1,0);
	}
  
 //  if(showGrid)
	//{
	//	glColor3f(0.2f,0.3f,0.2f);
	//	if (lighting) setMaterial(1);
	//	grid->render();
	//}

	
	if (showAxes)
	  {
		  axes->render();
	  }
	 
   if (lighting) {glEnable(GL_LIGHTING); glEnable(GL_NORMALIZE);} else glDisable(GL_LIGHTING);
   if (sunlighting) {glEnable(GL_LIGHT0),setLight(0);} else glDisable(GL_LIGHT0);
   if (streetlighting) {glEnable(GL_LIGHT1),setLight(1);} else glDisable(GL_LIGHT1);

   if (ambientlighting) 
   {
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
   } 
   else
   {
	   glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLightOff);
   }

   if (showPrimitive != 0) drawPrimitive();
   else drawScene();


  glPopMatrix();

}

void draw2DStuff()
{
	glColor3f(1,1,0);
	glRasterPos2f(30,60);
	
	//glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
	//drawText2d->drawstr(30, 30, "%s (%d)",dispString,45);

	drawText2d->drawstr(10, 530, "%s %8.2f,%8.2f,%8.2f, ","EyePoint",eyePoint[0],eyePoint[1],eyePoint[2]);

	drawText2d->drawstr(10, 500, "%s","2 - 2D mode, 3-3D mode");
    drawText2d->drawstr(10, 470, "%s","5 - 2D & 3D mode");
    drawText2d->drawstr(10, 440, "%s","a - axes,  w-wireframe");
    drawText2d->drawstr(10, 410, "%s","l - switch lighting on/off");
    drawText2d->drawstr(10, 380, "%s","m - ambient lowlight, s-sunlight");
    drawText2d->drawstr(10, 350, "%s","h - streetlight");
    drawText2d->drawstr(10, 320, "%s","n - render normals");
    drawText2d->drawstr(10, 290, "%s","p - render primitive");
    drawText2d->drawstr(10, 260, "%s","F1 - This help");
    drawText2d->drawstr(10, 230, "%s","Arrows and pgup/pgdown comma and dot move eyepos");
    drawText2d->drawstr(10, 200, "%s","Space = start / stop spin");
    drawText2d->drawstr(10, 170, "%s","F2 - restore start view");

	drawText2d->drawstr(10, 150, "%s","d - depth buffer");
    drawText2d->drawstr(10, 130, "%s","c - cull hidden  @-swaps CW/CCW");
    drawText2d->drawstr(10, 110, "%s","/ - spin speed");
    drawText2d->drawstr(10, 90 , "%s","+,- and = sets angle variable");
	drawText2d->drawstr(10, 70, "%s", "g - shoot projectile");
	drawText2d->drawstr(10, 50, "%s", "o,i - move ship");
  }


void CheckGL ()
{
  int  err;
  char msg[256];

  err = glGetError();
  if (err != GL_NO_ERROR) {
    sprintf(msg, "OpenGL error: %s \n", (char *)gluErrorString(err));
    printf(msg);
	getchar();
	exit(1);
  }
}

static void display()
{
  if (hiddenDepth) {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}
  else {glClear(GL_COLOR_BUFFER_BIT);}

  if (mode == 3 || mode == 5)
	{
		setProjection3D();
		seteyePoint();
		drawWorld();
	}

  if (mode == 2 || mode == 5)
	{
		setProjection2D();
		draw2DStuff();
	}
  /* Check everything OK and update screen */
  CheckGL();
  glutSwapBuffers();
}

static void resize (int width, int height)
{
  /* Save for event handlers */
  WinWidth  = width;
  WinHeight = height;
  
  /* Reset view in window. */
  glViewport(0, 0, WinWidth, WinHeight);
}


/****		User events		****/


static void menuChoice (int item)
{
  switch (item) {
    case cmdRed:
        color3[0]=1; // R
        color3[1]=0; // G
        color3[2]=0; // B
      break;
    case cmdGreen:
        color3[0]=0; // R
        color3[1]=1; // G
        color3[2]=0; // B
      break;
    case cmdExit:
      exit(0);
      break;
    default:
      break;
  }
}


/* In most GUI systems we would write just one event handler
   for all kinds of keystrokes. In GLUT, we need one for the
   standard ASCII keys and one for special cursor or function
   style keys that vary from system to system. Because the
   GLUT special key code range overlaps with ASCII lowercase,
   it isn't safe to use the same function for both.        */

static void asciiKey (unsigned char key, int x, int y)
{
  if (key == ESC) menuChoice(cmdExit);

  if (key =='2') {mode = 2; strcpy(dispString,"2D Mode");}
  if (key =='3') {mode = 3; strcpy(dispString,"3D Mode");}
  if (key =='5') {mode = 5; strcpy(dispString,"2D+3D Mode");}
  //if (key =='g') {showGrid=!showGrid;}
  if (key =='a') { showAxes=!showAxes;}
  if (key =='n') { showNormals=!showNormals;}
  if (key =='w') { showWireFrame=!showWireFrame;}
  if (key =='p') { showPrimitive=showPrimitive+1;if(showPrimitive>7) showPrimitive=0;}
  if (key ==' ') { showSpin=!showSpin;}

  if (key =='d') {hiddenDepth=!hiddenDepth;}
  if (key =='c') {hiddenCull=!hiddenCull;}
  if (key =='@') {clockWise=!clockWise;}
  if (key =='/') {spinIncrement=spinIncrement+1; if (spinIncrement > 5) {spinIncrement=0.4f;}}

  if (key =='=') {if (angle < 50) angle = 90; else angle = 0;}
  if (key =='+') {angle = angle + 10; angle = (GLfloat)((int)angle % 360);}
  if (key =='-') {angle = angle - 10; angle = (GLfloat)((int)angle % 360);}

  if (key =='.') {eyePoint[0]=eyePoint[0]+2;eyePoint[2]=eyePoint[2]+2;}
  if (key ==',') {eyePoint[0]=eyePoint[0]-2;eyePoint[2]=eyePoint[2]-2;}

  if (key =='l') {lighting = !lighting;}
  if (key =='m') {ambientlighting = !ambientlighting;}
  if (key =='s') {sunlighting = !sunlighting;}
  if (key =='h') {streetlighting = !streetlighting;}
  if (key == 'g') { 
	  prox = 0.2;
	  proz = 0.8;
	  autoMove = !autoMove;
	  if (autoMove)
	  {
		  speed = 0.5;
	  }
	  else
	  {
		  speed = 0.5;
	  }
	  //MovePlane2D(prox, proz, 20.0, speed, &prox, &proz); 
	  
	}
  if (key == 'o') {
	  MovePlane2D(shipx, shipz, 0, 5, &shipx, &shipz);

  }

  if (key == 'i') {
	  MovePlane2D(shipx, shipz, 180, 5, &shipx, &shipz);

  }

}

void setStartValues()
{
  eyePoint[0] = 200; // x
  eyePoint[1] = 200; // y
  eyePoint[2] = 40.0; // z
  
  lookAtPoint[0]=0;
  lookAtPoint[1]=0;
  lookAtPoint[2]=0;

  upVec[0]=0;
  upVec[1]=1;
  upVec[2]=0;

  fov=60; // field of view
  nearPlane=0.5;
  farPlane=400;

  spinAngle = 0.0;
}

static void specialKey (int key, int x, int y)
{
  
  if (key==GLUT_KEY_F1) {mode = 2;}

  if (key==GLUT_KEY_F2)
  {
  setStartValues();
  }  
  
  if (key==GLUT_KEY_F3)
  {
  eyePoint[0] = 5; // x
  eyePoint[1] = 5; // y
  eyePoint[2] = 5; // z

  lookAtPoint[0]=0;
  lookAtPoint[1]=0;
  lookAtPoint[2]=0;
  }
  
  if (key==GLUT_KEY_F4)
  {
	  cube1->windUp(4);
  }
  
  if (key==GLUT_KEY_F5)
  {
	  showTerrain=!showTerrain;
  }
  
  if (key==GLUT_KEY_F6)
  {
	  showTower = ! showTower;
  }

  if (key==GLUT_KEY_LEFT) eyePoint[0]=eyePoint[0]+delta2;
  if (key==GLUT_KEY_RIGHT) eyePoint[0]=eyePoint[0]-delta2;
  if (key==GLUT_KEY_UP) eyePoint[2]=eyePoint[2]-2;
  if (key==GLUT_KEY_DOWN) eyePoint[2]=eyePoint[2]+2;
  if (key==GLUT_KEY_PAGE_UP) eyePoint[1]=eyePoint[1]+delta2;
  if (key==GLUT_KEY_PAGE_DOWN) eyePoint[1]=eyePoint[1]-delta2;

}

void exitFunction(void)
{
  delete cube;
  delete axes;
  delete drawText2d; 
  //delete grid;
  delete triangle;
  delete quad;
  delete triPrism;
  delete ter;

  gluDeleteQuadric(quadric1);
  gluDeleteQuadric(quadric2);
  gluDeleteQuadric(quadric3);
  gluDeleteQuadric(quadric4);
//  gluDeleteQuadric(quadric2);
}


/****		Startup			****/

static void initGraphics (void)
{
  /* Black background */
  glClearColor(0, 0, 0, 0);

  /* colour green */
  color3[0]=0;
  color3[0]=1;
  color3[0]=0;

  glEnable(GL_DEPTH_TEST);

  /* Needed for vertex arrays */
  //glEnableClientState(GL_VERTEX_ARRAY);
  
  /* Popup menu attached to right mouse button */
  AppMenu = glutCreateMenu(menuChoice);
  glutSetMenu(AppMenu);
  glutAddMenuEntry("Red", cmdRed);
  glutAddMenuEntry("Green", cmdGreen);
  glutAddMenuEntry("----", 0);
  glutAddMenuEntry("Exit", cmdExit);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  
  /* Start values */
   setStartValues();

  menuChoice(cmdGreen);

  cube = new CubeClass();
  cube1 = new CubesClass(4);
  triangle = new TriClass();
  quad = new QuadClass();
  triPrism = new TriPrismClass(1,2,3);
  axes = new Axes();
  drawText2d = new Text2D();

  ter = new DEMTerrain();
  //ter->readFile("X21_100x100.txt");
  ter->readFile("X20_22.txt");
  ter->scaleValues(0.005f);
  ter->setMinMax();
  ter->displayCurrentValues();
  readTextureFile();

  //grid = new Grid();
  //grid->setUp(16,16,16,16);
  quadric1=gluNewQuadric();
  quadric2=gluNewQuadric();
  quadric3=gluNewQuadric();
  quadric4=gluNewQuadric();

  atexit (exitFunction);
}


void timer(int value)
{
timeForNextFrame=true;
glutTimerFunc(timerMSecs,timer,0);
}

/****		Main control		****/


static void idleRoutine(void)
{
  if (timeForNextFrame)
  {	
	glutPostRedisplay();
	timeForNextFrame=false;
	
	if (autoMove)
	{
		MovePlane2D(prox, proz, 0, speed, &prox, &proz);
	}
	spinAngle += spinIncrement;
	if (spinAngle > 360.0)
			spinAngle -= 360.0;
	
	if (angleVar < angle )
	{
		angleVar = angleVar+2;
		if (angleVar > angle ) angleVar = angle;
	}

	if (angleVar > angle )
	{
		angleVar = angleVar-2;
		if (angleVar < angle ) angleVar = angle;
	}


  }

}

int main (int argc, char * argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

  glutInitWindowSize(550, 550);
  glutInitWindowPosition(100, 75);
  glutCreateWindow("Cube");

  initGraphics();
  
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  
  glutKeyboardFunc(asciiKey);
  glutSpecialFunc(specialKey);
  
  glutIdleFunc(idleRoutine);
  glutTimerFunc(timerMSecs,timer,0);
  
  glutMainLoop();
  /* Should never get here, but keeps compiler happy */
  return 0;
}
