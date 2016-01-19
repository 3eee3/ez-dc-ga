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

#include <Eigen/Dense>
using namespace Eigen;

class Point
{
private:
    Vector3d pos;         /* Position of mass point */
    Vector3d vel;         /* Velicity of mass point */
    Vector3d force;       /* Sum of all forces on mass point */
    Vector3d userForce;   /* Additional external force exerted by user */

    double mass;      
    double damping;
    bool fixed;       /* True, if point is fixed in space */

public:               /* Various constructors */
    Point(void) 
    {
        pos = Vector3d(0.0, 0.0, 0.0);
        vel = Vector3d(0.0, 0.0, 0.0);
        force = Vector3d(0.0, 0.0, 0.0);
        userForce = Vector3d(0.0, 0.0, 0.0);

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

    Point(Vector3d p)
    {
        pos = p;
        vel = Vector3d(0.0, 0.0, 0.0);
        force = Vector3d(0.0, 0.0, 0.0);
        userForce = Vector3d(0.0, 0.0, 0.0);

        mass = 0.0;
        damping = 0.0;
        fixed = false; 
    }

    Point(Vector3d p, double m, double d)
    {
        pos = p;
        vel = Vector3d(0.0, 0.0, 0.0);
        force = Vector3d(0.0, 0.0, 0.0);
        userForce = Vector3d(0.0, 0.0, 0.0);

        mass = m;
        damping = d;
        fixed = false; 
    }

    Point(double m, double d) 
    {
        pos = Vector3d(0.0, 0.0, 0.0);
        vel = Vector3d(0.0, 0.0, 0.0);
        force = Vector3d(0.0, 0.0, 0.0);
        userForce = Vector3d(0.0, 0.0, 0.0);

        mass = m;
        damping = d;
        fixed = false; 
    }

    ~Point(void){}

    void render();

    /* Getting and setting private variables */
    void setPos(Vector3d p);
    Vector3d getPos();
    void setX(double x);
    double getX();
    void setY(double y);
    double getY();
    void setZ(double z);
    double getZ();

    void setVel(Vector3d v);
    Vector3d getVel();
    void setVelX(double vx);
    double getVelX();
    void setVelY(double vy);
    double getVelY();
    void setVelZ(double vz);
    double getVelZ();

    void setForce(Vector3d f);
    Vector3d getForce();
    void setForceX(double fx);
    double getForceX();
    void setForceY(double fy);
    double getForceY();
    void setForceZ(double fz);
    double getForceZ();

    void addForce(Vector3d f);

    void setMass(double m);
    double getMass();
    
    void setDamping(double d);
    double getDamping();

    void setFixed(bool fix);
    bool isFixed();

    void setUserForce(Vector3d f);
    Vector3d getUserForce();
  
    /* Copy data to new point */
    Point copy();
};

#endif
