/*
 * Spring.h
 *
 * 	Created on: Jan 23, 2016
 *  Based on the Spring class used during the winter term 2015/16
 *  in the physically based simulations course.
 */

#ifndef __SPRING_H__
#define __SPRING_H__

#include "Mass.h"

namespace std {

class Spring {
public:
	Spring(double stiff = 0.0, double restLen = 0.0);
	//Spring(Mass* mass0, Mass* mass1, double stiff = 0.0, double restLen = 0.0);
	Spring(Mass* mass0, Mass* mass1, double stiff);
	virtual ~Spring(void);

	void init(Mass* mass0, Mass *mass1);

	Mass* getMass(int i); /* Return end point 0 or 1 */

	double stiffness;
	double restLength; /* Rest length of spring */

private:
	Mass *m0, *m1; /* Pointers to two masses */
};

}  // namespace std

#endif // __SPRING_H__
