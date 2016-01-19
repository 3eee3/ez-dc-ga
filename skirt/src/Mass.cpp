/******************************************************************
*
* Mass.cpp
*
* This is a derived work from Point.cpp
*
* Description: Implementation of functions for handling mass points  
*
* Physically-Based Simulation Proseminar WS 2015
* 
* Interactive Graphics and Simulation Group
* Institute of Computer Science
* University of Innsbruck
*
*******************************************************************/

#include <GL/freeglut.h>

#include "Mass.h"

void Mass::render()
{
    /* Fixed vertices displayed in blue, free in red */
    if (fixed)
        glColor3f(0.0, 0.0, 1.0);
    else
        glColor3f(1.0, 0.0, 0.0);
    
    glTranslatef((GLfloat)pos.x(), (GLfloat)pos.y(), (GLfloat)pos.z());

    /* Draw unshaded spheres; appear as filled circles */
    glutSolidSphere(0.1, 36, 36);
    glLoadIdentity();   
}

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
    pos.x() = x;
}

double Mass::getX()
{
    return pos.x() ;
}

void Mass::setY(double y)
{
    pos.y() = y;
}

double Mass::getY()
{
    return pos.y();
}

void Mass::setZ(double z)
{
    pos.z() = z;
}

double Mass::getZ()
{
    return pos.z();
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
    vel.x() = vx;
}

double Mass::getVelX()
{
    return vel.x() ;
}

void Mass::setVelY(double vy)
{
    vel.y() = vy;
}

double Mass::getVelY()
{
    return vel.y();
}

void Mass::setVelZ(double vz)
{
    vel.z() = vz;
}

double Mass::getVelZ()
{
    return vel.z();
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
    force.x() = fx;
}

double Mass::getForceX()
{
    return force.x() ;
}

void Mass::setForceY(double fy)
{
    force.y() = fy;
}

double Mass::getForceY()
{
    return force.y();
}

void Mass::setForceZ(double fz)
{
    force.z() = fz;
}

double Mass::getForceZ()
{
    return force.z();
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
