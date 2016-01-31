/*
 * Collision.h
 *
 *  Created on: Jan 24, 2016
 *      Author: littlelion
 */
#include <GL/gl.h>
#include <cstddef>
#include "Mass.h"

namespace std{

void collisionDetectionAndResponse(std::vector<Mass> &points, const GLfloat object_mesh[], int len, Eigen::Vector3d positionObject, float scale);

}//end namespace std

