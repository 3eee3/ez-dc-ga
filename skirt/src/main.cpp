/*
 * main.cpp
 *
 *  Created on: Dec 28, 2015
 *      Author: littlelion
 */
#include <GL/freeglut.h>
#include <cstdlib>
#include <ctime>
#ifndef _WIN32
#include <sys/time.h>
#include <sys/times.h>
#else
//FIXME include time??? for windows
#endif
#include <iostream>
#include <cmath>

#ifndef _WIN32
#include "Accelerometer.h"
#endif
#include "Scene.h"
#include "MipMap.h"
#include "model_mapping.h"

using namespace std;

/* Simulation scene */
Scene *scene=NULL;

/*
 * Maximum time allowed between updates when running in real-time
 * mode (prevents performing too many calculations when running
 * slower than real time)
 */
const static double MAX_UPDATE_TIME = 0.01;

//FIXME doesn't work with windows OS
ulong getTime() {
#ifndef _WIN32
	struct timeval t;
	gettimeofday(&t, nullptr);
	return (ulong) (t.tv_sec*1000 + t.tv_usec);
#else
	return 0ul;//FIXME win os
#endif
}

void initialize() {
	//TODO implementation incomplete

	glClearColor(0.95f, 0.95f, 0.95f, 0.0f); // Background color

	// size of 3D space and camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set up shader and blender
	glFrontFace(GL_CCW);
//	glShadeModel(GL_FLAT); //TODO. use GL_SMOOTH ?
	glShadeModel(GL_SMOOTH);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glEnable(GL_CULL_FACE);//TODO. switch ON/OFF backside with these two lines
//	glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);


//	glEnable(GL_AUTO_NORMAL);
//	glEnable(GL_NORMALIZE);

	GLfloat spotdir[] = {1.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotdir);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
    GLfloat lposition[] = {2.0f, 2.0f, 2.0f, 0.0f};
//    GLfloat lposition[] = {9.0f, 9.0f, 9.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lposition);
    GLfloat lambient[] = {0.6f, 0.6f, 0.6f, 0.0f};
//    GLfloat lambient[] = {9.0f, 9.0f, 9.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lambient);
    GLfloat lspecular[] = {1.0f, 1.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, lspecular);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    MipMap texture;
//    texture.loadPNG("textures/cube.png");//TODO. select texture
//    texture.loadSGI("textures/wood.rgb");
//    texture.loadSGI("textures/red_cloth_256.rgb");
    texture.loadSGI(model3dTextureFilePath);

	scene->initialize();
	glEnable(GL_TEXTURE_2D);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//TODO. select < wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//TODO. select < solid
}

void motionCallback(int /*x*/, int /*y*/) {
	//TODO implement
}

void reshapeCallback(int w, int h) {
	//TODO implement
	glViewport(0, 0, w, h);

	glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   /* perspective projection */
   glFrustum (-3.0, 3.0, -3.0, 3.0, 7.5, 15.0);
   glMatrixMode (GL_MODELVIEW);
}

void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//TODO implement
	ulong curTime = getTime();
	static ulong prevTime = curTime;
	double remTime = 0;
	double tPassed = (double)(curTime - prevTime) / 1000.0;
	if (tPassed > MAX_UPDATE_TIME) {
		tPassed = MAX_UPDATE_TIME;
	}
	tPassed += remTime;

	ulong steps = (ulong)(tPassed / scene->getStep()); //round down

	// update simulation, then render
	for(ulong i=0; i<steps; ++i) {
		scene->update();
	}
	prevTime = curTime;
	remTime = fmod(tPassed, scene->getStep());

	scene->render();

	glutPostRedisplay();
	glutSwapBuffers();
}

void keyboardCallback(unsigned char key, int /*x*/, int /*y*/) {
	switch (key) {
	//TODO add keys for unimplemented features
	case 'q':
	case 27: // ESC
		// exit - assure to run cleanup code (not standard GLUT API)
		glutLeaveMainLoop();
		break;
	default:
		// do nothing
		break;
	}
}

void mouseCallback(int button, int state, int x, int y) {
	(void)button;
	(void)state;
	(void)x;
	(void)y;
	//TODO implement
}

void idleCallback() {
	//TODO implement
	glutPostRedisplay();
}

int main (int argc, char* argv[]) {

	glutInit(&argc, argv);
#ifndef _WIN32
	Accelerometer::accelerometerInitDevice(argc, argv);
#endif
	scene = new Scene(argc, argv);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Cloth simulation");

	initialize();

	glutMotionFunc(motionCallback);
	glutReshapeFunc(reshapeCallback);
	glutDisplayFunc(displayCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutIdleFunc(idleCallback);

	glutMainLoop();

	delete scene;
	// space for cleanup code

	return EXIT_SUCCESS;
}



