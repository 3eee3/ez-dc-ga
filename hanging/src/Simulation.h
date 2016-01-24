/*
 * Simulation.h
 *
 *  Created on: Jan 19, 2016
 *      Author: littlelion
 */

#ifndef SRC_SIMULATION_H_
#define SRC_SIMULATION_H_

namespace std {

/* mass spring - numerical solver */
class Simulation {
public:
//XXX remove this
	enum Method {
		EULER, SYMPLECTIC, LEAPFROG, MIDPOINT, ANALYTICAL
	};
// XXX end

	static void step(double dt, Method method, vector<Mass> &points,
			vector<Spring> &springs);
};

} // namespace std

#endif /* SRC_SIMULATION_H_ */
