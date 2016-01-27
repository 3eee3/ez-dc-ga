/*
 * model_mapping.h
 *
 *  Created on: Jan 21, 2016
 *      Author: littlelion
 */

/**
 * @file
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
extern const size_t model3dVertices;         ///< number of vertices
extern GLfloat* model3dPositions;            ///< all vertex positions
extern GLfloat* model3dTexels;               ///< all texture coordinates
extern GLfloat* model3dNormals;              ///< all normals of the face surfaces

// Index Mapping Arrays
extern const size_t model3dObjectsWithMass;  ///< number of objects to apply to the mass-spring simulation
extern const size_t* model3dMasses;          ///< array of 3D-objects with mass
extern const size_t* model3dMassFwdOffs;     ///< offset in the points array
extern const size_t* model3dMassVertices;    ///< number of vertices per object with mass
extern const size_t* model3dMassRevOffs;     ///< offsets for the model3dRevIndex array
extern const size_t* model3dMassRevOffsOrig; ///< offset to the first vertex of an object with mass

extern const size_t** model3dFwdIndex;       ///< indices of the mass-points in the positions array
extern const size_t* model3dFwdIndexLength;  ///< number of indices for each mass-point
extern const size_t* model3dRevIndex;        ///< index of a vertex in the mass-points array

// Object Topology Informations
extern const size_t model3dObjects;          ///< number of 3D-objects
extern const size_t* model3dObjectOffset;    ///< offset to the first vertex of a 3D-object
extern const size_t* model3dObjectLength;    ///< number of vertices for each 3D-object
extern const char** model3dObjectNames;      ///< names of the objects (for identification)
extern const char* model3dTextureFilePath;   ///< path to the texture-image file

} // namespace std

#endif /* SRC_MODEL_MAPPING_H_ */
