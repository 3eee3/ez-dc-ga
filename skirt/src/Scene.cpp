/*
 * Scene.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: littlelion
 */

#include <GL/freeglut.h>
#include "Scene.h"

#define _DEBUG

extern "C" {
#ifdef _DEBUG
#include "dice_RollingDie.h" //FIXME replace with skirt
#else
#include "skirt.h"
#endif
}

namespace std {

#ifdef _DEBUG
//FIXME correct size of 3D area - then remove this shift of X-axis
/*
 * shift x axis of cube to approx. center
 */
void iniPos() {
	for (int i = 0; i < dice_RollingDieVertices ; ++i) {
		dice_RollingDiePositions[3*i] -= 4.0;
	}
}
#endif

Scene::Scene() {
	// TODO Auto-generated constructor stub

}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}

void Scene::initialize() {

	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
#ifdef _DEBUG
	//FIXME correct size of 3D area - then remove this shift of X-axis
	iniPos();
	glVertexPointer(3, GL_FLOAT, 0, dice_RollingDiePositions);
//	glColorPointer(3, GL_FLOAT, 0, dice_RollingDieColor);
	glTexCoordPointer(2, GL_FLOAT, 0, dice_RollingDieTexels);
	glNormalPointer(GL_FLOAT, 0, dice_RollingDieNormals);
#else
	glVertexPointer(3, GL_FLOAT, 0, skirtPositions);
//	glColorPointer(3, GL_FLOAT, 0, skirtColor);
	glTexCoordPointer(2, GL_FLOAT, 0, skirtTexels);
	glNormalPointer(GL_FLOAT, 0, skirtNormals);
#endif
	GLfloat mShininess[] = {128};
	GLfloat whiteSpecularMaterial[] = {1.0, 1.0, 1.0};
	GLfloat whiteDiffuseMaterial[] = {0.4, 0.4, 0.4};
	GLfloat blankMaterial[] = {0.0, 0.0, 0.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteDiffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blankMaterial);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, blankMaterial);
}

void Scene::update() {
	// TODO Auto-generated destructor stub
}

void Scene::render() {
	// TODO Auto-generated destructor stub
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
#ifdef _DEBUG
	glDrawArrays(GL_TRIANGLES, 0, dice_RollingDieVertices);
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
