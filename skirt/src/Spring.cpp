/*
 * Spring.cpp
 *
 * 	Created on: Jan 23, 2016
 *  Based on the Spring class used during the winter term 2015/16
 *  in the physically based simulations course.
 */

#include "Spring.h"
#include <GL/freeglut.h> 

//#define _DEBUG_SPRING
//#define _DEBUG_SPRING_CTOR

#if defined _DEBUG_SPRING || defined _DEBUG_SPRING_CTOR
#include <iostream>
#warning "Debug messages for Spring class activated."
#define _DEBUG_MSG_C(X) std::cerr << "Spring class: " << X << std::endl;
#ifdef _DEBUG_SPRING
#define _DEBUG_MSG(X) _DEBUG_MSG_C(X)
#else
#define _DEBUG_MSG(X)
#endif
#else
#define _DEBUG_MSG_C(X)
#define _DEBUG_MSG(X)
#endif

/**
 * Default constructor.
 *
 * The end-points are left uninitialized, so after calling this constructor a call to @c init()
 * is necessary.
 * @param stiff stiffness factor of the spring
 * @param restLen the rest length = length of the spring without any forces taking effect to it
 */
Spring::Spring(double stiff, double restLen) :
		stiffness(stiff), restLength(restLen), m0(nullptr), m1(nullptr) {
	_DEBUG_MSG_C("constructor: stiff="<< stiffness << ", rlength=" << restLength)
}

/**
 * Constructor which initializes the endpoints of the spring with two mass-points
 * @param mass0 mass-point on the first end of the spring
 * @param mass1 mass-point on the second end
 * @param stiff stiffness factor of the spring
 * @param restLen the rest length = length of the spring without any forces taking effect to it
 */
Spring::Spring(Mass* mass0, Mass* mass1, double stiff, double restLen) :
		stiffness(stiff), restLength(restLen), m0(mass0), m1(mass1) {
	_DEBUG_MSG_C("constructor: stiff="<< stiffness << ", rlength=" << restLength << ", m0=" << m0 << ", m1=" << m1)
}

/**
 * Copy Constructor
 * @param s source spring
 */
Spring::Spring(Spring &s) :
		stiffness(s.stiffness), restLength(s.restLength), m0(s.getMass(0)), m1(s.getMass(1)) {
	_DEBUG_MSG_C("constuctor: copy")
}

/**
 * Move constructor
 * @param s source spring
 */
Spring::Spring(Spring &&s) :
		stiffness(s.stiffness), restLength(s.restLength), m0(s.getMass(0)), m1(s.getMass(1)) {
	s.m0 = nullptr;
	s.m1 = nullptr;
	_DEBUG_MSG_C("constuctor: move")
}

/**
 * Default destructor.
 */
Spring::~Spring() {
	_DEBUG_MSG_C("delete")
}

/**
 * Initialize spring with two given masses and set the rest length
 * to the distance of the two points.
 * @param mass0 mass at end-point
 * @param mass1 mass at end-point
 */
void Spring::init(Mass* mass0, Mass* mass1) {
	_DEBUG_MSG("init: m0=" << mass0 << ", m1=" << mass1)
	m0 = mass0;
	m1 = mass1;
	restLength = (m0->getPos() - m1->getPos()).norm();
}

/**
 * Get a mass.
 * @param i 0 or 1 to select the mass
 * @return the selected mass
 */
Mass* Spring::getMass(int i) {
	if (i != 0) {
		_DEBUG_MSG("getMass: m1=" << m1)
		return m1;
	} else {
		_DEBUG_MSG("getMass: m0=" << m0)
		return m0;
	}
}
