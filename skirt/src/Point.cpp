/******************************************************************
*
* Point.cpp
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


#include "Point.h"
#include <GL/freeglut.h> 

void Point::render()
{
    /* Fixed vertices displayed in blue, free in red */
    if (fixed)
        glColor3f(0.0, 0.0, 1.0);
    else
        glColor3f(1.0, 0.0, 0.0);
    
    glTranslatef(pos.x(), pos.y(), pos.z());

    /* Draw unshaded spheres; appear as filled circles */
    glutSolidSphere(0.1, 36, 36);
    glLoadIdentity();   
}

void Point::setPos(Vector3d p)
{
    pos = p;
}
 
Vector3d Point::getPos()
{
    return pos;
}

void Point::setX(double x)
{
    pos.x() = x;
}

double Point::getX()
{
    return pos.x() ;
}

void Point::setY(double y)
{
    pos.y() = y;
}

double Point::getY()
{
    return pos.y();
}

void Point::setZ(double z)
{
    pos.z() = z;
}

double Point::getZ()
{
    return pos.z();
}

void Point::setVel(Vector3d v)
{
    vel = v;
}
 
Vector3d Point::getVel()
{
    return vel;
}

void Point::setVelX(double vx)
{
    vel.x() = vx;
}

double Point::getVelX()
{
    return vel.x() ;
}

void Point::setVelY(double vy)
{
    vel.y() = vy;
}

double Point::getVelY()
{
    return vel.y();
}

void Point::setVelZ(double vz)
{
    vel.z() = vz;
}

double Point::getVelZ()
{
    return vel.z();
}

void Point::setForce(Vector3d f)
{
    force = f;
}
 
Vector3d Point::getForce()
{
    return force;
}

void Point::setForceX(double fx)
{
    force.x() = fx;
}

double Point::getForceX()
{
    return force.x() ;
}

void Point::setForceY(double fy)
{
    force.y() = fy;
}

double Point::getForceY()
{
    return force.y();
}

void Point::setForceZ(double fz)
{
    force.z() = fz;
}

double Point::getForceZ()
{
    return force.z();
}

void Point::addForce(Vector3d f)
{
    force = force + f;
}

void Point::setMass(double m)
{
    mass = m;
}

double Point::getMass()
{
    return mass;
}

void Point::setDamping(double d)
{
    damping = d;
}

double Point::getDamping()
{
    return damping;
}

void Point::setFixed(bool fix)
{
    fixed = fix;
}

bool Point::isFixed()
{
    return fixed;
}

void Point::setUserForce(Vector3d f)
{
    userForce = f;
}
 
Vector3d Point::getUserForce()
{
    return userForce;
}
