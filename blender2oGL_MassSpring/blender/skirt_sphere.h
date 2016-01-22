// This is a C-header file (.h) for the models "skirt_sphere_Grid", "skirt_sphere_Icosphere", "skirt_sphere_Cone"
// Don't edit! This is an auto-generated file by blender2oGL. Modifications are not permanent.

/*
 * model statistics:
 * name: "skirt_sphere_Grid"
 * # positions =   361 - start index =     0
 * # texels    =     4 - start index =     0
 * # normals   =     1 - start index =     0
 * # faces     =   648
 * contains converted polygons of triangle fan shape
 *
 * model statistics:
 * name: "skirt_sphere_Icosphere"
 * # positions =    42 - start index =   361
 * # texels    =     3 - start index =     4
 * # normals   =    80 - start index =     1
 * # faces     =    80
 *
 * model statistics:
 * name: "skirt_sphere_Cone"
 * # positions =  3456 - start index =   403
 * # texels    =  3723 - start index =     7
 * # normals   =    35 - start index =    81
 * # faces     =  6144
 *
 * # positions total =  3859
 * # texels total    =  3730
 * # normals total   =   116
 * # faces total     =  6872
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
extern GLfloat skirt_spherePositions[61848];
extern GLfloat skirt_sphereTexels[41232];
extern GLfloat skirt_sphereNormals[61848];

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