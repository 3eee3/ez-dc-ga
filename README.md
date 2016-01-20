# ez-dc-ga

This is a small project during a pro-semonar. It models a piece of cloth in OpenGL. The main goal is to simulate physical phenomena like gravity, friction, object collission and so on.

## Dependencies
Install following libraries / packages:
* OpenGL and utility libraries (package libglu***-dev e.g. libglu-mesa-dev; depends on graphics hardware)
* OpenGL utilities toolkit (package freeglut3-dev)
* libPNG  -- PNG file handling (package libpng12-dev)
* Eigen -- math utilities for vector and matrix arithmetics (package libeigen3-dev)

## Build the code:
The project contains some sub-projects, each with its own makefile  
Go to the subfolder _build_ of a project foder and run __make all__.
***
If you want compile directly on the console use the following options:  
-std=c++11 or -std=c++0x (use the C++ 11 dialect)  
-lGL (the OpenGL library)  
-lGLU (OpenGL utility library)
-lglut (the GL utility Toolkit - make sure 'freeglut' is installed, some extensions not present in the GLUT API are used)  
-lpthread (or -lthread; manage threads)  
-lpng (library to handle PNG files)

## License
... to be decided soon.

