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
#include "Mass.h"
#include "Scene.h"
#include "Simulation.h"

namespace std {

/* function declarations (aigner, including body) */
void fwd_euler(double dt, vector<Mass> &points, vector<Spring> &springs,
		bool interaction);
Eigen::Vector3d f_int(Mass &pt, vector<Spring> &springs);
Eigen::Vector3d gravity();       // gravity acceleration downwards

/* function declarations (zischg, including body) */
void symplectic(double dt, vector<Mass>& points, vector<Spring>& springs,
		bool interaction);
void leapfrog(double dt, vector<Mass>& points, vector<Spring>& springs,
		bool interaction);
bool leapFrogInitialized = false;
double t = 0;

/* function declarations (chea, including body, with help of aigner) */
void midpoint(double dt, vector<Mass>& points, vector<Spring>& springs,
		bool interaction);

/* universal applied constants (aigner) */
double floorLevel = -1.;            // ground level
double repulsiveSpringConst = -50.; // spring-constant effective if point hits the ground level

#ifdef _DEBUG /* (aigner) */

/*
 * print some data for debugging: summary of all points and springs
 */
void printData(vector<Mass>& points, vector<Spring>& springs) {
	//cout << "Points size: " << points.size();
	for (int i = 0; i < (int) points.size(); i++) {
		cout << "Point: X=(" << points[i].getX() << ", " << points[i].getY() << ", " << points[i].getZ();
		cout << "), F=(" << points[i].getForceX() << ", " << points[i].getForceY() << ", " << points[i].getForceZ();
		cout << "), f_u=(" << points[i].getUserForce().x() << ", " << points[i].getUserForce().y() << ", " << points[i].getUserForce().z();
		cout << "), V=(" << points[i].getVelX() << ", " << points[i].getVelY() << ", " << points[i].getVelZ();
		cout << "), m=" << points[i].mass << ", d=" << points[i].damping;
		cout << ", g=(" << gravity().x() << ", " << gravity().y() << ", " << gravity().z() << ")";
		cout << endl;
	}
	/*
	for (vector<Spring>::iterator it = springs.begin(); it != springs.end();
			++it) {
		cout << "Spring: k=" << it->stiffness << ", l="
		<< it->restLength << endl;
	}*/
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
void Simulation::step(double dt, Method method, std::vector<Mass> &points,
		std::vector<Spring> &springs) {

	switch (method) {
	case EULER: {
		fwd_euler(dt, points, springs, true);
		break;
	}

	case SYMPLECTIC: {
		symplectic(dt, points, springs, true);
		break;
	}

	case LEAPFROG: {
		leapfrog(dt, points, springs, true);
		break;
	}

	case MIDPOINT: {
		midpoint(dt, points, springs, true);
		break;
	}
	}

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
void fwd_euler(double dt, vector<Mass> &points, vector<Spring> &springs,
		bool interaction) {
	// preserve old values
	vector<Mass> oldPoints(points.begin(), points.end());

	// calculate internal forces - do it in advance with unchanged position
	// F_int + F_user + F_repulsive; gravity added as acceleration later
	for (vector<Mass>::iterator it = points.begin(); it != points.end(); ++it) {
		it->setForce(
				f_int(*it, springs)
						+ (it->getUserForce() * (interaction ? 1. : 0.)));
	}
	// update pos, vel of points using the old values
	vector<Mass>::iterator old = oldPoints.begin();
	for (vector<Mass>::iterator it = points.begin();
			it != points.end() && old != oldPoints.end(); ++it, ++old) {
		if (!it->fixed) {
			it->setPos(Eigen::Vector3d(old->getPos() + dt * old->getVel()));
		}
		Eigen::Vector3d acc = (1 / old->mass)
				* (old->getForce() - old->damping * old->getVel())
				+ gravity();
		it->setVel(Eigen::Vector3d(old->getVel() + (dt * acc)));
	}
}

/**
 * compute internal forces of point
 * @param pt       the actual point
 * @param springs  vector of springs
 * @return         sum of forces of all springs connected to point pt
 */
Eigen::Vector3d f_int(Mass &pt, vector<Spring> &springs) {
	Eigen::Vector3d f = Eigen::Vector3d();
	bool pt0;
	for (vector<Spring>::iterator it = springs.begin(); it != springs.end();
			++it) {
		if ((pt0 = (it->getMass(0) == &pt)) || it->getMass(1) == &pt) {
			Eigen::Vector3d diff = it->getMass(1)->getPos()
					- it->getMass(0)->getPos();
			f += diff.normalized()
					* ((it->restLength - diff.norm()) * it->stiffness
							* (pt0 ? -1 : 1));
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
void symplectic(double dt, vector<Mass>& points, vector<Spring>& springs,
		bool interaction) {
	/* Reuses new position for velocity computation */
	for (int i = 0; i < (int) points.size(); i++) {
		if (!points[i].fixed) {
			/*Update position*/
			points[i].setPos(points[i].getPos() + points[i].getVel() * dt);
		}
		/*Compute force due to gravity (initialize force with (0,0) if no gravity)*/
		points[i].setForce(gravity() * points[i].mass);
		/*Compute penalty force if mass hits the ground */
		/*Add user force*/
		points[i].setForce(
				Eigen::Vector3d(
						points[i].getForce()
								+ (interaction ? 1.0 : 0.0)
										* points[i].getUserForce()));
	}

	for (int j = 0; j < (int) springs.size(); j++) {
		/*Add internal forces to point force*/
		Mass* p1 = springs[j].getMass(0);
		Mass* p2 = springs[j].getMass(1);
		double k = springs[j].stiffness;
		double L = springs[j].restLength;
		Eigen::Vector3d direction = p1->getPos() - p2->getPos();
		Eigen::Vector3d internalF = direction.normalized() * k
				* (L - direction.norm());
		p1->setForce(p1->getForce() + internalF);
		p2->setForce(p2->getForce() - internalF);
	}

	for (int i = 0; i < (int) points.size(); i++) {
		if (!points[i].fixed) {
			/*Compute acceleration*/
			Eigen::Vector3d acceleration = (points[i].getForce()
					- points[i].getVel() * points[i].damping)
					/ points[i].mass;
			/*Update velocity*/
			points[i].setVel(points[i].getVel() + acceleration * dt);
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
void leapfrog(double dt, vector<Mass>& points, vector<Spring>& springs,
		bool interaction) {
	for (int i = 0; i < (int) (points.size()); i++) {
		/*Compute force due to gravity (initialize force with (0,0) if no gravity)*/
		points[i].setForce(gravity() * points[i].mass);
		/*Add penalty force and user force*/
		points[i].setForce(
				Eigen::Vector3d(
						points[i].getForce()
								+ (interaction ? 1.0 : 0.0)
										* points[i].getUserForce()));
	}
	for (int j = 0; j < (int) (springs.size()); j++) {
		/*Add internal forces*/
		Mass* p1 = springs[j].getMass(0);
		Mass* p2 = springs[j].getMass(1);
		double k = springs[j].stiffness;
		double L = springs[j].restLength;
		Eigen::Vector3d direction = p1->getPos() - p2->getPos();
		Eigen::Vector3d internalF = direction.normalized() * k
				* (L - direction.norm());
		p1->setForce(p1->getForce() + internalF);
		p2->setForce(p2->getForce() - internalF);
	}
	for (int i = 0; i < (int) (points.size()); i++) {
		if (!points[i].fixed) {
			/*Compute acceleration*/
			Eigen::Vector3d acceleration = (points[i].getForce()
					- points[i].getVel() * points[i].damping)
					/ points[i].mass;
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
void force(vector<Mass>& points, vector<Spring>& springs, bool interaction) {
	for (int i = 0; i < (int) ((points.size())); i++) {
		points[i].setForce(gravity() * points[i].mass);
		// Add penalty force and user force
		points[i].setForce(
				Eigen::Vector3d(
						points[i].getForce()
								+ (interaction ? 1.0 : 0.0)
										* points[i].getUserForce()));
	}
	for (int i = 0; i < (int) ((springs.size())); i++) {
		Eigen::Vector3d direction = springs[i].getMass(0)->getPos()
				- springs[i].getMass(1)->getPos();
		// internal force
		Eigen::Vector3d f = direction.normalized() * springs[i].stiffness
				* (springs[i].restLength - direction.norm());
		//Add internal forces
		springs[i].getMass(0)->setForce(springs[i].getMass(0)->getForce() + f);
		springs[i].getMass(1)->setForce(springs[i].getMass(1)->getForce() - f);
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
void midpoint(double dt, vector<Mass>& points, vector<Spring>& springs,
		bool interaction) {

	// preserve old values
	vector<Mass> oldPoints(points.begin(), points.end());
	double h = dt / 2.;

	for (int j = 0; j < 2; ++j) { // two half time-steps
		// update f(t), f(t+h/2)
		force(points, springs, interaction);

		for (int i = 0; i < (int) (points.size()); i++) {
			if (!points[i].fixed) {
				Eigen::Vector3d acc = (points[i].getForce()
						- points[i].damping * points[i].getVel())
						/ points[i].mass;

				// Compute position x(t+h/2), x(t+h)
				// this must be updated before v(...)
				points[i].setPos(
						Eigen::Vector3d(
								oldPoints[i].getPos()
										+ h * points[i].getVel()));

				// Compute position v(t+h/2), v(t+h)
				points[i].setVel(
						Eigen::Vector3d(oldPoints[i].getVel() + h * acc));
			}
		}
		h = dt;
	}
}

Eigen::Vector3d gravity() {
	return Eigen::Vector3d(0.0, -9.81, 0.0);
}

}
 // namespace std
