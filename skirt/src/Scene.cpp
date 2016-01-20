/*
 * Scene.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: littlelion
 */

#include <GL/freeglut.h>
#include <cstring>
#include <cstdlib>

//#define _DEBUG

//extern "C" {
#ifdef _DEBUG
#include "dice.h" //FIXME replace with skirt
#else
#include "skirt.h"
#endif
//}
#include "Scene.h"
#include "Mass.h"
#include "Spring.h"
#include "Simulation.h"

namespace std {

#ifdef _DEBUG
//FIXME correct size of 3D area - then remove this shift of X-axis
/*
 * shift x axis of cube to approx. center
 */
void iniPos() {
	for (size_t i = 0; i < diceVertices ; ++i) {
		dicePositions[3*i] -= 4.0f;
	}
}
#endif

Scene::Scene() : step(0.003) {}

Scene::Scene(int argc, char* argv[]) : step(0.003) {
	int arg = 1;
	while (arg < argc) {
		if (!strcmp(argv[arg], "-step")) {
			double par = atof(argv[++arg]);
			step = par > 0.0 ? par : 0.003;
			++arg;
		}
	}
}

Scene::~Scene() {}

void Scene::initialize() {

	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
#ifdef _DEBUG
	//FIXME correct size of 3D area - then remove this shift of X-axis
//	iniPos();
	glVertexPointer(3, GL_FLOAT, 0, dicePositions);
//	glColorPointer(3, GL_FLOAT, 0, dice_RollingDieColor);
	glTexCoordPointer(2, GL_FLOAT, 0, diceTexels);
	glNormalPointer(GL_FLOAT, 0, diceNormals);
#else
	glVertexPointer(3, GL_FLOAT, 0, skirtPositions);
//	glColorPointer(3, GL_FLOAT, 0, skirtColor);
	glTexCoordPointer(2, GL_FLOAT, 0, skirtTexels);
	glNormalPointer(GL_FLOAT, 0, skirtNormals);
#endif
	GLfloat mShininess[] = {128};
	GLfloat whiteSpecularMaterial[] = {1.0, 1.0, 1.0};
	GLfloat whiteDiffuseMaterial[] = {0.4f, 0.4f, 0.4f};
	GLfloat blankMaterial[] = {0.0, 0.0, 0.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteDiffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blankMaterial);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, blankMaterial);

	// set up points and springs //FIXME minimal example to test
	Eigen::Vector3d pt1(0.0, 1.0, 0.0);
	Eigen::Vector3d pt2;
	points.push_back(Mass(pt1, mass, damping));
	points.push_back(Mass(pt2, mass, damping));
	springs.push_back(Spring(stiffness));
	springs[0].init(&points[0], &points[1]);
}

void Scene::update() {
	timeStep(step, SYMPLECTIC, points, springs, false);
}

void Scene::render() {
	// TODO Auto-generated destructor stub
	glLoadIdentity();
	gluLookAt(-4.5, 4.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
#ifdef _DEBUG
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)diceVertices);
//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, dice_RollingDieVertexIdx);
#else
	glDrawArrays(GL_TRIANGLES, 0, skirtVertices);
#endif
	glFlush();
}

double Scene::getStep() const {
	return step;
}

} /* namespace std */
