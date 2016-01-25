/*
 * Scene.h
 *
 *  Created on: Jan 16, 2016
 *      Author: littlelion
 */

#ifndef SRC_SCENE_H_
#define SRC_SCENE_H_

#include <vector>

#include "Mass.h"
#include "Spring.h"

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
	void updateNormals();

private:
	double step;
    double mass;
    double stiffness;
    double weak_stiff;
    double damping;
    vector<Mass> points;
    vector<Spring> springs;
};

} /* namespace std */

#endif /* SRC_SCENE_H_ */
