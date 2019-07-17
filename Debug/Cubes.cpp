
#include "Cubes.h"
#include "bitmap.h"


// ------------------------- Some static data  -----------------------------


static GLfloat VertsZ[8][3] = {
  { -0.5,  0.5, -0.5 }, /* 0 left top rear */
  {  0.5,  0.5, -0.5 },	/* 1 right top rear */
  {  0.5, -0.5, -0.5 },	/* 2 right bottom rear */
  { -0.5, -0.5, -0.5 },	/* 3 left bottom rear */
  { -0.5,  0.5,  0.5 },	/* 4 left top front */
  {  0.5,  0.5,  0.5 },	/* 5 right top front */
  {  0.5, -0.5,  0.5 },	/* 6 right bottom front */
  { -0.5, -0.5,  0.5 }	/* 7 left bottom front */
};

static GLuint FacesZ[6][4] = {
  4, 5, 6, 7,	/* front */
  5, 1, 2, 6,	/* right */
  0, 4, 7, 3,	/* left */
  4, 0, 1, 5,	/* top */
  7, 6, 2, 3,	/* bottom */
  1, 0, 3, 2	/* rear */
};

static GLfloat NormalsZ[6][3] = {
  0, 0, 1, 	/* front */
  1, 0, 0, 	/* right */
  -1, 0, 0, 	/* left */
  0, 1, 0, 	/* top */
  0, -1, 0, /* bottom */
  0, 0, -1   /* rear */
};

static GLfloat spareVertexZ[3];

static CBitmap *bmp0; // the texture 
static CBitmap *bmp1; // the texture 
static GLuint texHandle[2]; // the open gl texture handle

void readTextureFile()
{
  bmp0= new CBitmap("metal.bmp");
  printf("bmp size %d %d\n",bmp0->GetWidth(),bmp0->GetHeight());
  //F:\\B\\VIC2015_MT2_Vs2015V1\\VIC2015_MT1_Vs2010\\starTexture3.bmp
  bmp1= new CBitmap("starTexture3.bmp");
  printf("bmp size %d %d\n",bmp1->GetWidth(),bmp1->GetHeight());

  glGenTextures(2, &texHandle[0]);

  // Texture handle will be bound to the BMP bitmap data just
  // as it was read in...
  glBindTexture(GL_TEXTURE_2D, texHandle[0]);
  glTexImage2D(GL_TEXTURE_2D,
               0,                              // Mipmap level 0
	       3,                              // 3 bytes [R,G,B] per pixel
		   bmp0->GetWidth(),   // width of bitmap
		   bmp0->GetHeight(),  // height of bitmap
  	       0,                              // no border pixels
	       GL_RGBA,                     // bitmap pixel format
	       GL_UNSIGNED_BYTE,               // format for GL to store it
		   bmp0->GetBits()               // pointer to the pixel data
  );  
  
  glBindTexture(GL_TEXTURE_2D, texHandle[1]);
  glTexImage2D(GL_TEXTURE_2D,
               0,                              // Mipmap level 0
	       3,                              // 3 bytes [R,G,B] per pixel
		   bmp1->GetWidth(),   // width of bitmap
		   bmp1->GetHeight(),  // height of bitmap
  	       0,                              // no border pixels
	       GL_RGBA,                     // bitmap pixel format
	       GL_UNSIGNED_BYTE,               // format for GL to store it
		   bmp1->GetBits()               // pointer to the pixel data
  );
  
}

// ------------------------- Cubes1Class -----------------------------

Cubes1Class::Cubes1Class() // constructor
{
	// --- Put your code here ---------------------------------------
	//

	// ------------- end --------------------------------------------
}

void Cubes1Class::render()
{
  /* Draw cube in traditional OpenGL style */
  	
	// --- Put your code here ---------------------------------------
	//
	for (int face = 0; face < 6; face++)
	{ 
		if (face == 1) { 
			glBindTexture(GL_TEXTURE_2D, texHandle[1]);
		} 

		else {
			glBindTexture(GL_TEXTURE_2D, texHandle[0]);
		}
		glBegin(GL_QUADS);
		for (int vert = 0; vert < 4; vert++) {
			glVertex3fv(VertsZ[FacesZ[face][vert]]);
			switch (vert) {
			case 0:
				glTexCoord2f(0.0, 0.0);
				break;
			case 1:
				glTexCoord2f(0.0, 1.0);
				break;
			case 2:
				glTexCoord2f(1.0, 1.0);
				break;
			case 3:
				glTexCoord2f(1.0, 0.0);
				break;

			}
		}
		glEnd();
	}



	// ------------- end --------------------------------------------
}

void Cubes1Class::renderWithTex()
{
	render();
}

void  Cubes1Class::renderTheNormals()
{
	// this normal renderinmg routine is not general purpose and works only with the cube
	glBegin(GL_LINES);
	glColor3f(1,1,0);
	for (int face = 0; face < 6; face++) 
		{
		spareVertexZ[0]=NormalsZ[face][0]/2.0f; //x
		spareVertexZ[1]=NormalsZ[face][1]/2.0f; //y
		spareVertexZ[2]=NormalsZ[face][2]/2.0f; //z
		glVertex3fv(spareVertexZ);
		glVertex3fv(NormalsZ[face]);
		}
	glEnd();
}

// ------------------------- CubesClass -----------------------------

    //Cubes1Class *lst[MAX_NUM_CUBES]; // the cubes
	//GLfloat angle[MAX_NUM_CUBES]; // the cubes rotations
	
	CubesClass::CubesClass(int num)   // constructor
	{
		numOfCubes = num;
		for (int i=0; i<numOfCubes; i++)
		{
			lst[i]=new Cubes1Class();
	
		}
		windUp(4);
	}

	void CubesClass::render()  // draw it
	{
		for (int i=0; i<numOfCubes; i++)
		{
			glPushMatrix();
	// --- Put your code here ---------------------------------------
	//
	// ------------- end --------------------------------------------
			lst[i]->render();
			glPopMatrix();
		}
	}

	CubesClass::~CubesClass()   // destructor
	{
		
		for (int i=0; i<numOfCubes; i++)
		{
			delete lst[i];
			lst[i]=NULL;
		}
	}

	void CubesClass::countDown(GLfloat unTwist)
	{
		for (int i=0; i<numOfCubes; i++)
		{
			angle[i]=angle[i]-unTwist;
			if (angle[i] < 0) angle[i]=0;
			glRotatef(angle[i], 0, 1, 0);
		}
	}

	void CubesClass::windUp(GLfloat twist)
	{
		for (int i=0; i<numOfCubes; i++)
		{
			angle[i]=360+i*twist;
		}
	}

// end