/*
 * main.cpp
 *
 *  Created on: Dec 28, 2015
 *      Author: littlelion
 */
#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>
using namespace std;

void Initialize() {
	//TODO implementation incomplete

	glClearColor(0.95, 0.95, 0.95, 0.0); // Background color

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MotionCallback(int x, int y) {
	//TODO implement
}

void ReshapeCallback(int w, int h) {
	//TODO implement
}

void DisplayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TODO implement

	glutPostRedisplay();
	glutSwapBuffers();
}

void KeyboardCallback(unsigned char key, int x, int y) {
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

void MouseCallback(int button, int state, int x, int y) {
	//TODO implement
}

void IdleCallback() {
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

	Initialize();

	glutMotionFunc(MotionCallback);
	glutReshapeFunc(ReshapeCallback);
	glutDisplayFunc(DisplayCallback);
	glutKeyboardFunc(KeyboardCallback);
	glutMouseFunc(MouseCallback);
	glutIdleFunc(IdleCallback);

	glutMainLoop();

	// space for cleanup code

	return EXIT_SUCCESS;
}



