/*
 * Object3dModel.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: littlelion
 */

#include "Object3dModel.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <stdexcept>


namespace std {



/**
 * A container for all 3D-model specific data.
 * @param nPositions
 * @param nTexels
 * @param nNormals
 * @param nFaces
 * @param isMassSpring
 */
Object3dModel::Object3dModel(size_t nPositions,
		      size_t nTexels,
			  size_t nNormals,
			  size_t nFaces,
			  bool isMassSpring) :
	nPositions(nPositions),
	nTexels(nTexels),
	nNormals(nNormals),
	nFaces(nFaces),
	nVertices(nFaces * 3),
	massSpring(isMassSpring),
	positions(new float[nPositions * 3]),
	texels(new float[nTexels * 2]),
	normals(new float[nNormals * 3]),
	faces(new int[nFaces * 9])  {}

/**
 * Default destructor.
 */
Object3dModel::~Object3dModel() {}

/**
 * Generate a header file for the 3D-model.
 * @param folderPath the target folder
 * @param logfp output to log file (nullptr to suppress output)
 */
void Object3dModel::writeHfile(string folderPath, ostream* logfp) {
	ofstream fp;
	string path = folderPath;
    string sep = folderPath[folderPath.size()-1] == '/' ? "" : "/";
	path.append(sep).append(name).append(".h");

	fp.open(path.c_str());
	if (fp.good()) {
	    *logfp << "generating header file ... " << flush;

		// headlines and statistics
		writeHeader(&fp, true);

		// include guards
		string hdr = name;
		transform(hdr.begin(), hdr.end(), hdr.begin(), ::toupper);
		fp << "#ifndef __" << hdr << "_H__\n";
		fp << "#define __" << hdr << "_H__\n\n";
		fp << "#include <GL/gl.h>\n\n";

	    // declarations
	    fp << "const size_t " << name << "Vertices;\n";
	    fp << "GLfloat " << name << "Positions[" << nVertices*3 << "];\n";
	    fp << "GLfloat " << name << "Texels[" << nVertices*2 << "];\n";
	    fp << "GLfloat " << name << "Normals[" << nVertices*3 << "];\n\n";

	    if (massSpring) {
		    fp << "GLfloat " << name << "Masses[" << nVertices << "];\n";
		    fp << "GLfloat " << name << "Springs[" << nVertices*2 << "];\n\n";//FIXME size
		}

	    // include guards
	    fp << "#endif // __" << hdr << "_H__" << endl;

		*logfp << "done." << endl;
		*logfp << "written to \"" << path << "\"" << endl;

	    fp.close();
	} else {
		throw runtime_error("Error writing header file:" + path);
	}
}

/**
 * Generate a source file for the 3D-model.
 * @param folderPath the target folder
 * @param logfp output to log file (nullptr to suppress output)
 */
void Object3dModel::writeCfile(string folderPath, ostream* logfp) {
	ofstream fp;
	string path = folderPath;
    string sep = folderPath[folderPath.size()-1] == '/' ? "" : "/";
	path.append(sep).append(name).append(".c");

	fp.open(path.c_str());
	if (fp.good()) {
	    *logfp << "generating source file ... " << flush;
		// headlines and statistics
		writeHeader(&fp);

		// header
	    fp << "#include " << "\"" << name << ".h" << "\"\n\n";

	    // vertices
	    fp << "const size_t " << name << "Vertices = " << nVertices << ";\n";
	    fp << endl;

	    *logfp << "positions ... " << flush;
		writeCpositions(&fp);
		*logfp << "texels ... " << flush;
		writeCtexels(&fp);
		*logfp << "normals ... " << flush;
		writeCnormals(&fp);

		if (massSpring) {
			*logfp << "masses ... " << flush;
			writeCmasses(&fp);
			*logfp << "springs ... " << flush;
			writeCsprings(&fp);
		}
		*logfp << "done." << endl;
		*logfp << "written to \"" << path << "\"" << endl;

	    fp.close();
	} else {
		throw runtime_error("Error writing source file:" + path);
	}
}

/**
 * Generate a stand-alone header file with all definitions for the 3D-model.
 * @param folderPath the target folder
 * @param logfp output to log file (nullptr to suppress output)
 */
void Object3dModel::writeSingleHfile(string folderPath, ostream* logfp) {
	ofstream fp;
	string path = folderPath;
    string sep = folderPath[folderPath.size()-1] == '/' ? "" : "/";
	path.append(sep).append(name).append(".h");

	fp.open(path.c_str());
	if (fp.good()) {
	    *logfp << "generating header file ... " << flush;

		// headlines and statistics
		writeHeader(&fp, true);

		// include guards
		string hdr = name;
		transform(hdr.begin(), hdr.end(), hdr.begin(), ::toupper);
		fp << "#ifndef __" << hdr << "_H__\n";
		fp << "#define __" << hdr << "_H__\n\n";

		fp << "#ifdef __cplusplus\n";
		fp << "namespace std {\n";
		fp << "#endif\n\n";

		fp << "#include <GL/gl.h>\n\n";

	    // vertices
	    fp << "const size_t " << name << "Vertices = " << nVertices << ";\n";
	    fp << endl;

	    *logfp << "positions ... " << flush;
		writeCpositions(&fp);
		*logfp << "texels ... " << flush;
		writeCtexels(&fp);
		*logfp << "normals ... " << flush;
		writeCnormals(&fp);

		if (massSpring) {
			*logfp << "masses ... " << flush;
			writeCmasses(&fp);
			*logfp << "springs ... " << flush;
			writeCsprings(&fp);
		}
		fp << "#ifdef __cplusplus\n";
		fp << "} // namespace std\n";
		fp << "#endif\n\n";
	    fp << "#endif // __" << hdr << "_H__" << endl;


		*logfp << "done." << endl;
		*logfp << "written to \"" << path << "\"" << endl;

	    fp.close();
	} else {
		throw runtime_error("Error writing source file:" + path);
	}
}

/**
 * Check if masses and springs structures are to generate in output files.
 * @retval true {create masses and springs data structures}
 * @retval false {write only data for solid objects}
 */
bool Object3dModel::isMassSpring() {
	return massSpring;
}

/**
 * Change object state to mass-spring model.
 */
void Object3dModel::setMassSpring() {
	massSpring = true;
}

/**
 * Change object state to solid model.
 */
void Object3dModel::unsetMassSpring() {
	massSpring = false;
}

/**
 * Check if .obj file contained polygons which have been converted to triangles.
 *
 * OpenGL supports polygon faces with an arbitrary number of vertices. To avoid
 * artefacts caused by putting a texture onto a non planar surface these polygons
 * are converted to a fan of triangles.
 *
 * @retval true {contains at least one converted polygon}
 * @retval false {only triangle shaped faces extracted from .obj file}
 */
bool Object3dModel::hasConvertedPolygons() {
	return hasConvPoly;
}

/**
 * Print a summary of the 3D model
 *
 * @param fp target file
 * @param addOffsets add starting offset informations
 * @param connect 0 = no, 1 = bottom open (first), 2 = both open (middle), 3 = top open (last part)
 */
void Object3dModel::writeStatistics(ostream* fp, bool addOffsets, int connect) {
	// WW is field width for integers in the summary block
#define WW 5
	*fp << (connect==2 || connect==3 ? " *\n" : "/*\n");
	*fp << " * model statistics:\n";
	*fp << " * name: \"" << name << "\"\n";
	*fp << " * # positions = " << setw(WW) << nPositions;
	if (addOffsets) {
		*fp << " - start index = " << setw(WW) << offsPos-1;
	}
	*fp << "\n * # texels    = " << setw(WW) << nTexels;
	if (addOffsets) {
		*fp << " - start index = " << setw(WW) << offsTex-1;
	}
	*fp << "\n * # normals   = " << setw(WW) << nNormals;
	if (addOffsets) {
		*fp << " - start index = " << setw(WW) << offsNor-1;
	}
	*fp << "\n * # faces     = " << setw(WW) << nFaces << "\n";
	if (hasConvPoly) {
		*fp << " * contains converted polygons of triangle fan shape" << "\n";
	}
	if (massSpring) {
		*fp << " * includes masses and springs structures\n";//FIXME add size
	}
	if (connect==1 || connect==2) {
		*fp << flush;
	} else {
		*fp << " */" << endl;
	}
#undef WW
}

/**
 * Generate a header for the C files.
 * @param fp target file
 * @param isHfile @c true for .h file, @c false for .c file
 */
void Object3dModel::writeHeader(ofstream* fp, bool isHfile) {
    *fp << "// This is a ";
    if (isHfile) {
    	*fp << "C-header file (.h)";
    } else {
    	*fp << "C-source file (.c)";
    }
    *fp << " for the model \"" << name << "\"\n";
    *fp << "// Don't edit! This is an auto-generated file by blender2oGL. Modifications are not permanent.\n\n";
    writeStatistics(fp);
    *fp << endl;
}

/**
 * Write data block of a 3D-object.
 * @param fp file pointer to target
 * @param data array of vertex data e.g. positions, texels, normals
 * @param offs offset to first entry
 * @param k offset in faces structure (0 = positions, 1 = texels, 2 = normals)
 * @param n number of values per coordinate (usually for texels 2 and for positions or normals 3)
 */
void Object3dModel::writeVertexData(ofstream* fp, float* data, size_t offs,
		size_t k, size_t n) {
	for (size_t i = 0; i < nFaces; ++i) {
		for (int j = 0; j < 9; j += 3) {
			int a = (faces[9 * i + j + k] - offs) * n;
			*fp << "\t";
			for (size_t s = 0; s < n; ++s) {
				*fp << data[a + s] << (s<n-1 ? "f, " : "f,");
			}
			*fp << "\n";
		}
		*fp << flush;
	}
}

/**
 * Generate positions array
 * @param fp target c-file
 */
void Object3dModel::writeCpositions(ofstream* fp) {
    *fp << "GLfloat " << name << "Positions[" << nVertices*3 << "] = {\n";
    *fp << setiosflags(ios::showpoint);
	writeVertexData(fp, positions, offsPos, 0, 3);
    *fp << "};\n" << endl;
}

/**
 * Generate texels array
 * @param fp target c-file
 */
void Object3dModel::writeCtexels(ofstream* fp){
    *fp << "GLfloat " << name << "Texels[" << nVertices*2 << "] = {\n";
	*fp << setiosflags(ios::showpoint);
	writeVertexData(fp, texels, offsTex, 1, 2);
    *fp << "};\n" << endl;
}

/**
 * Generate normals array
 * @param fp target c-file
 */
void Object3dModel::writeCnormals(ofstream* fp) {
    *fp << "GLfloat " << name << "Normals[" << nVertices*3 << "] = {\n";
	*fp << setiosflags(ios::showpoint);
	writeVertexData(fp, normals, offsNor, 2, 3);
    *fp << "};\n" << endl;
}

/**
 * Generate masses array
 * @param fp target c-file
 */
void Object3dModel::writeCmasses(ofstream* fp) {
	// TODO stub
}

/**
 * Generate sprngs array
 * @param fp target c-file
 */
void Object3dModel::writeCsprings(ofstream* fp) {
	// TODO stub
}

} /* namespace std */
