/*
 * Mass.h
 *
 * 	Created on: Jan 23, 2016
 *  Based on the Spring class used during the winter term 2015/16
 *  in the physically based simulations course.
 */

#ifndef __MASS_H__
#define __MASS_H__

#include <GL/gl.h>
#include <Eigen/Dense>
#include <vector>

using std::vector;

/**
 * This class contains all properties of a mass-point which are necessary to run
 * a physically based simulation of a mass-spring system.
 */
class Mass {

public:
	Mass(std::vector<GLfloat*> posPtr = vector<GLfloat*>(),
			double mass = 0.0, double damp = 0.0);
	Mass(double mass, double damp = 0.0);
	Mass(const Mass &m);
	Mass(Mass &&);

	virtual ~Mass();

	void registerVertex(GLfloat* ptr);
	void setPos(Eigen::Vector3d p);
	Eigen::Vector3d getPos();
	void setX(double x);
	void setY(double y);
	void setZ(double z);

	void setVel(Eigen::Vector3d v);
	Eigen::Vector3d getVel();

	void setForce(Eigen::Vector3d f);
	Eigen::Vector3d getForce();

	void addForce(Eigen::Vector3d f);

	void setUserForce(Eigen::Vector3d f);
	Eigen::Vector3d getUserForce();

	double mass;
	double damping;
	bool fixed; /* True, if point is fixed in space */

private:
	vector<GLfloat*> posPtr;  /* Position of mass point - this vector contains pointers to all
	                              vertices with shared position */
	Eigen::Vector3d velocity;  /* Velocity of mass point */
	Eigen::Vector3d force;     /* Sum of all forces on mass point */
	Eigen::Vector3d userForce; /* Additional external force exerted by user */
};

#endif // __MASS_H__
