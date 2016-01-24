/*
 * ObjectFileReader.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: littlelion
 */

#include "ObjectFileReader.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace std {

ObjectFileReader::ObjectFileReader(string path, bool reverseMass,
		bool objStatistics,
		bool inFileNameAsPrefix) :
	path(path), reverseMass(reverseMass),
	objStatistics(objStatistics),
	prefix(inFileNameAsPrefix ? "_" : "") {}

ObjectFileReader::~ObjectFileReader() {}

/**
 * Split a line into n tokens and store the converted float values into the data array.
 *
 * @param line string to read
 * @param n expected number of tokens
 * @param data target data structure
 * @param p offset to first write position
 */
void ObjectFileReader::tokenize(const string& line, const size_t n,
		float data[], size_t p) {
	char* l = new char[line.size() + 1];
	memcpy(l, line.c_str(), line.size() + 1);
	strtok(l, " ");
	for (size_t i = 0; i < n; ++i) {
		data[n * p + i] = atof(strtok(nullptr, " "));
	}
	delete[] l;
}

/**
 * @brief Read data from .obj file and build an instance of Object3dModel.
 *
 * This method extracts the data of one 3D object and maintains the internal state of the ObjFileReader.
 *
 * @return the Object3dModel instance
 */
Object3dModel ObjectFileReader::read() {
	if (hasNext()) {
		size_t p = 0;
		size_t t = 0;
		size_t n = 0;
		size_t f = 0;

		ifstream fObj;
		size_t offs = offset;
		fObj.open(path.c_str());
		if (fObj.good()) {
			fObj.seekg(offs);
			// count all entries from offset until next "o " line
			string line;
			bool done = false;
			bool warnConverted = false;

			while (!fObj.eof() && !done) {
				getline(fObj, line);
				offs = fObj.tellg();
				string entry = line.substr(0, 2);

				if (entry.compare("o ") == 0) {
					done = true;
				} else if (entry.compare("v ") == 0) {
					++p;
				} else if (entry.compare("vt") == 0) {
					++t;
				} else if (entry.compare("vn") == 0) {
					++n;
				} else if (entry.compare("f ") == 0) {
					char* l = new char[line.size()+1];
					memcpy(l, line.c_str(), line.size()+1);
					strtok(l, " ");
					int cnt = 0;
					while(strtok(nullptr, " ") != nullptr) ++cnt;
					delete[] l;
					f += cnt - 2;
					if (cnt > 3) warnConverted = true;
				}
			}
			totalVert += (3*f);

			// read data
			Object3dModel model = Object3dModel(p, t, n, f);
			model.name = string(objects[objNo]);
			model.hasConvPoly = warnConverted;
			done = false;
			offs = offset;
			fObj.clear();
			fObj.seekg(offs);
			p = 0;
			t = 0;
			n = 0;
			f = 0;
			while (!fObj.eof() && !done) {
				getline(fObj, line);
				offs = fObj.tellg();
				string entry = line.substr(0, 2);
				if (entry.compare("o ") == 0) {
					offset = offs;
					done = true;
				} else if (entry.compare("v ") == 0) { // Points: 'v <x> <y> <z>'
					tokenize(line, 3, model.positions, p);
					++p;
				} else if (entry.compare("vt") == 0) { // Texels: 'vt <u> <v>'
					tokenize(line, 2, model.texels, t);
					++t;
				} else if (entry.compare("vn") == 0) { // Normals: 'vn <x> <y> <z>'
					tokenize(line, 3, model.normals, n);
					++n;
				} else if (entry.compare("f ") == 0) { // faces: 'f X/T/N Y/T/N Z/T/N'
					// command format: f position_id/texture_coordinates_id/normal_id
					// possibly no texture coordinates available: fill with 0;
					// caution: all coordinates start with 1, so correct it by -1 if translated to array indices
					// if multiple objects are in a .obj file then all positions, texels and normals are indexed sequently,
					// so, the indices need to be corrected by the offset given by the array sizes of all previous objects.
					char* l = new char[line.size()+1];
					memcpy(l, line.c_str(), line.size()+1);
					strtok(l, " ");
					int cnt = 0;
					int temp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
					char* tok;
					while((tok = strtok(nullptr, " ")) != nullptr) {
						// store first vertex in temp[0..2], then toggle between temp[3..5] and temp[6..8]
						int i = cnt > 0 ? (1-cnt%2)*3+3 : 0;
						temp[i] = atoi(tok);
						if((tok = strchr(tok, '/')) != nullptr) {
							temp[i+1] = atoi(++tok);
							if((tok = strchr(tok, '/')) != nullptr) {
								temp[i+2] = atoi(++tok);
							}
						}

						// write face
						if (cnt >=2) {
							// vertex 1: take always the first index triplet P/T/N
							model.faces[9*f] = temp[0];
							model.faces[9*f+1] = temp[1];
							model.faces[9*f+2] = temp[2];
							// vertex 2: take temp[3]..temp[5] if cnt even, otherwise use temp[6]..temp[8]
							model.faces[9*f+3] = temp[cnt%2*3+3];
							model.faces[9*f+4] = temp[cnt%2*3+4];
							model.faces[9*f+5] = temp[cnt%2*3+5];
							// vertex 3: take opposite part of vertex 2 in range of temp[3..8], they're updated alternating
							model.faces[9*f+6] = temp[(1-cnt%2)*3+3];
							model.faces[9*f+7] = temp[(1-cnt%2)*3+4];
							model.faces[9*f+8] = temp[(1-cnt%2)*3+5];
							++f;
						}
						++cnt;
					}
					delete[] l;
				}
			}
			fObj.close();
			// update state for next object to read
			++objNo;
			model.offsPos = vOffs;
			model.offsTex = tOffs;
			model.offsNor = nOffs;
			vOffs += model.nPositions;
			tOffs += model.nTexels;
			nOffs += model.nNormals;
			models.push_back(model);
			return model;
		} else {
			throw runtime_error("Error reading file.");
		}
	} else {
		throw runtime_error("No data for generating object.");
	}
}

/**
 * Check if objects are available
 *
 * @retval true {if an object is available}
 * @retval false {otherwise}
 */
bool ObjectFileReader::hasNext() {
	if (!isInit) {
		init();
	}
	return objNo < objects.size();
}

/**
 * Read all 3D-models in one turn.
 *
 * @return the number of models
 */
int ObjectFileReader::readAll() {
	int n = 0;
	while (hasNext()) {
		Object3dModel model = read();
		++n;
	}
	return n;
}

/**
 * @brief Initialize the ObjFileReader object.
 *
 * The method reads all entries of type "o" (object) and sets the initial stream-offset.
 */
void ObjectFileReader::init() {
	if (!isInit) {
		// prefix for object names
		if (prefix.compare("_") == 0) {
			size_t start = path.find_last_of('/');
			if (start > path.size()) {
				start = 0; // no '/' found --> take whole string
			} else {
				++start;
			}
			prefix = path.substr(start, path.find_last_of('.') - start);
			prefix.append("_");
		}

	    // read object names
		ifstream fObj;
		size_t offs = 0;
		fObj.open(path.c_str());
		if (fObj.good()) {
			// read all "o " entries and store them as object names
			string line;
			while (!fObj.eof()) {
				getline(fObj, line);
				offs = fObj.tellg();
				string entry = line.substr(0, 2);
				if (entry.compare("o ") == 0) {
					if (offset == 0) {
						// set initial stream-offset
						offset = offs;
					}
					size_t end = line.find_last_not_of(" \t");
					string obj = prefix;
					objects.push_back(obj.append(line.substr(2, end-1))); // end-(start=2)+1
				}
			}
			fObj.close();
		} else {
			throw runtime_error("Error reading file.");
		}
		textureFilePath = "/not_implemented_yet/edit/by.hand"; //TODO implement parser for .mat file

		isInit = true;
	}
}

void ObjectFileReader::genDeclarations(string hdr, ofstream* fp) {
	// declarations
	*fp << "extern const size_t " << hdr << "Vertices;\n";
	*fp << "extern GLfloat " << hdr << "Positions[" << totalVert * 3 << "];\n";
	*fp << "extern GLfloat " << hdr << "Texels[" << totalVert * 2 << "];\n";
	*fp << "extern GLfloat " << hdr << "Normals[" << totalVert * 3 << "];\n";
	size_t o = 0;
	size_t n = 0;
	size_t p = 0;
	for (Object3dModel m : models) {
		if (m.isMassSpring()) {
			++o;
			n += m.nVertices;
			p += m.nPositions;
		}
	}
	if (o > 0) {
		*fp << "\n/* all masses and springs */\n";
		*fp << "/* indexing arrays: 3 ascending values per coordinate at index, index+1 and index+2 */\n";

		string hdr = prefix.substr(0, prefix.size()-1);
		*fp << "extern const size_t " << hdr << "ObjectsWithMass;\n";

		*fp << "extern const size_t " << hdr << "Masses[" << o << "];\n";
		*fp << "extern const size_t " << hdr << "MassFwdOffs[" << o << "];\n";
		if (reverseMass) {
			*fp << "extern const size_t " << hdr << "MassVertices[" << o << "];\n";
			*fp << "extern const size_t " << hdr << "MassRevOffs[" << o << "];\n";
			*fp << "extern const size_t " << hdr << "MassRevOffsOrig[" << o << "];\n";
		}
		*fp << "\nextern const size_t " << hdr << "FwdIndexI[" << n << "];\n";
		*fp << "extern const size_t* " << hdr << "FwdIndex[" << p << "];\n";
		*fp << "extern const size_t " << hdr << "FwdIndexLength[" << p << "];\n";
		if (reverseMass) {
			*fp << "extern const size_t " << hdr << "RevIndex[" << n << "];\n";
		}
	}
	if (objStatistics) {
		*fp << "\n/* object metadata */\n";
		*fp << "extern const size_t " << hdr << "Objects;\n";
		*fp << "extern const size_t " << hdr << "ObjectOffset[" << objects.size() << "];\n";
		*fp << "extern const size_t " << hdr << "ObjectLength[" << objects.size() << "];\n";
		size_t len = 0;
		for (string o : objects) {
			len += o.length()+1;
		}
		*fp << "extern const char " << hdr << "ObjectNamesString[" << len << "];\n";
		*fp << "extern const char* " << hdr << "ObjectNames[" << objects.size() << "];\n";
		*fp << "extern const char " << hdr << "TextureFilePath[" << textureFilePath.length()+1 << "];\n";
	}
	*fp << flush;
}

/**
 * Generate the body of a source file with all definitions.
 * @param fp output stream to target file
 * @param logfp logging stream
 */
void ObjectFileReader::genSrcBody(string hdr, ofstream* fp, ostream* logfp) {
    // vertices
    *fp << "const size_t " << hdr << "Vertices = " << totalVert << ";\n" << endl;
	*logfp << "positions ... " << flush;
	writeCpositions(fp);
	*logfp << "texels ... " << flush;
	writeCtexels(fp);
	*logfp << "normals ... " << flush;
	writeCnormals(fp);
	bool hasMass = false;
	for (Object3dModel o : models) {
		if (o.isMassSpring()) {
			hasMass = true;
		}
	}
	if (hasMass) {
		*logfp << "masses ... " << flush;
		writeCmasses(fp);
		*logfp << "springs ... " << flush;
		writeCsprings(fp);
		*logfp << "fwd indices ... " << flush;
		writeForwardIdx(fp);
		if (reverseMass) {
			*logfp << "rev indices ... " << flush;
			writeReverseIdx(fp);
		}
	}
	*logfp << "metadata ... " << flush;
	writeObjectSummary(fp);
}

/**
 * Generate a header file for the 3D-model.
 * @param folderPath the target folder
 * @param logfp output to log file (nullptr to suppress output)
 */
void ObjectFileReader::writeHfile(string folderPath, ostream* logfp) {
	if (hasNext()) {
		throw runtime_error("ObjectFileReader::writeHfile: Read task incomplete.");
	}
	ofstream fp;
	string path = folderPath;
    string sep = folderPath[folderPath.size()-1] == '/' ? "" : "/";
	string hdr = prefix.substr(0, prefix.size()-1);
	path.append(sep).append(hdr).append(".h");

	fp.open(path.c_str());
	if (fp.good()) {
	    *logfp << "generating header file ... " << flush;

		// headlines and statistics
		writeHeader(&fp, true);

		// include guards, includes and namespace open
		Object3dModel::genHdrHeader(hdr, &fp);

	    // declarations
		genDeclarations(hdr, &fp);

	    // include guards and namespace close
	    Object3dModel::genHdrFooter(hdr, &fp);

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
void ObjectFileReader::writeCfile(string folderPath, ostream* logfp) {
	if (hasNext()) {
		throw runtime_error("ObjectFileReader::writeHfile: Read task incomplete.");
	}
	ofstream fp;
	string path = folderPath;
    string sep = folderPath[folderPath.size()-1] == '/' ? "" : "/";
	string hdr = prefix.substr(0, prefix.size()-1);
	path.append(sep).append(hdr).append(".c");

	fp.open(path.c_str());
	if (fp.good()) {
	    *logfp << "generating source file ... " << flush;
		// headlines and statistics
		writeHeader(&fp);

		// include files
		fp << "#include <stddef.h>\n";
		fp << "#include <GL/gl.h>\n\n";
	    fp << "#include " << "\"" << hdr << ".h" << "\"\n\n";

	    // vertices
		genSrcBody(hdr, &fp, logfp);

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
void ObjectFileReader::writeSingleHfile(string folderPath, ostream* logfp) {
	if (hasNext()) {
		throw runtime_error("ObjectFileReader::writeHfile: Read task incomplete.");
	}
	ofstream fp;
	string path = folderPath;
    string sep = folderPath[folderPath.size()-1] == '/' ? "" : "/";
	string hdr = prefix.substr(0, prefix.size()-1);
	path.append(sep).append(hdr).append(".h");

	fp.open(path.c_str());
	if (fp.good()) {
	    *logfp << "generating header file ... " << flush;

		// headlines and statistics
		writeHeader(&fp, true);

		// include guards, includes and namespace open
		Object3dModel::genHdrHeader(hdr, &fp);

	    // vertices
		genSrcBody(hdr, &fp, logfp);

	    // include guards and namespace close
		Object3dModel::genHdrFooter(hdr, &fp);

		*logfp << "done." << endl;
		*logfp << "written to \"" << path << "\"" << endl;

	    fp.close();
	} else {
		throw runtime_error("Error writing source file:" + path);
	}
}

/**
 * Generate a header for the C files.
 * @param fp target file
 * @param isHfile @c true for .h file, @c false for .c file
 */
void ObjectFileReader::writeHeader(ofstream* fp, bool isHfile) {
	// MAX_N is the max number of names in the header
	// WWW is field width for integers in the summary block
#define MAX_N 3
#define WWW 5
    *fp << "// This is a ";
    if (isHfile) {
    	*fp << "C-header file (.h)";
    } else {
    	*fp << "C-source file (.c)";
    }
    *fp << " for the model" << (models.size()>1 ? "s" : "") << " \"";
    for (size_t i = 0; i < models.size() && i < MAX_N; ++i) {
    	*fp << models[i].name << "\"";
    	if (i < models.size()-1 && i < MAX_N-1) {
    		*fp << ", \"";
    	} else if (i < models.size()-1 && i < MAX_N) {
    		*fp << " ...";
    	}
    }
    *fp << "\n// Don't edit! This is an auto-generated file by blender2oGL. Modifications are not permanent.\n\n";
    for (size_t i = 0; i < models.size(); ++i) {
    	models[i].writeStatistics(fp, true, i==0 ? 1 : 2);
    }
    // summary
    *fp << " *\n * # positions total = " << setw(WWW) << vOffs - 1;
	*fp <<   "\n * # texels total    = " << setw(WWW) << tOffs - 1;
	*fp <<   "\n * # normals total   = " << setw(WWW) << nOffs - 1;
	*fp <<   "\n * # faces total     = " << setw(WWW) << totalVert / 3 << "\n";
	int m = 0;
	for (Object3dModel o : models) {
		if (o.isMassSpring()) {
			m += o.nPositions;
		}
	}
	if (m > 0) {
		*fp << " * includes masses and springs structures\n";//FIXME add size
	}
	*fp << " */\n" << endl;
#undef WWW
#undef MAX_N
}

/**
 * Generate positions array
 * @param fp target c-file
 */
void ObjectFileReader::writeCpositions(ofstream* fp) {
	string hdr = prefix.substr(0, prefix.size()-1);
    *fp << "GLfloat " << hdr << "Positions[" << totalVert*3 << "] = {\n";
    *fp << setiosflags(ios::showpoint);
    for (Object3dModel m : models) {
    	m.writeVertexData(fp, m.positions, m.offsPos, 0, 3);
    }
    *fp << "};\n" << endl;
}

/**
 * Generate texels array
 * @param fp target c-file
 */
void ObjectFileReader::writeCtexels(ofstream* fp){
	string hdr = prefix.substr(0, prefix.size()-1);
    *fp << "GLfloat " << hdr << "Texels[" << totalVert*2 << "] = {\n";
    *fp << setiosflags(ios::showpoint);
    for (Object3dModel m : models) {
    	m.writeVertexData(fp, m.texels, m.offsTex, 1, 2);
    }
    *fp << "};\n" << endl;
}

/**
 * Generate normals array
 * @param fp target c-file
 */
void ObjectFileReader::writeCnormals(ofstream* fp) {
	string hdr = prefix.substr(0, prefix.size()-1);
    *fp << "GLfloat " << hdr << "Normals[" << totalVert*3 << "] = {\n";
    *fp << setiosflags(ios::showpoint);
    for (Object3dModel m : models) {
    	m.writeVertexData(fp, m.normals, m.offsNor, 2, 3);
    }
    *fp << "};\n" << endl;
}

/**
 * Generate masses array size informations
 * @param fp target c-file
 */
void ObjectFileReader::writeCmasses(ofstream* fp) {
	size_t o = 0;
	for (Object3dModel m : models) {
		if (m.massSpring) {
			++o;
		}
	}
	if (o > 0) {
		string hdr = prefix.substr(0, prefix.size()-1);
		*fp << "const size_t " << hdr << "ObjectsWithMass = " << o << ";\n\n";

		*fp << "const size_t " << hdr << "Masses[" << o << "] = {\n";
		for (Object3dModel m : models) {
			if (m.massSpring) {
				*fp << "\t" << m.nPositions << ",\n";
			}
		}
		*fp << "};\n" << endl;

		*fp << "const size_t " << hdr << "MassFwdOffs[" << o << "] = {\n";
		int ofs = 0;
		for (Object3dModel m : models) {
			if (m.massSpring) {
				*fp << "\t" << ofs << ",\n";
				ofs += m.nPositions;
			}
		}
		*fp << "};\n" << endl;

		if (reverseMass) {
			*fp << "const size_t " << hdr << "MassVertices[" << o << "] = {\n";
			for (Object3dModel m : models) {
				if (m.massSpring) {
					*fp << "\t" << m.nVertices << ",\n";
				}
			}
			*fp << "};\n" << endl;

			*fp << "const size_t " << hdr << "MassRevOffs[" << o << "] = {\n";
			ofs = 0;
			for (Object3dModel m : models) {
				if (m.massSpring) {
					*fp << "\t" << ofs << ",\n";
					ofs += m.nVertices;
				}
			}
			*fp << "};\n" << endl;

			*fp << "const size_t " << hdr << "MassRevOffsOrig[" << o << "] = {\n";
			ofs = 0;
			for (Object3dModel m : models) {
				if (m.massSpring) {
					*fp << "\t" << ofs << ",\n";
				}
				ofs += m.nVertices;
			}
			*fp << "};\n" << endl;
		}
	}
}

/**
 * Generate springs array
 * @param fp target c-file
 */
void ObjectFileReader::writeCsprings(ofstream* fp) {
	string hdr = prefix.substr(0, prefix.size()-1);
	// TODO stub
}

/**
 * Generate index structures for forward mapping position --> mass.
 * @param fp target c-file
 */
void ObjectFileReader::writeForwardIdx(ofstream* fp) {
	size_t n = 0;
	size_t p = 0;
	for (Object3dModel m : models) {
		if (m.massSpring) {
			n += m.nVertices;
			p += m.nPositions;
		}
	}
	if (n > 0) {
		string hdr = prefix.substr(0, prefix.size()-1);
		// --> index data
		*fp << "const size_t " << hdr << "FwdIndexI[" << n << "] = {\n";
		size_t ofs = 0;
		for (Object3dModel m : models) {
			if (m.massSpring) {
				m.writeForwardIdx(fp, ofs, 1);
			}
			ofs += m.nVertices;
		}
		*fp << "};\n" << endl;

		// --> pointers
		*fp << "const size_t* " << hdr << "FwdIndex[" << p << "] = {\n";
		ofs = 0;
		for (Object3dModel m : models) {
			if (m.massSpring) {
				m.writeForwardIdx(fp, ofs, 2, hdr);
				ofs += m.nVertices;
			}
		}
		*fp << "};\n" << endl;

		// --> lengths
		*fp << "const size_t " << hdr << "FwdIndexLength[" << p << "] = {\n";
		for (Object3dModel m : models) {
			if (m.massSpring) {
				m.writeForwardIdx(fp, 0, 3);
			}
		}
		*fp << "};\n" << endl;
	}
}

/**
 * Generate index structures for reverse mapping mass --> position.
 * @param fp target c-file
 */
void ObjectFileReader::writeReverseIdx(ofstream* fp) {
	size_t n = 0;
	for (Object3dModel m : models) {
		if (m.revMapping) {
			n += m.nVertices;
		}
	}
	if (n > 0) {
		string hdr = prefix.substr(0, prefix.size()-1);
		*fp << "const size_t " << hdr << "RevIndex[" << n << "] = {\n";
		size_t ofs = 1;
		for (Object3dModel m : models) {
			if (m.revMapping) {
				m.writeReverseIdx(fp, ofs, false);
			} else {
				ofs += m.nPositions;
			}
		}
		*fp << "};\n" << endl;
	}
}

/**
 * Generate some structures with metadata about the 3D-objects in the model
 * @param fp target c-file
 */
void ObjectFileReader::writeObjectSummary(ofstream* fp) {
	string hdr = prefix.substr(0, prefix.size()-1);
    *fp << "const size_t " << hdr << "Objects = " << models.size() << ";\n\n";

	*fp << "const size_t " << hdr << "ObjectOffset[" << models.size() << "] = {\n";
	int ofs = 0;
	for (Object3dModel m : models) {
		*fp << "\t" << ofs << ",\n";
		ofs += m.nVertices;
	}
	*fp << "};\n" << endl;

	*fp << "const size_t " << hdr << "ObjectLength[" << models.size() << "] = {\n";
	for (Object3dModel m : models) {
		*fp << "\t" << m.nVertices << ",\n";
	}
	*fp << "};\n" << endl;

	size_t len = 0;
	for (string o : objects) {
		len += o.length()+1;
	}
	*fp << "const char " << hdr << "ObjectNamesString[" << len << "] = {\n";
	for (string o : objects) {
		*fp << "\t\"" << o << "\\0\"\n";
	}
	*fp << "};\n" << endl;
	*fp << "const char* " << hdr << "ObjectNames[" << objects.size() << "] = {\n";
	len = 0;
	for (string o : objects) {
		*fp << "\t&" << hdr << "ObjectNamesString[" << len << "],\n";
		len += o.size()+1;
	}
	*fp << "};\n" << endl;

	*fp << "const char " << hdr << "TextureFilePath[" << textureFilePath.length()+1 << "] = \"";
	*fp << textureFilePath << "\";" << endl;
}

} /* namespace std */
