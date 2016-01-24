//============================================================================
// Name        : blender2oGL_MassSpring.cpp
// Author      : Gerhard Aigner
// Version     :
// Copyright   : @
// Description : Converting tool to create c source and header files of
//               OpenGL rendered masses and springs from an .obj file supplied
//               by blender.
//============================================================================

#include <sys/stat.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Object3dModel.h"
#include "ObjectFileReader.h"

/**
 * Print a help message to the output stream 'out'
 * @param argv
 * @param out
 */
inline void printHelp(char* argv[], std::ostream* out = &std::cout) {
	std::string prog = std::string(argv[0]);
	size_t start = prog.find_last_of('/');
	if (start > prog.size()) {
		start = 0; // no '/' ? --> use whole string
	} else {
		++start;
	}
	*out << "USAGE: " << prog.substr(start) << " -options source-file.obj\n";
	*out << "Options are:\n";
	*out << "\t-t target-folder   - the destination folder (default: same as the folder\n";
	*out << "\t                     containing the source file)\n";
	*out << "\t-s (default)       - separate declarations (in .h file) and instances\n";
	*out << "\t                     (in .c file)\n";
	*out << "\t-c                 - combine contents into .h file\n";
	*out << "\t-m                 - merge all objects to single arrays\n";
	*out << "\t-a model           - add mass and spring structures to this model.\n";
	*out << "\t                     This option may be used multiple times to select\n";
	*out << "\t                     more than one active object. The name of the model\n";
	*out << "\t                     is composed of the input file-name without \".obj\"\n";
	*out << "\t                     suffix and the \"o\" entry in the file in the shape\n";
	*out << "\t                     of <source-file>_<o-entry>, e.g. \"myFile_myObject\".\n";
	*out << "\t-r                 - include reverse mapping for positions array.\n";
	*out << "\t                     This option takes only effect if the -a option\n";
	*out << "\t                     is passed at least once.\n";
	*out << "\t-o                 - add object metadata\n";
	*out << "\t-v                 - print statistics (verbose)\n";
	*out << "\t-h or -?           - show this help text\n";
	*out << "Hint: Don't use shortcuts like ~ or %HOMEPATH% in paths." << std::endl;
}

/**
 * Print a warning message to an output stream if model contains converted polygons.
 * @param model the 3D-model
 * @param logfp output stream to logfile
 */
void warnIfConvertedFaces(std::Object3dModel model, std::ostream* logfp = &std::cout) {
	if (model.hasConvertedPolygons()) {
		*logfp << "Warning: \"" << model.name
				<< "\" contains to triangles converted polygon faces." << std::endl;
	}
}

/**
 * Print an error message, usage information and exit with error state.
 * @param argv the arguments vector
 * @param msg an optional error message
 */
inline void exitErr(char* argv[], std::string msg = "") {
	std::cerr << msg << "\n" << std::endl;
	printHelp(argv, &std::cerr);
	exit(EXIT_FAILURE);
}

/**
 * @brief The main function.
 *
 * Two arguments are obligatory:
 * --> source-file.obj - the path to the .obj file
 * --> target-folder   - the path to the destination folder
 *
 * @param argc arguments count
 * @param argv arguments vector
 * @return exit status for caller
 */
int main(int argc, char* argv[]) {
    // process arguments
	bool separate = false;
	bool combine = false;
	bool merge = false;
	bool verbose = false;
	bool reverse = false;
	bool objectData = false;
	std::vector<std::string> activeObj;
	std::string srcPath;
	std::string dstPath;
	enum parse_state{UNPARSED, STARTED, DONE}
		srcParsed = UNPARSED;

	int arg = 1;
	while (arg < argc) {
		if (argv[arg][0] == '-' && srcParsed == STARTED) {
			srcParsed = DONE;
		}
		if (!strcmp(argv[arg], "-t")) {
			if (++arg < argc && argv[arg][0] != '-') {
			dstPath = std::string(argv[arg]);
			} else {
				exitErr(argv, "Invalid parameter for option -o");
			}
		} else if (!strcmp(argv[arg], "-s")) {
			separate = true;
		} else if (!strcmp(argv[arg], "-c")) {
			combine = true;
		} else if (!strcmp(argv[arg], "-m")) {
			merge = true;
		} else if (!strcmp(argv[arg], "-a")) {
				if (++arg < argc && argv[arg][0] != '-') {
					activeObj.push_back(std::string(argv[arg]));
				} else {
					exitErr(argv, "Invalid parameter for option -a");
				}
		} else if (!strcmp(argv[arg], "-r")) {
			reverse = true;
		} else if (!strcmp(argv[arg], "-o")) {
			objectData = true;
		} else if (!strcmp(argv[arg], "-v")) {
			verbose = true;
		} else if (!strcmp(argv[arg], "-h") || !strcmp(argv[arg], "-?")) {
			printHelp(argv);
			exit(EXIT_SUCCESS);
		} else {
			switch (srcParsed) {
			case UNPARSED:
				srcPath = argv[arg];
				srcParsed = STARTED;
				break;
			case STARTED:
				srcPath += " " + std::string(argv[arg]);
				break;
			case DONE:
				exitErr(argv, "Invalid argument" + std::string(argv[arg]));
				break;
			}

		}
		++arg;
	}
	if (!combine) {
		separate = true;
	}
	if (srcPath.empty()) {
		exitErr(argv, "No input file declared.");
	}
	if (dstPath.empty()) {
		size_t end = srcPath.find_last_of('/');
		if (end > srcPath.size()) {
			dstPath = "./"; // no '/' ? --> use program folder
		} else {
			dstPath = srcPath.substr(0, end + 1);
		}
	}
	if (combine && separate) {
		exitErr(argv, "Combined options -s and -c are invalid.");
	}
	{
		struct stat buf;
		stat(srcPath.c_str(), &buf);
		if (!S_ISREG(buf.st_mode)) {
			exitErr(argv, "Source: " + srcPath + " not found or is no valid file.");
		}
	}

    std::cout << "source file: " << srcPath << std::endl;
    std::cout << "target folder: " << dstPath <<std::endl;

    std::ObjectFileReader reader = std::ObjectFileReader(srcPath, reverse, objectData);

    // read data
    try {
    	reader.readAll();
    	std::cout << "\nImported " << reader.models.size() << " 3D-models." << std::endl;
        for(std::Object3dModel m : reader.models) {
        	std::cout << "\t" << m.name << std::endl;
        }
        std::cout << std::endl;
    } catch (std::exception &e) {
    	exitErr(argv, e.what());
    }
    for(size_t i = 0; i<reader.models.size(); ++i) {
    	for (std::string s : activeObj) {
    		if(!reader.models[i].name.compare(s)) {
    			reader.models[i].setMassSpring(reverse);
    			std::cout << "Selected for mass-spring processing: "
    					<< reader.models[i].name << std::endl;
    			break;
    		}
    	}
    }

    // generate output
    try {
    	if (merge) {
			std::cout << "\nprocessing merged model arrays" << std::endl;
			for (size_t i = 0; i < reader.models.size(); ++i) {
				warnIfConvertedFaces(reader.models[i]);
			}
			if (combine) {
				reader.writeSingleHfile(dstPath);
			} else {
				reader.writeHfile(dstPath);
				reader.writeCfile(dstPath);
			}
			if (verbose) {
				for (size_t i = 0; i < reader.models.size(); ++i) {
					reader.models[i].writeStatistics(&std::cout, true);
				}
			}
    	} else {
			for (size_t i = 0; i < reader.models.size(); ++i) {
				std::cout << "\nprocessing model \"" << reader.models[i].name << "\"" << std::endl;
				warnIfConvertedFaces(reader.models[i]);
				if (combine) {
					reader.models[i].writeSingleHfile(dstPath);
				} else {
					reader.models[i].writeHfile(dstPath);
					reader.models[i].writeCfile(dstPath);
				}
				if (verbose) {
					reader.models[i].writeStatistics();
				}
			}
    	}
    } catch (std::exception& e) {
    	exitErr(argv, e.what());
    }
    return EXIT_SUCCESS;
}
