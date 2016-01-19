/*
 * Simulation.h
 *
 *  Created on: Jan 19, 2016
 *      Author: littlelion
 */

#ifndef SRC_SIMULATION_H_
#define SRC_SIMULATION_H_

namespace std {
	//XXX remove this
	/* Numerical solver */
	//    enum Method{EULER, SYMPLECTIC, LEAPFROG, MIDPOINT};
	enum Method{EULER, SYMPLECTIC, LEAPFROG, MIDPOINT, ANALYTICAL}; // modified version
	/* Test scene */

	// XXX end
	void timeStep(double dt, Method method,
	              vector<Mass> &points, vector<Spring> &springs, bool interaction);

}



#endif /* SRC_SIMULATION_H_ */
