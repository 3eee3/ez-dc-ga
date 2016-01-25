// This is a C-header file (.h) for the models "dice_RollingDie", "dice_Die"
// Don't edit! This is an auto-generated file by blender2oGL. Modifications are not permanent.

/*
 * model statistics:
 * name: "dice_RollingDie"
 * # positions =     8 - start index =     0
 * # texels    =    14 - start index =     0
 * # normals   =     6 - start index =     0
 * # faces     =    12
 * contains converted polygons of triangle fan shape
 * includes masses and springs structures
 *
 * model statistics:
 * name: "dice_Die"
 * # positions =     8 - start index =     8
 * # texels    =    14 - start index =    14
 * # normals   =     6 - start index =     6
 * # faces     =    12
 * contains converted polygons of triangle fan shape
 * includes masses and springs structures
 *
 * # positions total =    16
 * # texels total    =    28
 * # normals total   =    12
 * # faces total     =    24
 * includes masses and springs structures
 */

#ifndef __DICE_H__
#define __DICE_H__

#include <GL/gl.h>

#ifdef __cplusplus
#include <cstddef>
namespace std {
extern "C" {
#else
#include <stddef.h>
#endif

extern const size_t diceVertices;
extern GLfloat dicePositions[216];
extern GLfloat diceTexels[144];
extern GLfloat diceNormals[216];

/* all masses and springs */
/* indexing arrays: 3 ascending values per coordinate at index, index+1 and index+2 */
extern const size_t diceObjectsWithMass;
extern const size_t diceMasses[2];
extern const size_t diceMassFwdOffs[2];
extern const size_t diceMassVertices[2];
extern const size_t diceMassRevOffs[2];

extern const size_t diceFwdIndexI[72];
extern const size_t* diceFwdIndex[16];
extern const size_t diceFwdIndexLength[16];
extern const size_t diceRevIndex[72];

/* object metadata */
extern const size_t diceObjects;
extern const size_t diceObjectOffset[2];
extern const size_t diceObjectLength[2];
extern const char diceObjectNamesString[25];
extern const char* diceObjectNames[2];
extern const char diceTextureFilePath[34];

#ifdef __cplusplus
} // extern "C"
} // namespace std
#endif

#endif // __DICE_H__
