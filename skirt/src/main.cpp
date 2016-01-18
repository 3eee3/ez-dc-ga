/*
 * main.cpp
 *
 *  Created on: Dec 28, 2015
 *      Author: littlelion
 */
#include <GL/freeglut.h>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <sys/times.h>
#include <iostream>
#include <cmath>

#include "Scene.h"

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
	struct timeval t;
	gettimeofday(&t, nullptr);
	return (ulong) (t.tv_sec*1000 + t.tv_usec);
}

void initialize() {
	//TODO implementation incomplete

	glClearColor(0.95, 0.95, 0.95, 0.0); // Background color

	// size of 3D space and camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set up shader and blender
	glShadeModel(GL_FLAT); //TODO. use GL_SMOOTH ?
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    GLfloat lp[] = {2.0, 2.0, 2.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lp);
    GLfloat la[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, la);

	scene->initialize();
}

void motionCallback(int x, int y) {
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
	static ulong remTime = 0;
	double tPassed = (curTime - prevTime) / 1000.0;
	if (tPassed > MAX_UPDATE_TIME) {
		tPassed = MAX_UPDATE_TIME;
	}
	tPassed += remTime;

	ulong steps = tPassed / scene->getStep();

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

void keyboardCallback(unsigned char key, int x, int y) {
	switch (key) {
	//TODO add keys for unimplemented features
	case 'q':
		// exit - assure to run cleanup code (not standard GLUT API)
		glutLeaveMainLoop();
		break;
	default:
		// do nothing
		break;
	}
}

void mouseCallback(int button, int state, int x, int y) {
	//TODO implement
}

void idleCallback() {
	//TODO implement
	glutPostRedisplay();
}

int main (int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Cloth simulation");

	//TODO implement 3D model and physics engine

	scene = new Scene();
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



