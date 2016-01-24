/******************************************************************
*
* Mass.cpp
*
* Description: Implementation of functions for handling mass Masss  
*
* Physically-Based Simulation Proseminar WS 2015
* 
* Interactive Graphics and Simulation Group
* Institute of Computer Science
* University of Innsbruck
*
*******************************************************************/


#include "Mass.h"
#include <GL/freeglut.h> 

void Mass::setPos(Eigen::Vector3d p)
{
    pos = p;
}
 
Eigen::Vector3d Mass::getPos()
{
    return pos;
}

void Mass::setX(double x)
{
    pos[0]=x;
}

double Mass::getX()
{
    return pos[0] ;
}

void Mass::setY(double y)
{
    pos[1] = y;
}

void Mass::setZ(double z)
{
	pos[2] = z;
}

double Mass::getY()
{
    return pos[1];
}

double Mass::getZ()
{
    return pos[2];
}

void Mass::setVel(Eigen::Vector3d v)
{
    vel = v;
}
 
Eigen::Vector3d Mass::getVel()
{
    return vel;
}

void Mass::setVelX(double vx)
{
    vel[0] = vx;
}

double Mass::getVelX()
{
    return vel[0] ;
}

void Mass::setVelY(double vy)
{
    vel[1] = vy;
}

double Mass::getVelY()
{
    return vel[1];
}

double Mass::getVelZ()
{
    return vel[2];
}

void Mass::setForce(Eigen::Vector3d f)
{
    force = f;
}
 
Eigen::Vector3d Mass::getForce()
{
    return force;
}

void Mass::setForceX(double fx)
{
    force[0] = fx;
}

double Mass::getForceX()
{
    return force[0] ;
}

void Mass::setForceY(double fy)
{
   force[1] = fy;
}

double Mass::getForceY()
{
    return force[1];
}

double Mass::getForceZ()
{
    return force[2];
}

void Mass::addForce(Eigen::Vector3d f)
{
    force = force + f;
}

void Mass::setMass(double m)
{
    mass = m;
}

double Mass::getMass()
{
    return mass;
}

void Mass::setDamping(double d)
{
    damping = d;
}

double Mass::getDamping()
{
    return damping;
}

void Mass::setFixed(bool fix)
{
    fixed = fix;
}

bool Mass::isFixed()
{
    return fixed;
}

void Mass::setUserForce(Eigen::Vector3d f)
{
    userForce = f;
}
 
Eigen::Vector3d Mass::getUserForce()
{
    return userForce;
}
