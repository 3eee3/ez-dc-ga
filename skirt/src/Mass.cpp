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

//#define _DEBUG_MASS
//#define _DEBUG_MASS_CTOR

#if defined _DEBUG_MASS || defined _DEBUG_MASS_CTOR
#include <iostream>
#warning "Debug messages for Mass class activated."
#define _DEBUG_MSG_C(X) std::cerr << "Mass class: " << X << std::endl;
#ifdef _DEBUG_MASS
#define _DEBUG_MSG(X) _DEBUG_MSG_C(X)
#else
#define _DEBUG_MSG(X)
#endif
#else
#define _DEBUG_MSG_C(X)
#define _DEBUG_MSG(X)
#endif

#include <vector>

/**
 * Default constructor.
 * @param posPtr vector of pointers to the vertices in the positions array
 * @param mass the mass of the mass-point
 * @param damp the damping factor of the mass-point (to simulate friction and air resistance)
 */
Mass::Mass(std::vector<GLfloat*> posPtr, double mass, double damp) :
		mass(mass), damping(damp), posPtr(posPtr)  {
	_DEBUG_MSG_C("constructor: posPtr.size=" << posPtr.size())
	velocity = Eigen::Vector3d(0.0, 0.0, 0.0);
	force = Eigen::Vector3d(0.0, 0.0, 0.0);
	userForce = Eigen::Vector3d(0.0, 0.0, 0.0);
	fixed = false;
}

/**
 * Constructor which takes at least the mass argument.
 * @param mass the mass of the mass-point
 * @param damp the damping factor of the mass-point (to simulate friction and air resistance)
 */
Mass::Mass(double mass, double damp) :
		mass(mass), damping(damp) {
	_DEBUG_MSG_C("constructor: new posPtr")
	posPtr = vector<GLfloat*>();
	velocity = Eigen::Vector3d(0.0, 0.0, 0.0);
	force = Eigen::Vector3d(0.0, 0.0, 0.0);
	userForce = Eigen::Vector3d(0.0, 0.0, 0.0);
	fixed = false;
}

/**
 * Copy constructor
 * @param m source object
 */
Mass::Mass(const Mass &m) {
	_DEBUG_MSG_C("constructor: copy mass")
	mass = m.mass;
	damping = m.damping;
	fixed = m.fixed;
	posPtr = vector<GLfloat*>(m.posPtr);
	velocity = m.velocity;
	force = m.force;
	userForce = m.userForce;
}

/**
 * Move Constructor
 * @param m
 */
Mass::Mass(Mass &&m) {
	_DEBUG_MSG_C("constructor: move mass")
	mass = m.mass;
	damping = m.damping;
	fixed = m.fixed;
	posPtr = std::move(m.posPtr);
	velocity = std::move(m.velocity);
	force = std::move(m.force);
	userForce = std::move(m.userForce);
	m.posPtr = vector<GLfloat*>();
	m.velocity = Eigen::Vector3d();
	m.force = Eigen::Vector3d();
	m.userForce = Eigen::Vector3d();
}

/**
 * Default destructor.
 */
Mass::~Mass() {
	_DEBUG_MSG_C("delete")
}

/**
 * Register the pointer to a vertex as position.
 * @param ptr pointer targeting the x-coordinate of the vertex position.
 *        The y- and z-coordinates must be in consecutive order in the array.
 */
void Mass::registerVertex(GLfloat* ptr) {
	_DEBUG_MSG("registerVertex : posPtr.size=" << posPtr.size())
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
		p[0] = (GLfloat)pos[0];
		p[1] = (GLfloat)pos[1];
		p[2] = (GLfloat)pos[2];
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
		p[0] = (GLfloat)x;
	}
}

/**
 * Set y-coordinate of the mass object.
 *
 * This setter spreads the y-coordinate to all registered vertices.
 * Don't use getPos.y to write a coordinate, it will mess up the
 * vertices array.
 * @param y the coordinate
 */
void Mass::setY(double y) {
	for (GLfloat* p : posPtr) {
		p[0] = (GLfloat)y;
	}
}

/**
 * Set z-coordinate of the mass object.
 *
 * This setter spreads the z-coordinate to all registered vertices.
 * Don't use getPos.z to write a coordinate, it will mess up the
 * vertices array.
 * @param z the coordinate
 */
void Mass::setZ(double z) {
	for (GLfloat* p : posPtr) {
		p[0] = (GLfloat)z;
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
	if (!posPtr.empty() && posPtr.size() < posPtr.max_size()) {
		_DEBUG_MSG("getPos: posPtr.size=" << posPtr.size() << ", x = " << *posPtr[0] << ", y = " << *(posPtr[0]+1) << ", z = " << *(posPtr[0]+2))
		GLfloat x = *posPtr[0];
		GLfloat y = *(posPtr[0]+1);
		GLfloat z = *(posPtr[0]+2);

		return Eigen::Vector3d((double)x, (double)y, (double)z);
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
