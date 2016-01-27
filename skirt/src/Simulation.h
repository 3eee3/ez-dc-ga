/*
 * Simulation.h
 *
 *  Created on: Jan 19, 2016
 *      Author: littlelion
 */

#ifndef SRC_SIMULATION_H_
#define SRC_SIMULATION_H_

namespace std {

/**
 * Simulation class. Solver for mass-spring systems.
 *
 * This class provides some numerical solvers. They are programmed during a assignment
 * of the pro-seminar physically based simulations preceding to this project.
 */
class Simulation {
public:
//XXX remove this
	enum Method {
		EULER, SYMPLECTIC, LEAPFROG, MIDPOINT
	};
// XXX end

	static void step(double dt, Method method, vector<Mass> &points,
			vector<Spring> &springs);
};

} // namespace std

#endif /* SRC_SIMULATION_H_ */
