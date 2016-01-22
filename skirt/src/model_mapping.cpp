/*
 * model_mapping.c
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
 * Edit the pointers in this file to fit the code to an exchanged header file.
 * DON'T EDIT them in the generated .h file, they may be overwritten by the
 * generator tool.
 */

/* activate the small model with two dice by uncommenting this line */
#define _DEBUG_OGL_MODEL

#include <GL/gl.h>
#include "model_mapping.h"

#ifdef _DEBUG_OGL_MODEL
#include "dice.h"
#else
/* edit this #include instruction to import the generated header file */
#include "skirt_sphere.h"
#endif

namespace std {

#ifndef _DEBUG_OGL_MODEL
/*
 * Vertex Arrays:
 * The arrays contain the vertex data for all objects of a 3D model
 *
 * Positions and normals are composed of 3 values per coordinate
 * Texels contain 2 values per coordinate
 *
 * All faces consist of triangles, which are ordered in sequence in the arrays.
 * If the original model contains polygons other than triangles, then they will
 * be converted by the 'blender2oGL' tool.
 *
 * A triangular face with vertices A, B, and C is composed this values:
 * indices A_x, A_y, A_z, B_x, B_y, B_z, C_x, C_y, C_z in modelPositions and modelNormals
 * indices A_u, A_v, B_u, B_v, C_u, C_v in modelTexels
 *
 * size of model3dPositions and mode3dNormals is 3*model3dVertices
 * size of model3dTexels is 2*model3dVertices
 */
const size_t model3dVertices = skirt_sphereVertices;
GLfloat* model3dPositions = skirt_spherePositions;
GLfloat* model3dTexels = skirt_sphereTexels;
GLfloat* model3dNormals = skirt_sphereNormals;

//FIXME not finished yet
/*
 * Index Mapping Arrays:
 */
const size_t* model3dMasses = nullptr;
const size_t* model3dSprings = nullptr;
/* indexing arrays: 3 ascending values per coordinate at index, index+1 and index+2 */
const size_t* model3dFwdIndexI = nullptr; //FIXME needed?
const size_t** model3dFwdIndex = nullptr;
const size_t* model3dFwdIndexLength = nullptr;
const size_t* model3dRevIndex = nullptr;

/*
 * Object Topology Informations:
 * model3dObjectOffset contains the offset in terms of vertices to the first entry in the
 * vertex array of each individual 3D-object.
 *
 * model3dObjectLength contains the numbers of vertices the objects belong to.
 * Multiply the offsets and lengths with the dimension of the model space to calculate
 * the index (*3 for model3dPositions and model3dNormals, *2 for model3dTexels).
 *
 * model3dObjectNames contains identification strings for each 3d-object.
 * model3dTextureFilePath identifies the image file for texturing.
 */
//XXX begin
const size_t skirt_sphereObjectOffset[] = {0, 1944, 2184};
const size_t skirt_sphereObjectLength[] = {1944, 240, 18432};
const char skirt_sphereObjectNames[3][10] = {"Grid", "Icosphere", "Cone"};
//XXX end
const size_t* model3dObjectOffset = skirt_sphereObjectOffset;//FIXME hardcoded data
const size_t* model3dObjectLength = skirt_sphereObjectLength;
const char** model3dObjectNames = (const char**) skirt_sphereObjectNames;
const char* model3dTextureFilePath = "textures/textures_all.rgb";

#else /* defined _DEBUG_OGL_MODEL */
/* provide a small model for debugging issues */
const size_t model3dVertices = diceVertices;
GLfloat* model3dPositions = dicePositions;
GLfloat* model3dTexels = diceTexels;
GLfloat* model3dNormals = diceNormals;
const size_t* model3dMasses;
const size_t* model3dSprings;
const size_t* model3dFwdIndexI; //FIXME needed?
const size_t** model3dFwdIndex;
const size_t* model3dFwdIndexLength;
const size_t* model3dRevIndex;
//XXX begin
const size_t diceObjectOffset[] = { 0, 36 };
const size_t diceObjectLength[] = { 36, 36 };
const char diceObjectNames[2][11] = { "RollingDie", "Die" };
//XXX end
const size_t* model3dObjectOffset = diceObjectOffset; //FIXME hardcoded data
const size_t* model3dObjectLength = diceObjectLength;
const char** model3dObjectNames = (const char**) diceObjectNames;
const char* model3dTextureFilePath = "textures/wood.rgb";
#endif /* _DEBUG_OGL_MODEL */

} // namespace std