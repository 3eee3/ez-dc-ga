/*
 * Scene.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: littlelion
 */

#include <GL/freeglut.h>
#include <cstring>
#include <cstdlib>

#include "Scene.h"
#include "Mass.h"
#include "Spring.h"
#include "Simulation.h"
#include "model_mapping.h"

namespace std {

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

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)model3dVertices);
//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, dice_RollingDieVertexIdx);
	glFlush();
}

double Scene::getStep() const {
	return step;
}

} /* namespace std */
