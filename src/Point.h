/******************************************************************
*
* Point.h
*
* Description: Class definition for mass points  
*
* Physically-Based Simulation Proseminar WS 2015
* 
* Interactive Graphics and Simulation Group
* Institute of Computer Science
* University of Innsbruck
*
*******************************************************************/

#ifndef __POINT_H__
#define __POINT_H__

#include "Vec3.h"

class Point
{
private:
    Vec3 pos;         /* Position of mass point */
    Vec3 vel;         /* Velicity of mass point */
    Vec3 force;       /* Sum of all forces on mass point */
    Vec3 userForce;   /* Additional external force exerted by user */

    double mass;      
    double damping;
    bool fixed;       /* True, if point is fixed in space */

public:               /* Various constructors */
    Point(void) 
    {
        pos = Vec3(0.0, 0.0, 0.0);
        vel = Vec3(0.0, 0.0, 0.0);
        force = Vec3(0.0, 0.0, 0.0);
        userForce = Vec3(0.0, 0.0, 0.0);

        mass = 0.0;
        damping = 0.0;
        fixed = false; 
    }

    Point(const Point &rhs)
    {
        pos = rhs.pos;
        vel = rhs.vel;
        force = rhs.force;
        userForce = rhs.userForce;

        mass = rhs.mass;
        damping = rhs.damping;
        fixed = rhs.fixed;
    }

    Point(Vec3 p) 
    {
        pos = p;
        vel = Vec3(0.0, 0.0, 0.0);
        force = Vec3(0.0, 0.0, 0.0);
        userForce = Vec3(0.0, 0.0, 0.0);

        mass = 0.0;
        damping = 0.0;
        fixed = false; 
    }
	
	Point(Vec3 p, double m, double d)
    {
        pos = p;
        vel = Vec3(0.0, 0.0, 0.0);
        force = Vec3(0.0, 0.0, 0.0);
        userForce = Vec3(0.0, 0.0, 0.0);

        mass = m;
        damping = d;
        fixed = false; 
    }

    Point(double m, double d) 
    {
        pos = Vec3(0.0, 0.0, 0.0);
        vel = Vec3(0.0, 0.0, 0.0);
        force = Vec3(0.0, 0.0, 0.0);
        userForce = Vec3(0.0, 0.0, 0.0);

        mass = m;
        damping = d;
        fixed = false; 
    }

    ~Point(void){}

    void render();

    /* Getting and setting private variables */
    void setPos(Vec3 p);
    Vec3 getPos();
    void setX(double x);
    double getX();
    void setY(double y);
    double getY();
    void setZ(double y);
    double getZ();

    void setVel(Vec3 v);
    Vec3 getVel();
    void setVelX(double vx);
    double getVelX();
    void setVelY(double vy);
    double getVelY();
    void setVelZ(double vz);
    double getVelZ();

    void setForce(Vec3 f);
    Vec3 getForce();
    void setForceX(double fx);
    double getForceX();
    void setForceY(double fy);
    double getForceY();
    void setForceZ(double fz);
    double getForceZ();

    void addForce(Vec3 f);

    void setMass(double m);
    double getMass();
    
    void setDamping(double d);
    double getDamping();

    void setFixed(bool fix);
    bool isFixed();

    void setUserForce(Vec3 f);    
    Vec3 getUserForce();
  
    /* Copy data to new point */
    Point copy();
};

#endif
