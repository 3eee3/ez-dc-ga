/*
 * Object3dModel.h
 *
 *  Created on: Jan 20, 2016
 *      Author: littlelion
 */

#ifndef OBJECT3DMODEL_H_
#define OBJECT3DMODEL_H_

#include <cstddef>
#include <iostream>
#include <string>

namespace std {

/**
 * Objects of this class represent a single 3D graphics object.
 * Write methods will generate C-source and header files for OpenGL integration.
 */
class Object3dModel {
	friend class ObjectFileReader;

public:
	Object3dModel(size_t nPositions,
			      size_t nTexels,
				  size_t nNormals,
				  size_t nFaces,
				  bool revMapping = false,
				  bool isMassSpring = false);

	virtual ~Object3dModel();

	void writeHfile(string folderPath, ostream* logfp = &cout);
	void writeCfile(string folderPath, ostream* logfp = &cout);
	void writeSingleHfile(string folderPath, ostream* logfp = &cout);
	bool isMassSpring();
	void setMassSpring(bool reverseMapping = false);
	void unsetMassSpring();
	bool hasConvertedPolygons();
	void writeStatistics(ostream* fp = &cout,  bool addOffsets = false, int connect = 0);

	string name;

private:
	void writeHeader(ofstream* fp, bool isHfile = false);
	void writeCpositions(ofstream* fp);
	void writeCtexels(ofstream* fp);
	void writeCnormals(ofstream* fp);
	void writeCmasses(ofstream* fp);
	void writeCsprings(ofstream* fp);
	void writeForwardIdx(ofstream* fp, size_t offs = 0, int mode = 0, string prefix = "");
	void writeReverseIdx(ofstream* fp, size_t offs, bool closed = true);
	void writeVertexData(ofstream* fp, float* data, size_t offs, size_t k,
			size_t n);
	void writeObjectSummary(ofstream* fp);
	static void genHdrHeader(const string hdr, ofstream* fp);
	static void genHdrFooter(const string hdr, ofstream* fp);
	void genDeclarations(ofstream* fp);
	void genCsrcBody(ofstream* fp, ostream* logfp);

	/* size informations */
	size_t nPositions;
	size_t nTexels;
	size_t nNormals;
	size_t nFaces;
	size_t nVertices;
	/* offset to first index in faces structure */
	size_t offsPos = 1;
	size_t offsTex = 1;
	size_t offsNor = 1;

	bool revMapping;          ///< activate reverse index array for position --> mass mapping
	bool massSpring;          ///< object with masses and springs
	bool hasConvPoly = false; ///< this object contains from polygons converted triangles
	float* positions;         ///< size: nPositions * 3
	float* texels;            ///< size: nTexels * 2
	float* normals;           ///< size: nNormals * 3
	int* faces;               ///< size: nFaces * 9 (unchanged indices from .obj file continuous for all objects beginning with 1)
};

} /* namespace std */

#endif /* OBJECT3DMODEL_H_ */
