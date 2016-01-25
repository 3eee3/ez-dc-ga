/*
 * Scene.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: littlelion
 */

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
#include "model_mapping.h"

// define simulation constants here

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
	glTranslatef(-2.0,3.0,0.0);
    glScalef(3.5,3.5,3.5);
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

	//draw model3DObject
	

	
	glPopMatrix();	
}

void Scene::initialize() {
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, model3dPositions);
//	glColorPointer(3, GL_FLOAT, 0, model3dColor);
	glTexCoordPointer(2, GL_FLOAT, 0, model3dTexels);
	glNormalPointer(GL_FLOAT, 0, model3dNormals);

	GLfloat mShininess[] = {128};
	GLfloat whiteSpecularMaterial[] = {1.0, 1.0, 1.0};
	GLfloat whiteDiffuseMaterial[] = {0.4f, 0.4f, 0.4f};
	GLfloat blankMaterial[] = {0.0, 0.0, 0.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteDiffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blankMaterial);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, blankMaterial);
*/
	this->Scene::initSquaredCloth();
}

void Scene::update() {
	Simulation::step(step, Simulation::SYMPLECTIC, points, springs);
	//collisionDetectionAndResponse(points, model3dPositions, 0, model3dVertices*3);
}

void Scene::render() {
	glLoadIdentity();
	gluLookAt(-4.5, 4.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glDrawArrays(GL_TRIANGLES, 0, (GLsizei)model3dVertices);
	//draw square Cloth
	this->Scene::drawSquaredCloth();
	glFlush();
}

double Scene::getStep() const {
	return step;
}

} /* namespace std */
