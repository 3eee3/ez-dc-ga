/*
 * ResourcePath.h
 *
 *  Created on: Nov 9, 2015
 *      Author: littlelion
 */

#ifndef RESOURCE_PATH_H_
#define RESOURCE_PATH_H_

#include <iostream>
#include <string>

using namespace std;

/**
 * @brief Get the path of the application folder.
 *
 * The path returned is an absolute path with trailing path separator.
 * The first call of this function initializes the path constant and thus,
 * it needs non-null values for argc and argv. If the initialization fails, the return value is an empty string.
 * @param argc the argumants count of the @c main() function on first call, omit or set to 0 on succeeding calls
 * @param argv the arguments list provided by the @c main() function on first call, omit or set to @c nullptr on succeeding calls
 * @return the base path or an empty string if not initialized.
 */
string getBasePath(int /*argc = 0*/, char* argv[] = nullptr) {
	// make this function system independent
#ifdef _WIN32
	const char PATH_SEP = '\\';
#else
	const char PATH_SEP = '/';
#endif

	static string basePath;

	if(basePath.empty() && argv != nullptr) {
		string path = argv[0];
		size_t n = path.rfind(PATH_SEP);
		path = path.substr(0, n+1);
		if (!path.empty()) {
			basePath = path;
		}
	}
	return basePath;
}

/**
 * @brief Get path to resource file relative to the BasePath().
 *
 * The path returned is an absolute path resolved from ../resources/<filePath>.
 * If the path to a sub-folder is required use the path separator supported
 * by the target system.
 * @param filePath the path to the requested file in the resources folder
 * @param resourcesFoder name of the folder without path separators
 *        or empty string to address the parent folder, default is "resources"
 * @param argc the arguments count of the @c main() function on first call
 * @param argv the arguments list of the @c main() function on first call
 * @return the base path to the resource folder appended by filePath
 */
string getResourcePath (string filePath,
						string resourcesFolder = "resources",
						int argc = 0,
						char* argv[] = nullptr) {

	// make this function system independent
#ifdef _WIN32
	const char PATH_SEP = '\\';
#else
	const char PATH_SEP = '/';
#endif

	string path = getBasePath(argc, argv);

	// replace the last sub-folder with "/resources/"
	size_t n = path.rfind(PATH_SEP,path.length()-2)+1;
	path = path.substr(0, n);
	if (!resourcesFolder.empty()) {
		path = path + resourcesFolder + PATH_SEP;
	}

	return path + filePath;
}

#endif /* RESOURCEPATH_H_ */
