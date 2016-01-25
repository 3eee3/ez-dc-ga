/*
 * Texture.h
 *
 *  Created on: Jan 17, 2016
 *      Author: littlelion
 */

#ifndef SRC_MIPMAP_H_
#define SRC_MIPMAP_H_

#include <GL/glu.h>
#include <string>

namespace std {

class MipMap {
public:
	MipMap();
	virtual ~MipMap();
	void loadPNG(const string fileName);
	void loadSGI(const string fileName);
	void load(const string fileName);

private:
	GLuint texture = 0;
	size_t width = 0;
	size_t height = 0;
};

} /* namespace std */

#endif /* SRC_MIPMAP_H_ */
