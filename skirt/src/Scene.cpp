/*
 * Scene.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: littlelion
 */

#include <GL/freeglut.h>
#include "Scene.h"
#include "dice_RollingDie.h" //FIXME replace with skirt

namespace std {

//FIXME correct size of 3D area - then remove this shift of X-axis
/*
 * shift x axis of cube to approx. center
 */
void iniPos() {
	for (int i = 0; i < dice_RollingDieVertices ; ++i) {
		dice_RollingDiePositions[3*i] -= 4.0;
	}
}

Scene::Scene() {
	// TODO Auto-generated constructor stub

}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}

void Scene::initialize() {
	//FIXME correct size of 3D area - then remove this shift of X-axis
	iniPos();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, dice_RollingDiePositions);
	glColorPointer(3, GL_FLOAT, 0, dice_RollingDieColor);
}

void Scene::update() {
	// TODO Auto-generated destructor stub
}

void Scene::render() {
	// TODO Auto-generated destructor stub
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, dice_RollingDieVertexIdx);
	glFlush();
}

double Scene::getStep() const {
	return step;
}

} /* namespace std */
