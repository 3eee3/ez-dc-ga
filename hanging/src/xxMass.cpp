/*
 * Mass.cpp
 *
 * 	Created on: Jan 23, 2016
 *  Based on the Point class used during the winter term 2015/16
 *  in the physically based simulations course.
 */

#include <GL/freeglut.h>
#include <vector>
#include "Mass.h"
#include "model_mapping.h"


#include <vector>
Mass::Mass(std::vector<GLfloat*> posPtr, double mass, double damp) :
		mass(mass), damping(damp), posPtr(posPtr)  {
	velocity = Eigen::Vector3d(0.0, 0.0, 0.0);
	force = Eigen::Vector3d(0.0, 0.0, 0.0);
	userForce = Eigen::Vector3d(0.0, 0.0, 0.0);
	fixed = false;
}

Mass::Mass(double mass, double damp) :
		mass(mass), damping(damp) {
	posPtr = vector<GLfloat*>();
	velocity = Eigen::Vector3d(0.0, 0.0, 0.0);
	force = Eigen::Vector3d(0.0, 0.0, 0.0);
	userForce = Eigen::Vector3d(0.0, 0.0, 0.0);
	fixed = false;
}

Mass::Mass(const Mass &rhs) {
	mass = rhs.mass;
	damping = rhs.damping;
	fixed = rhs.fixed;
	posPtr = rhs.posPtr;
	velocity = rhs.velocity;
	force = rhs.force;
	userForce = rhs.userForce;
}

Mass::~Mass() {
}

/**
 * Register the pointer to a vertex as position.
 * @param ptr pointer targeting the x-coordinate of the vertex position.
 *        The y- and z-coordinates must be in consecutive order in the array.
 */
void Mass::registerVertex(GLfloat* ptr) {
	posPtr.push_back(ptr);
}

/**
 * Set position of the mass object.
 *
 * This setter spreads the position to all registered vertices.
 * @param pos the position vector
 */
void Mass::setPos(Eigen::Vector3d pos) {
	for (GLfloat* p : posPtr) {
		p[0] = pos[0];
		p[1] = pos[1];
		p[2] = pos[2];
	}
}

/**
 * Set x-coordinate of the mass object.
 *
 * This setter spreads the x-coordinate to all registered vertices.
 * Don't use getPos.x to write a coordinate, it will mess up the
 * vertices array.
 * @param x the coordinate
 */
void Mass::setX(double x) {
	for (GLfloat* p : posPtr) {
		p[0] = x;
	}
}

double Mass::getX() {
	return posPtr[0][0];
}

double Mass::getY() {
	return posPtr[0][1];
}

double Mass::getZ() {
	return posPtr[0][2];
}

/**
 * Set y-coordinate of the mass object.
 *
 * This setter spreads the y-coordinate to all registered vertices.
 * Don't use getPos.y to write a coordinate, it will mess up the
 * vertices array.
 * @param x the coordinate
 */
void Mass::setY(double y) {
	for (GLfloat* p : posPtr) {
		p[0] = y;
	}
}

/**
 * Set z-coordinate of the mass object.
 *
 * This setter spreads the z-coordinate to all registered vertices.
 * Don't use getPos.z to write a coordinate, it will mess up the
 * vertices array.
 * @param x the coordinate
 */
void Mass::setZ(double z) {
	for (GLfloat* p : posPtr) {
		p[0] = z;
	}
}

/**
 * Get position vector of mass.
 *
 * Don't write to the elements of the returned vector. This will lead to
 * inconsistencies in the vertices array. To set a single value use
 * setX(), setY() and setZ() instead.
 * @return the position vector or if no vertices are registered the 0 vector.
 */
Eigen::Vector3d Mass::getPos() {
	if (posPtr.size() > 0) {
		return Eigen::Vector3d(posPtr[0][0], posPtr[0][1], posPtr[0][2]);
	} else {
		return Eigen::Vector3d(0.0, 0.0, 0.0);
	}
}

/**
 * Set the velocity.
 * @param v velocity vector
 */
void Mass::setVel(Eigen::Vector3d v) {
	velocity = v;
}

/**
 * Get the velocity.
 * @return velocity vector
 */
Eigen::Vector3d Mass::getVel() {
	return velocity;
}

/**
 * Set the force.
 *
 * This vector is reserved for the solver. Use @c setUserForce() to apply edxternal forces.
 * @param f force vector.
 */
void Mass::setForce(Eigen::Vector3d f) {
	force = f;
}

/**
 * Get the force.
 * @return force vector
 */
Eigen::Vector3d Mass::getForce() {
	return force;
}

/**
 * Add a force to the stored value.
 * @param f force vector
 */
void Mass::addForce(Eigen::Vector3d f) {
	force = force + f;
}

/**
 * Set external forces.
 *
 * Use this setter to apply all external forces except the gravity,
 * the solver applies it separately.
 * @param f force vector
 */
void Mass::setUserForce(Eigen::Vector3d f) {
	userForce = f;
}

/**
 * Get external forces (usually without gravity).
 * @return force vector
 */
Eigen::Vector3d Mass::getUserForce() {
	return userForce;
}
