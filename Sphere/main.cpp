#include<GL/glut.h>
#include<GL/gl.h>
#include<stdio.h>
#include<iostream>

#include "skirt_sphere.h"
#include "Triangle.h"
#include "Triangle2.h"
#include "Node2.h"
#include "Vector3.h"

void init (void);

GLfloat rotation = 0.0;

//from skirt
void getTrianglesAndNeighbours(Triangle (&triangles)[10000], Triangle2 (&neighbour)[10000]){
    int n=0;
    int m=0;
    Triangle t;
    Vector3 node[3];
    for(int i=2189;i<18428/9;i++){
        for(int j=0;j<3;j++){
            node[j] = Vector3(skirt_spherePositions[i*9+j*3],skirt_spherePositions[i*9+j*3+1],skirt_spherePositions[i*9+j*3+2]);
        }
        t = Triangle(node[0],node[1],node[2]);

        if(t.isTriangle())triangles[n] = t;
            for(int j=0;j<n;j++){
                if(triangles[j].isNeighbour(t)){
                    neighbour[m] = Triangle2(triangles[j],t);
                    m++;
                }
            }
        n++;
    }
}
void getCrossNodesOfNeighbourAndDistance(Triangle t1, Triangle t2,Node2 &nodes, float &distance){
    Triangle t;
    t.getWeakNeighbourVector(t1, t2, nodes, distance);
}

void drawSphere(void){
    glTranslatef(0.0f,0.0f,0.0f);
    glRotatef(rotation, 0.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, skirt_spherePositions);
    glDrawArrays(GL_TRIANGLES, 0, 2189);    // sphere
    glDisableClientState(GL_VERTEX_ARRAY);
    glFlush();
}

void drawSkirt(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, skirt_spherePositions);
    glDrawArrays(GL_TRIANGLES, 2189, 18428);  //skirt
    glDisableClientState(GL_VERTEX_ARRAY);
    glFlush();
}

void rotateObject(int millisec){
    rotation+=5;
    glutTimerFunc(millisec, rotateObject, millisec); // for next timer event
    glutPostRedisplay();
}

void init (void){
	glClearColor(0.0,0.0,0.0,0.0); // Background color
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
	glutInitWindowSize(600, 600);
    glutCreateWindow("Cloth Simulation");
    init();

 //   glutDisplayFunc(drawSkirt);
    glutDisplayFunc(drawSphere);
    glutTimerFunc(300, rotateObject, 300);

    glutMainLoop();
    return 0;
}

