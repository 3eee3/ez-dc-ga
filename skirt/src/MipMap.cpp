/*
 * Texture.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: littlelion
 */

#include "MipMap.h"

#include <GL/glu.h>
#include <png.h>
#include <iostream>

extern "C" {
#include "SGIimage.h"
}

namespace std {

MipMap::MipMap() {
	// TODO Auto-generated constructor stub
}

MipMap::~MipMap() {
	if (texture) {
		glDeleteTextures(1, &texture);
	}
}

/**
 * @brief load texture from PNG
 *
 * loads a png file into an opengl mipmap object, using cstdio , libpng, and opengl.
 * This function is almost identical with the example code on
 * https://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures
 *
 * \param filename : the png file to be loaded
 */
void MipMap::loadPNG(const string filename) {
	if (texture) {
		throw runtime_error("Texture::loadPNG: Invalid call to load function: texture already loaded.");
	}
	//header for testing if it is a png
	png_byte header[8];

	//open file as binary
	FILE *fp = fopen(filename.c_str(), "rb");
	if (!fp) {
		throw runtime_error("Texture::loadPNG: Opening file " + filename + " failed.");
	}

	//read the header
	if (!fread(header, 1, 8, fp)) {
		fclose(fp);
		throw runtime_error("Texture::loadPNG: Reading signature of file " + filename + " failed.");
	}

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) {
		fclose(fp);
		throw runtime_error("Texture::loadPNG: File " + filename + " is not a PNG image.");
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
	NULL, NULL);
	if (!png_ptr) {
		fclose(fp);
		throw runtime_error("Texture::loadPNG: Function png_create_read_struct() returned 0.");
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);
		throw runtime_error("Texture::loadPNG: Function png_create_info_struct() returned 0.");
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		throw runtime_error("Texture::loadPNG: Function png_create_info_struct() returned 0.");
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		throw runtime_error("Texture::loadPNG: libpng encountered an error.");
	}

	//init png reading
	png_init_io(png_ptr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
	NULL, NULL, NULL);

	//update width and height based on png info
	width = twidth;
	height = theight;

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// glTexImage2d requires rows to be 4-byte aligned
	rowbytes += 3 - ((rowbytes - 1) % 4);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data = new png_byte[rowbytes * height];
	if (!image_data) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		throw runtime_error("Texture::loadPNG: Could not allocate memory for PNG image data.");
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[height];
	if (!row_pointers) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		fclose(fp);
		throw runtime_error("Texture::loadPNG: Could not allocate memory for PNG row pointers.");
	}
	// set the individual row_pointers to point at the correct offsets of image_data
	for (size_t i = 0; i < height; ++i)
		row_pointers[height - 1 - i] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//Now generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
//	GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLenum gluerr;
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	if((gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, (GLsizei)width, (GLsizei)height, GL_RGB,
				   GL_UNSIGNED_BYTE, (GLvoid *)image_data))) {
		throw runtime_error("GLULib " + string((char*)gluErrorString(gluerr)));
	}
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] image_data;
	delete[] row_pointers;
	fclose(fp);
}

/**
 * @brief load texture from PNG
 *
 * loads a RGB file into an opengl mipmap object, using cstdio , libpng, and opengl.
 * This function is almost identical with the example code on
 * https://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures
 *
 * \param filename : the SGI-rgb file to be loaded
 */
void MipMap::loadSGI(const string filename) {
	if (texture) {
		throw runtime_error("Texture::loadPNG: Invalid call to load function: texture already loaded.");
	}
	IMAGE *img;
	GLenum gluerr;
	GLuint texName;

	glGenTextures(1,&texName);
	glBindTexture(GL_TEXTURE_2D,texName);
	if(!(img=ImageLoad((char*)filename.c_str()))) {
		throw runtime_error("Error reading the texture wood.rgb");
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	if((gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->sizeX, img->sizeY, GL_RGB,
				   GL_UNSIGNED_BYTE, (GLvoid *)(img->data)))) {
		throw runtime_error("GLULib " + string((char*)gluErrorString(gluerr)));
	}
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

} /* namespace std */
