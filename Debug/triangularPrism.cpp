// Triclass 	
#include "triangularPrism.h"
#include "glut.h"
#include "vec3.h"

TriClass::TriClass() // constructor
{
	p0=vec3(0,0.5,0); // default triangle
	p1=vec3(-0.5,-0.5,0);
	p2=vec3(0.5,-0.5,0);
	computeNormal();
}
 
TriClass::TriClass(vec3 v0, vec3 v1, vec3 v2 ) // constructor
{
	p0=v0;
	p1=v1;
	p2=v2;
	computeNormal();
}

void TriClass::computeNormal()
{
	n0 = polygonNormal(p0,p1,p2);
}

void TriClass::renderWithNormals()
{
	glBegin(GL_TRIANGLES);
	glNormal3fv(n0.fv());
	glVertex3fv(p0.fv());
	glVertex3fv(p1.fv());
	glVertex3fv(p2.fv());
    glEnd();
}

void TriClass::renderTheNormals()
{
	temp1n.x = (p0.x+p1.x+p2.x)/3.0f; // for rendering normals
	temp1n.y = (p0.y+p1.y+p2.y)/3.0f; // for rendering normals
	temp1n.z = (p0.z+p1.z+p2.z)/3.0f; // for rendering normals
	temp2n = temp1n+n0;
	glBegin(GL_LINES);
	
	glVertex3fv(temp1n.fv());
	glVertex3fv(temp2n.fv());
	
    glEnd();
}

// ---------------------  TriClassSmooth ------------------------- //




TriClassSmooth::TriClassSmooth() // constructor
{
	p0 = vec3(0, 0.5, 0); // default triangle
	p1 = vec3(-0.5, -0.5, 0);
	p2 = vec3(0.5, -0.5, 0);
	c0 = vec3(1, 0, 0); // Red
	c1 = vec3(0, 1, 0); // Green
	c2 = vec3(0, 0, 1); // Blue
	computeNormal();
}

TriClassSmooth::TriClassSmooth(vec3 v0, vec3 v1, vec3 v2, vec3 color0, vec3 color1, vec3 color2) // constructor
{
	p0 = v0;
	p1 = v1;
	p2 = v2;
	c0 = color0;
	c1 = color1;
	c2 = color2;
	computeNormal();
}

void TriClassSmooth::renderWithSmooth()
{
	glShadeModel(GL_SMOOTH); // actually we may only need to do this once 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // actually we may only need to do this once as well
	glBegin(GL_TRIANGLES);
	glColor3fv(c0.fv());
	glVertex3fv(p0.fv());
    glColor3fv(c1.fv());
	glVertex3fv(p1.fv());
    glColor3fv(c2.fv());
	glVertex3fv(p2.fv());
	glEnd();
}

void TriClassSmooth::computeNormal()
{
	n0 = polygonNormal(p0, p1, p2);
}

// --------------------- Quad --------------------------- // 



	
QuadClass::QuadClass() // constructor
{
	p3=vec3(0.5,0.5,0); // default quad
	p2=vec3(0.5,-0.5,0);
	p1=vec3(-0.5,-0.5,0);
	p0=vec3(-0.5,0.5,0);
	computeNormal();
}
 
QuadClass::QuadClass(vec3 v0, vec3 v1, vec3 v2, vec3 v3 ) // constructor
{
	p0=v0;
	p1=v1;
	p2=v2;
	p3=v3;
	computeNormal();
}

void QuadClass::computeNormal()
{
	n0 = polygonNormal(p0,p1,p2);
}

void QuadClass::renderWithNormals()
{
	glBegin(GL_QUADS);
	glNormal3fv(n0.fv());
	glVertex3fv(p0.fv());
	glVertex3fv(p1.fv());
	glVertex3fv(p2.fv());
	glVertex3fv(p3.fv());
    glEnd();
}

void QuadClass::renderTheNormals()
{
	temp1n.x = (p0.x+p1.x+p2.x+p3.x)/4.0f; // for rendering normals
	temp1n.y = (p0.y+p1.y+p2.y+p3.y)/4.0f; // for rendering normals
	temp1n.z = (p0.z+p1.z+p2.z+p3.z)/4.0f; // for rendering normals
	temp2n = temp1n+n0;
	glBegin(GL_LINES);
	
	glVertex3fv(temp1n.fv());
	glVertex3fv(temp2n.fv());
	
    glEnd();
}


// ------------------------ triangular prism ------------------------------

	TriClass *tri1;
	TriClass *tri2;
	QuadClass *quad1;
	QuadClass *quad2;
	QuadClass *quad3;

	TriPrismClass::TriPrismClass() // constructor
	{
		makeIt(1,1,1);
	}

	TriPrismClass::~TriPrismClass() // destructor
	{
		delete tri1;
		delete tri2;
		delete quad1;
		delete quad2;
		delete quad3;
	}

	TriPrismClass::TriPrismClass(GLfloat height, GLfloat width, GLfloat length) // constructor
	{
		makeIt(height, width, length);
	}

	void TriPrismClass::makeIt(GLfloat height, GLfloat width, GLfloat length)
	{
		vec3 p1 =  vec3(0,height/2,length/2);
		vec3 p2 =  vec3(-width/2,-height/2,length/2);
		vec3 p3 =  vec3(width/2,-height/2,length/2);

		vec3 p4 =  vec3(0,height/2,-length/2);
		vec3 p5 =  vec3(-width/2,-height/2,-length/2);
		vec3 p6 =  vec3(width/2,-height/2,-length/2);

		tri1 = new TriClass(p1,p2,p3);
		tri2 = new TriClass(p6,p5,p4);
		
		quad1 = new QuadClass(p4,p5,p2,p1);
		quad2 = new QuadClass(p3,p6,p4,p1);


		quad3 = new QuadClass(p2,p5,p6,p3);
	}

	void TriPrismClass::computeNormal()
	{
		tri1->computeNormal();
		tri2->computeNormal();
		quad1->computeNormal();
		quad2->computeNormal();
		quad3->computeNormal();
	}

	void TriPrismClass::renderWithNormals()
	{
		tri1->renderWithNormals();
		tri2->renderWithNormals();
		quad1->renderWithNormals();
		quad2->renderWithNormals();
		quad3->renderWithNormals();
	}

	void TriPrismClass::renderTheNormals()
	{
		tri1->renderTheNormals();
		tri2->renderTheNormals();
		quad1->renderTheNormals();
		quad2->renderTheNormals();
		quad3->renderTheNormals();
	}


// end