/*
 * Scene.h
 *
 *  Created on: Jan 16, 2016
 *      Author: littlelion
 */

#ifndef SRC_SCENE_H_
#define SRC_SCENE_H_

namespace std {

class Scene {
public:
	Scene();
	virtual ~Scene();
	void initialize();
	void update();
	void render();
	double getStep() const;

private:
	double step = 0.003;
};

} /* namespace std */

#endif /* SRC_SCENE_H_ */
