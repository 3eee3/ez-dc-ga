/*
 * model_mapping.h
 *
 *  Created on: Jan 21, 2016
 *      Author: littlelion
 */

/*
 * This file includes the vertex, texture and normals coordinates
 * and some 3D object topology informations.
 *
 * It is a wrapper to make the code relying on these variables independent
 * from varying names supplied by the header file generator tool
 * 'blender2oGL'
 *
 * Edit the pointers in the 'model_mapping.cpp' file to fit the code to an exchanged header file.
 * DON'T EDIT them in the generated .h file, they may be overwritten by the
 * generator tool.
 *
 * See 'model_mapping.cpp' for further details.
 */
#ifndef SRC_MODEL_MAPPING_H_
#define SRC_MODEL_MAPPING_H_

#include <GL/gl.h>
#include <cstddef>

namespace std {

// Vertex Arrays
extern const size_t model3dVertices;
extern GLfloat* model3dPositions;
extern GLfloat* model3dTexels;
extern GLfloat* model3dNormals;

//FIXME not finished yet
// Index Mapping Arrays
extern const size_t model3dMassesLength;
extern const size_t model3dSpringsLength;
extern const size_t* model3dMasses;
extern const size_t* model3dSprings;
extern const size_t* model3dFwdIndexI; //FIXME needed?
extern const size_t** model3dFwdIndex;
extern const size_t* model3dFwdIndexLength;
extern const size_t* model3dRevIndex;

// Object Topology Informations
extern const size_t model3dObjects;
extern const size_t* model3dObjectOffset;
extern const size_t* model3dObjectLength;
extern const char** model3dObjectNames;
extern const char* model3dTextureFilePath;

} // namespace std

#endif /* SRC_MODEL_MAPPING_H_ */
