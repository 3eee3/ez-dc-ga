/******************************************************************
*
* Exercise.cpp  
*
* Description: In this file - in the function TimeStep() - the various 
* numerical solvers of the first programming assignment have to be
* implemented. Feel free to add new local functions into this file. 
* Changes to other source files of the framework should not be 
* required. It is acceptable to assume knowledge about the scene
* topology (i.e. how springs and points are connected).
*
* Physically-Based Simulation Proseminar WS 2015
* 
* Interactive Graphics and Simulation Group
* Institute of Computer Science
* University of Innsbruck
*
*******************************************************************/
/* uncomment following defines for additional output */
//#define _DEBUG
//#define _WRITE_FILE "results.csv"

/* Standard includes */
#include <iostream>
#include <vector>
#include <stdio.h>
#include <Eigen/Dense>

/* Local includes */
#include "Point.h"
#include "Scene.h"
#include "Accelerometer.h"
#include "Simulation.h"

namespace std {
//namespace Eigen {

/* function declarations (aigner, including body) */
void fwd_euler(double dt, vector<Point> &points, vector<Spring> &springs, bool interaction);
Vector3d f_int(Point &pt, vector<Spring> &springs);
Vector3d gravity();       // gravity acceleration downwards

/* function declarations (zischg, including body) */
#ifdef _WRITE_FILE
void writeToFile(double xt_,double vt_,Scene::Method method,double dt,vector<Point> &points, vector<Spring>& springs);
#endif
void symplectic(double dt, vector<Point>& points, vector<Spring>& springs, bool interaction);
void leapfrog(double dt, vector<Point>& points, vector<Spring>& springs, bool interaction);
bool leapFrogInitialized=false;
double t=0;
void analytical_spring(double dt, vector<Point>& points, vector<Spring>& springs);


/* function declarations (chea, including body, with help of aigner) */
void midpoint(double dt, vector<Point>& points, vector<Spring>& springs, bool interaction);

/* universal applied constants (aigner) */
double floorLevel = -1.;            // ground level
double repulsiveSpringConst = -50.; // spring-constant effective if point hits the ground level

#ifdef _DEBUG /* (aigner) */

/*
 * print some data for debugging: summary of all points and springs
 */
void printData(vector<Point>& points, vector<Spring>& springs) {
	for (vector<Point>::iterator it = points.begin(); it != points.end();
			++it) {
		cout << "Point: X=(" << it->getX() << ", " << it->getY() << ", " << it->getZ();
		cout << "), F=(" << it->getForceX() << ", " << it->getForceY() << ", " << it->getForceZ();
		cout << "), f_u=(" << it->getUserForce().x() << ", " << it->getUserForce().y() << ", " << it->getUserForce().z();
		cout << "), V=(" << it->getVelX() << ", " << it->getVelY() << ", " << it->getVelZ();
		cout << "), m=" << it->getMass() << ", d=" << it->getDamping();
		cout << ", g=(" << gravity().x() << ", " << gravity().y()  << ", " << gravity().z() << ")";
		cout << endl;
	}
	for (vector<Spring>::iterator it = springs.begin(); it != springs.end();
			++it) {
		cout << "Spring: k=" << it->getStiffness() << ", l="
				<< it->getRestLength() << endl;
	}
}
#endif

/* *****************************************************************
*
* TimeStep
*
* This function is called every time step of the dynamic simulation.
* Positions, velocities, etc. should be updated to simulate motion
* of the mass points of the 2D scene. The selected solver is passed
* to the function as well as the time step, the spings, and the 
* mass points. 
*
*******************************************************************/
void timeStep(double dt, Method method,
              vector<Point> &points, vector<Spring> &springs, bool interaction)
{ 


	switch (method)
    {
    case ANALYTICAL:
		{
			/*Analytic solution of single hanging mass*/
			analytical_spring(dt, points, springs);
			break;
		}

        case EULER:
        {
			fwd_euler(dt, points, springs, interaction);
        	break;
        }
        
        case SYMPLECTIC:
        {
        	symplectic(dt, points, springs, interaction);
        	break;
        }

        case LEAPFROG:
        {
        	leapfrog(dt, points, springs, interaction);
            break;
        }

        case MIDPOINT:
        {
        	midpoint(dt, points, springs, interaction);
            break;  
        }            
    }

#ifdef _WRITE_FILE
	writeToFile(points[1].getY(),points[1].getVelY(),method,dt,points,springs);
#endif

#ifdef _DEBUG
    printData(points, springs);
#endif
}       

/**
 * Euler Method - Implementation
 * This function calculates the position and velocity for the next timestep.
 * # Compute position at t+h      x(t+h) = x(t) + h*v(t)
 * # Compute forces at t          f(t)   = f_int(t) + f_ext(t)
 * # Compute acceleration at t    a(t)   = 1/m (f(t) - gamma*v(t))
 * # Compute velocity at t+h      v(t+h) = v(t) + h*a(t)
 *
 * @param dt            a time step
 * @param points        vector of points
 * @param springs       vector of springs
 * @param interaction   switch user-force on or off
 */
void fwd_euler(double dt, vector<Point> &points, vector<Spring> &springs, bool interaction)
{
	// preserve old values
	vector<Point> oldPoints(points.begin(), points.end());

	// calculate internal forces - do it in advance with unchanged position
	// F_int + F_user + F_repulsive; gravity added as acceleration later
	for(vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
		it->setForce(f_int(*it, springs) + (it->getUserForce() * (interaction ? 1. : 0.)) +
				Vector3d(0., it->getY() < floorLevel ? (it->getY() - floorLevel) * repulsiveSpringConst : 0., 0.));
	}
	// update pos, vel of points using the old values
	vector<Point>::iterator old = oldPoints.begin();
	for(vector<Point>::iterator it = points.begin(); it != points.end() && old != oldPoints.end(); ++it,++old) {
		if (!it->isFixed()) {
			it->setPos(Vector3d(old->getPos() + dt* old->getVel()));
		}
		Vector3d acc = (1 / old->getMass()) * (old->getForce() - old->getDamping()*old->getVel()) + gravity();
	    it->setVel(Vector3d(old->getVel() + (dt * acc)));
	}
}

/**
 * compute internal forces of point
 * @param pt       the actual point
 * @param springs  vector of springs
 * @return         sum of forces of all springs connected to point pt
 */
Vector3d f_int(Point &pt, vector<Spring> &springs) {
	Vector3d f = Vector3d();
	bool pt0;
	for(vector<Spring>::iterator it = springs.begin(); it != springs.end(); ++it) {
		if((pt0 = (it->getPoint(0) == &pt)) || it->getPoint(1) == &pt) {
			Vector3d diff = it->getPoint(1)->getPos() - it->getPoint(0)->getPos();
			f += diff.normalized() *
					((it->getRestLength() - diff.norm()) *
							it->getStiffness() * (pt0 ? -1 : 1));
		}
	}
	return f;
}

/**
 * Symplectic euler method
 * # Compute position at t+h      x(t+h) = x(t) + h*v(t)
 * # Compute forces at t          f(t)   = f_int(t) + f_ext(t)
 * # Compute acceleration at t    a(t)   = 1/m (f(t) - gamma*v(t))
 * # Compute velocity at t+h      v(t+h) = v(t) + h*a(t)
 * @param dt
 * @param points
 * @param springs
 * @param interaction
 */
void symplectic(double dt, vector<Point>& points, vector<Spring>& springs,
		bool interaction) {
	/* Reuses new position for velocity computation */
	for (int i=0; i<(int)points.size(); i++){
		if(!points[i].isFixed()){
			/*Update position*/
			points[i].setPos(points[i].getPos()+points[i].getVel()*dt);
		}
		/*Compute force due to gravity (initialize force with (0,0) if no gravity)*/
		points[i].setForce(gravity() * points[i].getMass());
		/*Compute penalty force if mass hits the ground */
		Vector3d penaltyF = Vector3d (0.0,(points[i].getY()<floorLevel) ? repulsiveSpringConst * (floorLevel+points[i].getY()):0.0, 0.0);
		/*Add penalty force and user force*/
		points[i].setForce(Vector3d(points[i].getForce()+penaltyF+(interaction ? 1.0 : 0.0)*points[i].getUserForce()));
	}

	for (int j=0;j<(int)springs.size(); j++){
		/*Add internal forces to point force*/
		Point* p1 = springs[j].getPoint(0);
		Point* p2 = springs[j].getPoint(1);
		double k = springs[j].getStiffness();
		double L = springs[j].getRestLength();
		Vector3d direction = p1->getPos()-p2->getPos();
		Vector3d internalF = direction.normalized()*k*(L-direction.norm());
		p1->setForce(p1->getForce()+internalF);
		p2->setForce(p2->getForce()-internalF);
	}

	for (int i=0; i<(int)points.size(); i++){
		if(!points[i].isFixed()){
			/*Compute acceleration*/
			Vector3d acceleration = (points[i].getForce()-points[i].getVel()*points[i].getDamping())/points[i].getMass();
			/*Update velocity*/
			points[i].setVel(points[i].getVel()+acceleration*dt);
		}
	}
}

/**
 * leapfrog method
 * @param dt
 * @param points
 * @param springs
 * @param interaction
 */
void leapfrog(double dt, vector<Point>& points, vector<Spring>& springs,
		bool interaction) {
	for (int i = 0; i < (int) (points.size()); i++) {
		/*Compute force due to gravity (initialize force with (0,0) if no gravity)*/
		points[i].setForce(gravity() * points[i].getMass());
		/*Compute penalty force if mass hits the ground */
		Vector3d penaltyF = Vector3d(0.0,
				(points[i].getY() < floorLevel) ?
						repulsiveSpringConst * (floorLevel + points[i].getY()) : 0.0, 0.0);
		/*Add penalty force and user force*/
		points[i].setForce(
				Vector3d(points[i].getForce() + penaltyF
						+ (interaction ? 1.0 : 0.0) * points[i].getUserForce()));
	}
	for (int j = 0; j < (int) (springs.size()); j++) {
		/*Add internal forces*/
		Point* p1 = springs[j].getPoint(0);
		Point* p2 = springs[j].getPoint(1);
		double k = springs[j].getStiffness();
		double L = springs[j].getRestLength();
		Vector3d direction = p1->getPos() - p2->getPos();
		Vector3d internalF = direction.normalized() * k * (L - direction.norm());
		p1->setForce(p1->getForce() + internalF);
		p2->setForce(p2->getForce() - internalF);
	}
	for (int i = 0; i < (int) (points.size()); i++) {
		if (!points[i].isFixed()) {
			/*Compute acceleration*/
			Vector3d acceleration = (points[i].getForce()
					- points[i].getVel() * points[i].getDamping())
					/ points[i].getMass();
			/*Update velocity v(t+dt/2)*/
			if (leapFrogInitialized) {
				points[i].setVel(points[i].getVel() + acceleration * dt);
			} else {
				//Initialization of leapfrog method
				points[i].setVel(points[i].getVel() + acceleration * dt / 2);
				leapFrogInitialized = true;
			}
			/*Update position using v(t+dt/2)*/
			points[i].setPos(points[i].getPos() + points[i].getVel() * dt);
		}
	}
}

/**
 * calculate force for all points at time t according to x(t) stored in the points.
 * @param points
 * @param springs
 * @param interaction
 */
void force(vector<Point>& points, vector<Spring>& springs, bool interaction) {
	for (int i = 0; i < (int) ((points.size())); i++) {
		points[i].setForce(gravity() * points[i].getMass());
		//penalty force
		Vector3d f = Vector3d(0.0,
				(points[i].getY() < floorLevel) ?
						repulsiveSpringConst * (points[i].getY() - floorLevel) :
						0.0, 0.0);
		// Add penalty force and user force
		points[i].setForce(
				Vector3d(points[i].getForce() + f
						+ (interaction ? 1.0 : 0.0) * points[i].getUserForce()));
	}
	for (int i = 0; i < (int) ((springs.size())); i++) {
		Vector3d direction = springs[i].getPoint(0)->getPos()
				- springs[i].getPoint(1)->getPos();
		// internal force
		Vector3d f = direction.normalized() * springs[i].getStiffness()
				* (springs[i].getRestLength() - direction.norm());
		//Add internal forces
		springs[i].getPoint(0)->setForce(
				springs[i].getPoint(0)->getForce() + f);
		springs[i].getPoint(1)->setForce(
				springs[i].getPoint(1)->getForce() - f);
	}
}

/**
 * midpoint method
 * formula:
 *   a(t) = (f(t) - gamma*v(t))/m
 *   v_half(t+h/2) = v(t) + h/2*a(t)
 *   x_half(t+h/2) = x(t) + h/2*v(t)
 *   a_half(t+h/2) = (f_half(t+h/2) - gamma*v_half(t+h/2))/m
 *   x(t+h) = x(t) + h*v_half(t+h/2)
 *   v(t+h) = v(t) + h*a_half(t+h/2)
 * @param dt
 * @param points
 * @param springs
 * @param interaction
 */
void midpoint(double dt, vector<Point>& points, vector<Spring>& springs,
		bool interaction) {

	// preserve old values
	vector<Point> oldPoints(points.begin(), points.end());
	double h = dt/2.;

	for (int j = 0; j < 2; ++j) { // two half time-steps
		// update f(t), f(t+h/2)
		force(points, springs, interaction);

		for (int i = 0; i < (int) (points.size()); i++) {
			if (!points[i].isFixed()) {
				Vector3d acc = (points[i].getForce()
						- points[i].getDamping() * points[i].getVel())
						/ points[i].getMass();

				// Compute position x(t+h/2), x(t+h)
				// this must be updated before v(...)
				points[i].setPos(Vector3d(oldPoints[i].getPos() + h * points[i].getVel()));

				// Compute position v(t+h/2), v(t+h)
				points[i].setVel(Vector3d(oldPoints[i].getVel() + h * acc));
			}
		}
		h = dt;
	}
}

///**
// * absolute value
// * @param x
// * @return |x|
// */
//double abs(double x) {
//	return x>=0. ? x : -x;
//}

/**
 * Analytical solution to vertical spring testcase.
 * @param dt
 * @param points
 * @param springs
 */
void analytical_spring(double dt, vector<Point>& points,
		vector<Spring>& springs) {
	/*Analytic solution of single hanging mass*/
	double xt, vt;
	t += dt;
	double m = points[1].getMass();
	double g = -gravity().y();
	double k = springs[0].getStiffness();
	double damp_crit = 2* sqrt(m*k);
	double w_r = points[1].getDamping() / (2 * m);
	double w_ = sqrt(abs(k / m - w_r * w_r));
	double A = m * g / k;
	double B = w_r * m * g / (k * w_);
	double ee = exp(-w_r * t);
	if (points[1].getDamping() < damp_crit) { // weak damped case
		xt = ee * (A * cos(w_ * t) + B * sin(w_ * t)) - A; //initial pos is 0 (point[0]=(0,1) point[1]=(0,0))
		vt = ee * (-A * w_ * sin(w_ * t) + B * w_ * cos(w_ * t))
						- w_r * ee * (A * cos(w_ * t) + B * sin(w_ * t));
	} else if (points[1].getDamping() > damp_crit) { // strong damped case
		xt = ee * (A * exp(w_ * t) + B * exp(-w_ * t)) - A; //initial pos is 0 (point[0]=(0,1) point[1]=(0,0))
		vt = ee * (A * w_ * exp(w_ * t) + B * -w_ * exp(-w_ * t))
						- w_r * ee * (A * exp(w_ * t) + B * exp(w_ * t));
	} else { // (points[1].getDamping() == damp_crit) // aperiodic case
		xt = ee * (A + B * t) - A; //initial pos is 0 (point[0]=(0,1) point[1]=(0,0))
		vt = ee * B	- w_r * ee * (A + B * t);
	}
	points[1].setPos(Vector3d(0.,xt,0.));
	points[1].setVel(Vector3d(0.,vt,0.));
}

Vector3d gravity() {
	static Accelerometer acc = Accelerometer();
	return Vector3d(-acc.getY(), -acc.getZ(), acc.getX());
}

#ifdef _WRITE_FILE
void writeToFile(double xt_,double vt_,Scene::Method method,double dt,vector<Point> &points,vector<Spring>& springs){
	static int compstep=0;
	static bool compfinished=false;
	static const int numberofsteps=10000;//10 second when dt=0.001
	static double xt[numberofsteps], vt[numberofsteps];
	static int compi=0;
	if(compi<=0) {
		fprintf (stdout, "collecting data ...\n");
	}
	compstep+=1;
	if(compfinished){
		return;
	}else if(compstep>numberofsteps){
		compfinished=true;
		//write to csv file
		int i;
		FILE * pFile;
		pFile = fopen (_WRITE_FILE, "wb");
		switch (method) {
			case Scene::ANALYTICAL:
			{
				fprintf(pFile,"scene,analytical\n");
				break;
			}
			case Scene::EULER:
			{
				fprintf(pFile,"scene,euler\n");
				break;
			}
			case Scene::SYMPLECTIC:
			{
				fprintf(pFile,"scene,symplectic\n");
				break;
			}
			case Scene::LEAPFROG:
			{
				fprintf(pFile,"scene,leapfrog\n");
				break;
			}
			case Scene::MIDPOINT:
			{
				fprintf(pFile,"scene,midpoint\n");
				break;
			}
		}
		fprintf(pFile,"step,%f\ndamping,%f\nmass,%f\nk spring,%f\n",dt,points[1].getDamping(),points[1].getMass(),springs[0].getStiffness());
		fprintf(pFile,"y(t),v_y(t)\n");
		for(i=0;i<compi;i++){
			fprintf(pFile,"%f,",xt[i]);
			fprintf(pFile,"%f,",vt[i]);
			fprintf(pFile,"\n");
		}
		fclose (pFile);
		fprintf(stdout,"Results written to file %s.\n", _WRITE_FILE);
	}else{
		xt[compi]=xt_;
		vt[compi]=vt_;
		compi++;
	}
}
#endif

//} // namespace Eigen
} // namespace std