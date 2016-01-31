/*
 * Scene.cpp
 * Contains functions for initializing and drawing squared cloth and cube.
 */

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "Scene.h"
#include "Mass.h"
#include "Spring.h"
#include "Simulation.h"
#include "Collision.h"

namespace std {

Scene::Scene() : step(0.003), mass(0.15), stiffness(60.0), weak_stiff(5.0), damping(0.08) {}

Scene::Scene(int argc, char* argv[]) : step(0.003), mass(0.15), stiffness(60.0), weak_stiff(5.0), damping(0.08) {
	int arg = 1;
	while (arg < argc) {
		if(arg < argc +1) {
			if (!strcmp(argv[arg], "-step")) {
				double par = atof(argv[++arg]);
				step = par > 0.0 ? par : 0.003;
			}
			if (!strcmp(argv[arg], "-mass")) {
				double par = atof(argv[++arg]);
				mass = par > 0.0 ? par : 0.15;
			}
			if (!strcmp(argv[arg], "-stiff")) {
				double par = atof(argv[++arg]);
				stiffness = par > 0.0 ? par : 60.0;
			}
			if (!strcmp(argv[arg], "-wstiff")) {
				double par = atof(argv[++arg]);
				weak_stiff = par > 0.0 ? par : 5.0;
			}
			if (!strcmp(argv[arg], "-damp")) {
				double par = atof(argv[++arg]);
				damping = par > 0.0 ? par : 0.08;
			}
		}
		if (!strcmp(argv[arg], "-h")) {
			cout << "Arguments:\n";
			cout << "\t-step <val>     time step in seconds (default 0.003)\n";
			cout << "\t-mass <val>     mass for each mass point (0.15)\n";
			cout << "\t-stiff <val>    stiffness factor for stron springs (60.0)\n";
			cout << "\t-wstiff <val>   stiffness for weak springs (5.0)\n";
			cout << "\t-damp <val>     damping factor of masses (friction) (0.08)\n";
			cout << "\t-h              print this help message and exit\n";
			exit(EXIT_SUCCESS);
		}
		++arg;
	}
}

Scene::~Scene() {}

//////////////////////////////////////// OBJECT CUBE///////////////////////////////////////////////////

Eigen::Vector3d positionObject;
float scaleObject;
Eigen::Vector3d velocityObject;

static const int numVerticesObject = 36;
static const GLfloat vertex_buffer[] = {
     -1.0f,-1.0f,-1.0f, // triangle 1
     -1.0f,-1.0f, 1.0f,
     -1.0f, 1.0f, 1.0f, 
     1.0f, 1.0f,-1.0f, // triangle 2 : begin
     -1.0f,-1.0f,-1.0f,
     -1.0f, 1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
     -1.0f,-1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     -1.0f,-1.0f,-1.0f,
     -1.0f,-1.0f,-1.0f,
     -1.0f, 1.0f, 1.0f,
     -1.0f, 1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
     -1.0f,-1.0f, 1.0f,
     -1.0f,-1.0f,-1.0f,
     -1.0f, 1.0f, 1.0f,
     -1.0f,-1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,
     -1.0f, 1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
     -1.0f, 1.0f,-1.0f,
     -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     -1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f
 };
/*
void Scene::updateObject(double dt){
	//explicit euler integration
	//update position
	positionObject = positionObject + velocityObject * dt;
	//velocity remains constant
}*/

void Scene::drawObject(){
	glPushMatrix();
	glCullFace(GL_CW);
	glScalef(scaleObject,scaleObject,scaleObject);
	glTranslatef(positionObject.x(),positionObject.y(),positionObject.z());//object position
	glColor4f(0.4, 0, 0, 1.0);
	glLineWidth(2.0f);
	for(int i=0;i<numVerticesObject;i+=3){
		if(i==3){glColor4f(0.9, 0, 0, 1.0);}
		if(i==6){glColor4f(0.8, 0, 0.8, 1.0);}
		if(i==9){glColor4f(0.9, 0, 0.9, 1.0);}
		if(i==12){glColor4f(0.7, 0, 0.7, 1.0);}
		if(i==15){glColor4f(0.6, 0, 0.6, 1.0);}
		if(i==18){glColor4f(0.7, 0, 0, 1.0);}
		if(i==21){glColor4f(0.5, 0, 0.5, 1.0);}
		if(i==24){glColor4f(0.6, 0, 0, 1.0);}
		if(i==27){glColor4f(0.7, 0, 0.7, 1.0);}
		if(i==30){glColor4f(0.5, 0, 0, 1.0);}
		if(i==33){glColor4f(0.5, 0.5, 0, 1.0);}
		glBegin(GL_TRIANGLES);
			glVertex3f(vertex_buffer[i*3],vertex_buffer[i*3+1],vertex_buffer[i*3+2]);
			glVertex3f(vertex_buffer[i*3+3],vertex_buffer[i*3+4],vertex_buffer[i*3+5]);
			glVertex3f(vertex_buffer[i*3+6],vertex_buffer[i*3+7],vertex_buffer[i*3+8]);
		glEnd();
	}
	glPopMatrix();
}

void Scene::initObject(){
	positionObject = Eigen::Vector3d(1.5,-3.0,0.0);
    scaleObject = 1/3.5;
	velocityObject = Eigen::Vector3d(0.0,0.0,0.1);
}

////////////////////////////////////////END OBJECT///////////////////////////////////////////////////

static int res = 10;
bool inline withinBoundary(int x, int y, int res){
	if(x<0 || y<0 || x>res || y> res) 
		return false;
	else 
		return true;
}

void Scene::initSquaredCloth(){
	//Init cloth as square 0,0 -> 1,1
	double deltaGrid=1/(double)res;

	points.resize((res+1)*(res+1));
	//Init positions
	for(int y=0;y<res+1;y++){
		for(int x=0;x<res+1;x++){	
			points[x+y*(res+1)]=Mass(Eigen::Vector3d(x*deltaGrid,0.0,y*deltaGrid),0.1,0.1);
		}
	}

	//fix 2 positions
	points[0].setFixed(true);
	points[res].setFixed(true);
	//points[res*(res+1)].setFixed(true);
	//points[res+res*(res+1)].setFixed(true);
	
	
	//Init springs, masses references, stiffness, restLength is computed in constructor
	for(int y=0;y<res+1;y++){
		for(int x=0;x<res+1;x++){
			//structural springs
			if(withinBoundary(x-1,y,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x-1+y*(res+1)],400.0));
			}
			if(withinBoundary(x,y-1,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x+(y-1)*(res+1)],400.0));
			}
			if(withinBoundary(x+1,y,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x+1+y*(res+1)],400.0));
			}
			if(withinBoundary(x,y+1,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x+(y+1)*(res+1)],400.0));
			}

			//shear springs
			if(withinBoundary(x-1,y-1,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x-1+(y-1)*(res+1)],400.0));
			}
			if(withinBoundary(x+1,y-1,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x+1+(y-1)*(res+1)],400.0));
			}
			if(withinBoundary(x-1,y+1,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x-1+(y+1)*(res+1)],400.0));
			}
			if(withinBoundary(x+1,y+1,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x+1+(y+1)*(res+1)],400.0));
			}

			//bend springs
			if(withinBoundary(x-2,y,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x-2+y*(res+1)],100.0));
			}
			if(withinBoundary(x,y-2,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x+(y-2)*(res+1)],100.0));
			}
			if(withinBoundary(x+2,y,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x+2+y*(res+1)],100.0));
			}
			if(withinBoundary(x,y+2,res)){
				springs.push_back(Spring(&points[x+y*(res+1)],&points[x+(y+2)*(res+1)],100.0));
			}
		}
	}
}

void Scene::drawSquaredCloth(){
	glPushMatrix();
	//glTranslatef(-2.0,3.0,0.0);
    //glScalef(3.5,3.5,3.5);
	glLineWidth(2.0f);
	glColor4f(0.2, 0.2, 0.2, 1.0);
	for(int y=0;y<res;y++){
		glBegin(GL_LINE_STRIP);
		glVertex3f (points[0+y*(res+1)].getX(), points[0+y*(res+1)].getY(), points[0+y*(res+1)].getZ());
		for(int x=0;x<res;x++){
			glVertex3f (points[x+(y+1)*(res+1)].getX(), points[x+(y+1)*(res+1)].getY(), points[x+(y+1)*(res+1)].getZ());
    		glVertex3f (points[x+1+(y+1)*(res+1)].getX(), points[x+1+(y+1)*(res+1)].getY(), points[x+1+(y+1)*(res+1)].getZ());
			glVertex3f (points[x+y*(res+1)].getX(), points[x+y*(res+1)].getY(), points[x+y*(res+1)].getZ());
			glVertex3f (points[x+1+y*(res+1)].getX(), points[x+1+y*(res+1)].getY(), points[x+1+y*(res+1)].getZ());
		}
		glVertex3f (points[res+(y+1)*(res+1)].getX(), points[res+(y+1)*(res+1)].getY(), points[res+(y+1)*(res+1)].getZ());
		glEnd();
	}
	glPopMatrix();	
}

void Scene::initialize() {
	this->Scene::initSquaredCloth();
	this->Scene::initObject();
}

void Scene::update() {
	Simulation::step(step, Simulation::SYMPLECTIC, points, springs);
	collisionDetectionAndResponse(points, vertex_buffer, numVerticesObject, positionObject, scaleObject);
}

void Scene::render() {
	glLoadIdentity();
	gluLookAt(2.5, 1.0, -3, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	glTranslatef(0.0,0.5,0.0);
	//draw square Cloth
	this->Scene::drawSquaredCloth();

	//draw object
	this->Scene::drawObject();
	glFlush();
	glPopMatrix();
	
}

double Scene::getStep() const {
	return step;
}

} /* namespace std */
