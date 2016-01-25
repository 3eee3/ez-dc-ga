/*
 * Collision.h
 *
 *  Created on: Jan 24, 2016
 *      Author: littlelion
 */

#ifndef SRC_COLLISION_H_
#define SRC_COLLISION_H_

#include <GL/gl.h>
#include <cstddef>

#include "Mass.h"

namespace std {

void collisionDetectionAndResponse(vector<Mass> &points, size_t offsP,
		size_t lenP, GLfloat object_mesh[], size_t offsO, size_t lenO);

} // namespace std

#endif /* SRC_COLLISION_H_ */
