/*
 * Spring.cpp
 *
 * 	Created on: Jan 23, 2016
 *  Based on the Spring class used during the winter term 2015/16
 *  in the physically based simulations course.
 */

#include "Spring.h"
#include <GL/freeglut.h> 

namespace std {

Spring::Spring(double stiff, double restLen) :
		stiffness(stiff), restLength(restLen), m0(new Mass), m1(new Mass) {
}
/*
Spring::Spring(Mass* mass0, Mass* mass1, double stiff, double restLen) :
		stiffness(stiff), restLength(restLen), m0(mass0), m1(mass1) {
}*/

Spring::Spring(Mass* _mass0, Mass* _mass1, double stiff){
{
	/* Initialize spring with pointers to both mass points */
	m0=_mass0;
	m1=_mass1;

	stiffness = stiff;
	/* Assume rest length is given by initial configuration */
	restLength = (_mass0->getPos() - _mass1->getPos()).norm();
}
}

Spring::~Spring() {
}

/**
 * Initialize spring with two given masses and set the rest length
 * to the distance of the two points.
 * @param mass0 mass at end point
 * @param mass1 mass at end point
 */
void Spring::init(Mass* mass0, Mass* mass1) {
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
		return m1;
	} else {
		return m0;
	}
}

}  // namespace std


