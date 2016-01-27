/**
 * @file
 * This is a C-header file (.h) for the models "skirt_sphere_Cone", "skirt_sphere_Grid", "skirt_sphere_Icosphere"
 * Don't edit! This is an auto-generated file by blender2oGL. Modifications are not permanent.
 */

/*
 * model statistics:
 * name: "skirt_sphere_Cone"
 * # positions =   353 - start index =     0
 * # texels    =   416 - start index =     0
 * # normals   =    98 - start index =     0
 * # faces     =   608
 * contains converted polygons of triangle fan shape
 * includes masses and springs structures
 *
 * model statistics:
 * name: "skirt_sphere_Grid"
 * # positions =   361 - start index =   353
 * # texels    =     4 - start index =   416
 * # normals   =     1 - start index =    98
 * # faces     =   648
 * contains converted polygons of triangle fan shape
 *
 * model statistics:
 * name: "skirt_sphere_Icosphere"
 * # positions =    42 - start index =   714
 * # texels    =     3 - start index =   420
 * # normals   =    80 - start index =    99
 * # faces     =    80
 *
 * # positions total =   756
 * # texels total    =   423
 * # normals total   =   179
 * # faces total     =  1336
 * includes masses and springs structures
 */

#ifndef __SKIRT_SPHERE_H__
#define __SKIRT_SPHERE_H__

#include <GL/gl.h>

#ifdef __cplusplus
#include <cstddef>
namespace std {
extern "C" {
#else
#include <stddef.h>
#endif

extern const size_t skirt_sphereVertices;
extern GLfloat skirt_spherePositions[12024];
extern GLfloat skirt_sphereTexels[8016];
extern GLfloat skirt_sphereNormals[12024];

/* all masses and springs */
/* indexing arrays: 3 ascending values per coordinate at index, index+1 and index+2 */
extern const size_t skirt_sphereObjectsWithMass;
extern const size_t skirt_sphereMasses[1];
extern const size_t skirt_sphereMassFwdOffs[1];
extern const size_t skirt_sphereMassVertices[1];
extern const size_t skirt_sphereMassRevOffs[1];
extern const size_t skirt_sphereMassRevOffsOrig[1];

extern const size_t skirt_sphereFwdIndexI[1824];
extern const size_t* skirt_sphereFwdIndex[353];
extern const size_t skirt_sphereFwdIndexLength[353];
extern const size_t skirt_sphereRevIndex[1824];

/* object metadata */
extern const size_t skirt_sphereObjects;
extern const size_t skirt_sphereObjectOffset[3];
extern const size_t skirt_sphereObjectLength[3];
extern const char skirt_sphereObjectNamesString[59];
extern const char* skirt_sphereObjectNames[3];
extern const char skirt_sphereTextureFilePath[34];

#ifdef __cplusplus
} // extern "C"
} // namespace std
#endif

#endif // __SKIRT_SPHERE_H__
