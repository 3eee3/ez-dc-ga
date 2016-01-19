/*
 * Scene.h
 *
 *  Created on: Jan 16, 2016
 *      Author: littlelion
 */

#ifndef SRC_SCENE_H_
#define SRC_SCENE_H_

#include <vector>

#include "Spring.h"
#include "Point.h"

namespace std {

class Scene {
public:
	Scene();
	Scene(int argc, char* argv[]);
	virtual ~Scene();
	void initialize();
	void update();
	void render();
	double getStep() const;

private:
	double step;
    double mass = 0.15;      /* Identical mass for all points */
    double stiffness = 60.0; /* Identical spring stiffness for all springs */
    double damping = 0.08;   /* Identical damping for all points */
    vector<Point> points;
    vector<Spring> springs;
};

} /* namespace std */

#endif /* SRC_SCENE_H_ */
