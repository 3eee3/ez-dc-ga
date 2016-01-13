/* Standard includes */
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h> 

#include "Object.h"

/* The four vertices of a tetrahedron */
const GLfloat vertices[4][3] = {
{  1.0,  1.0,  1.0  },   /* index 0 */
{ -1.0, -1.0,  1.0  },   /* index 1 */
{ -1.0,  1.0, -1.0  },   /* index 2 */
{  1.0, -1.0, -1.0  } }; /* index 3 */

/* Color information for each vertex */
const GLfloat colors[4][3] = {
{  1.0,  0.0,  0.0  },   /* red */
{  0.0,  1.0,  0.0  },   /* green */
{  0.0,  0.0,  1.0  },   /* blue */
{  1.0,  1.0,  1.0  } }; /* white */

const GLubyte indices[6] = { 0, 1, 2, 3, 0, 1 };

GLfloat position[3]={ 0.0, 0.0, 0.0};
//GLfloat orientation[3]={ 0.0, 0.0, 0.0};
GLfloat angle=0.0;
GLfloat scale[3]={ 1.0,0.0,0.0 };

Object::Object()
{

}

void Object::draw(){
	//Duplicate Matrix on top of the stack (glMatrixMode(GL_MODELVIEW))
	glPushMatrix();
	//Multiplied with current matrix and current matrix is replaced by product
	
	glScalef(0.25,0.25,0.25);
	glTranslatef(2.0,2.0,0.0);
	glRotatef(angle,0.0,1.0,0.0);
	angle+=0.5;	
	if(angle>360) angle=0.0;
	
	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
	
	glDrawElements(GL_TRIANGLE_STRIP,6,GL_UNSIGNED_BYTE,indices);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glPopMatrix();
}

void Object::drawTest()
{
	glPushMatrix();

    glScalef(0.5,0.5,1.0);
    glBegin (GL_TRIANGLES);
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glVertex3f (0.25, 0.25, -11.0);
    glVertex3f (1.0, 0.25, 0.0);
    glVertex3f (0.25, 1.0, 0.0);
    glEnd ();

    glBegin (GL_TRIANGLES);
    glColor4f(1.0, 1.0, 1.0, 0.5);
    glVertex3f (0.0, 0.0, 0.0);
    glVertex3f (1.0, 0.0, 0.0);
    glVertex3f (0.0, 1.0, 0.0);
    glEnd ();

	glPopMatrix();
}


