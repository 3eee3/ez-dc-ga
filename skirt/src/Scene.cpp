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
#include "model_mapping.h"
//#include "Cloth3D.h"

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

//vector<Mass> points;
//vector<Spring> springs;
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
	glTranslatef(0.0,2.0,0.0);
    glScalef(2.0,2.0,2.0);
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
//	GLfloat model3dColor[model3dVertices*3];
//	for(int i = 0; i < model3dVertices*3;++i) model3dColor[i]=1/(float)(rand()%254+1);
	int model3dIndices[model3dVertices*3];
	for(int i = 0; i < model3dVertices;++i) model3dIndices[i]=i;

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

	this->Scene::initSquaredCloth();
	//cloth = new Cloth3D(sim_res);

	/*
	for (size_t i = 0; i < model3dObjectsWithMass; ++i) { // for all objects with mass-spring definition

		// initialize mass points with data given by the model header
		for (size_t j = 0; j < model3dMasses[i]; ++j) {
			points.push_back(Mass(mass, damping));
			for (size_t k = 0; k < model3dFwdIndexLength[j]; ++k) {
				points.back().registerVertex(&model3dPositions[model3dFwdIndex[j+model3dMassFwdOffs[i]][k]*3]);
			}
		}
		
		// initialize strong springs
		vector<bool> done = vector<bool>(model3dMassVertices[i], false);
		for (size_t j = 0; j < model3dMassVertices[i]/3; ++j) {
			for (size_t k = 0; k < 3; ++k) {
				if (!done[j+k]) {
					springs.push_back(Spring(stiffness));
					springs.back().init(
							&points[model3dRevIndex[j+model3dMassRevOffs[i]+k]+model3dMassFwdOffs[i]],
							&points[model3dRevIndex[j+model3dMassRevOffs[i]+(k+1)%3]+model3dMassFwdOffs[i]]);
				}
			}
		}

		// initialize weak springs
		//FIXME weak springs are missing

	}*/
}

void Scene::update() {
	//Collision::collisionDetectionAndResponse(vector<Mass> &points, GLfloat object_mesh[], size_t offs, size_t len)
	Simulation::step(step, Simulation::SYMPLECTIC, points, springs);
}

void Scene::render() {
	// TODO Auto-generated destructor stub
	glLoadIdentity();
	gluLookAt(-4.5, 4.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//updateNormals();
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)model3dVertices);

	//draw square Cloth
	this->Scene::drawSquaredCloth();

//	int model3dIndices[model3dVertices*3];
//	for(int i = 0; i < model3dVertices;++i) model3dIndices[i]=i;
//	glDrawElements(GL_TRIANGLES, (GLsizei)model3dVertices, GL_UNSIGNED_BYTE, model3dIndices);
	glFlush();
}

double Scene::getStep() const {
	return step;
}

/**
 * Recalculate all normals for all faces modified by the simulaton step.
 */
void Scene::updateNormals() {
	for (size_t i = 0; i < model3dVertices/3; ++i) {
		GLfloat* a_x = &model3dPositions[0];
		GLfloat* n_x = &model3dNormals[0];


//	for (size_t i = 0; i < model3dObjectsWithMass; ++i) { // for all objects with mass-spring definition
//		for (size_t j = 0; j < model3dMassVertices[i]/3; ++j) { // for each triangle of a 3D-object
//			GLfloat* a_x = &model3dPositions[(j+model3dMassRevOffs[i])*3];
//			GLfloat* n_x = &model3dNormals[(j+model3dMassRevOffs[i])*3];

			Eigen::Vector3f a, b, c, n;
			a = Eigen::Vector3f(*a_x, *(a_x+1), *(a_x+2));
			b = Eigen::Vector3f(*(a_x+3), *(a_x+4), *(a_x+5));
			c = Eigen::Vector3f(*(a_x+6), *(a_x+7), *(a_x+8));
//			a = points[model3dRevIndex[j+model3dMassRevOffs[i]]+model3dMassFwdOffs[i]].getPos().cast<float>();
//			b = points[model3dRevIndex[j+model3dMassRevOffs[i]+1]+model3dMassFwdOffs[i]].getPos().cast<float>();
//			c = points[model3dRevIndex[j+model3dMassRevOffs[i]+2]+model3dMassFwdOffs[i]].getPos().cast<float>();
			n = (a-b).cross(c-a).normalized();


//			model3dNormals[(j+model3dMassRevOffs[i])*3] = n[0];
//			model3dNormals[(j+model3dMassRevOffs[i])*3+1] = n[1];
//			model3dNormals[(j+model3dMassRevOffs[i])*3+2] = n[2];
			*n_x = n[0];
			*(n_x+1) = n[1];
			*(n_x+2) = n[2];
			*(n_x+3) = n[0];
			*(n_x+4) = n[1];
			*(n_x+5) = n[2];
			*(n_x+6) = n[0];
			*(n_x+7) = n[1];
			*(n_x+8) = n[2];

//		}
	}
}

} /* namespace std */
