/* Standard includes */
#include <iostream>
#include <GL/glut.h> 

/* Local includes */
#include "Cloth3D.h"
#include "Object.h"

using namespace std;

/*----------------------------------------------------------------*/
bool pauseFlag = false;        /* Toggle simulation */
bool addSource = true;         /* Toggle density injection at fixed source */
bool addMouseSource = false;   /* Switch for adding density at mouse */

int width  = 600;
int height = 600;
double camera[] = {0.0, 1.0, 0.0, 1.0};

float xs = 0.0;    /* Coordinates of mouse for adding density */
float ys = 0.0;

Object* cube;
Cloth3D* cloth;

/******************************************************************
*
*******************************************************************/

void reshapeCallback(int w, int h)
{
    if (h == 0) h = 1;
    
    width = w;
    height = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(camera[0], camera[1], camera[2], camera[3], -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


/******************************************************************
*
*******************************************************************/

void displayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT);

	cloth->draw();
	//cloth->drawPoints();
	//cube->draw();
	//cube->drawTest();

    glutSwapBuffers();
}


/******************************************************************
*
*******************************************************************/

void keybCallback(unsigned char key, int x, int y)
{
    switch(key) 
    {
        case 'q':
            exit(0);
            break;

        case ' ': 
            pauseFlag = !pauseFlag;
            break;
    }

    glutPostRedisplay();
}


/******************************************************************
*
*******************************************************************/

void buttonCallback(int button, int state, int x, int y)
{
    /* Add density when mouse button down */
    if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
        addMouseSource = true;
    else
        addMouseSource = false; 

    glutPostRedisplay();
}


/******************************************************************
*
*******************************************************************/

void mouseCallback(int x, int y)
{
    /* Store mouse position for injecting density */
    xs = float(x) / width;
    ys = float(y) / height;
 
    if (xs < 0.0) xs = 0.01;
    if (xs > 1.0) xs = 0.99;

    if (ys < 0.0) ys = 0.01;
    if (ys > 1.0) ys = 0.99;

    glutPostRedisplay();
}


/******************************************************************
*
*******************************************************************/

void idleFunc()
{ 
    if (!pauseFlag)
    {
        //if (addSource) /* Add fixed density source */
            //fluid->addDensity(0.45, 0.55, 0.10, 0.15);

        //if (addMouseSource) /* Add density at mouse position on button press */
            //fluid->addDensity(xs-0.01, xs+0.01, 1.0-(ys+0.01), 1.0-(ys-0.01));

        /* Simulation time step */
        //fluid->step(); 
    }

    glutPostRedisplay();
}


/******************************************************************
*
*******************************************************************/

int main(int argc, char **argv)
{
    /* Default simulation resolution */
    int sim_res = 10;

	cloth = new Cloth3D(sim_res);
	cube = new Object();

    if(argc == 2)
        sim_res = atoi(argv[1]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);         
    glutCreateWindow("Cloth Simulation");

    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keybCallback);
	glutPassiveMotionFunc(mouseCallback);
	glutMotionFunc(mouseCallback);
    glutMouseFunc(buttonCallback);
    glutIdleFunc(idleFunc);  

    glClearColor(0.0, 0.0, 0.0, 1.0);
    //glShadeModel(GL_SMOOTH);
	glShadeModel(GL_FLAT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT, GL_FILL);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

    glutMainLoop();
    return 0;
}

