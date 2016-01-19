/******************************************************************
*
* Mass.h
*
* This is a derived work from Point.h
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

#ifndef __MASS_H__
#define __MASS_H__

#include <Eigen/Dense>

class Mass
{
private:
    Eigen::Vector3d pos;         /* Position of mass point */
    Eigen::Vector3d vel;         /* Velicity of mass point */
    Eigen::Vector3d force;       /* Sum of all forces on mass point */
    Eigen::Vector3d userForce;   /* Additional external force exerted by user */

    double mass;      
    double damping;
    bool fixed;       /* True, if point is fixed in space */

public:               /* Various constructors */
    Mass(void) 
    {
        pos = Eigen::Vector3d(0.0, 0.0, 0.0);
        vel = Eigen::Vector3d(0.0, 0.0, 0.0);
        force = Eigen::Vector3d(0.0, 0.0, 0.0);
        userForce = Eigen::Vector3d(0.0, 0.0, 0.0);

        mass = 0.0;
        damping = 0.0;
        fixed = false; 
    }

    Mass(const Mass &rhs)
    {
        pos = rhs.pos;
        vel = rhs.vel;
        force = rhs.force;
        userForce = rhs.userForce;

        mass = rhs.mass;
        damping = rhs.damping;
        fixed = rhs.fixed;
    }

    Mass(Eigen::Vector3d p)
    {
        pos = p;
        vel = Eigen::Vector3d(0.0, 0.0, 0.0);
        force = Eigen::Vector3d(0.0, 0.0, 0.0);
        userForce = Eigen::Vector3d(0.0, 0.0, 0.0);

        mass = 0.0;
        damping = 0.0;
        fixed = false; 
    }

    Mass(Eigen::Vector3d p, double m, double d)
    {
        pos = p;
        vel = Eigen::Vector3d(0.0, 0.0, 0.0);
        force = Eigen::Vector3d(0.0, 0.0, 0.0);
        userForce = Eigen::Vector3d(0.0, 0.0, 0.0);

        mass = m;
        damping = d;
        fixed = false; 
    }

    Mass(double m, double d) 
    {
        pos = Eigen::Vector3d(0.0, 0.0, 0.0);
        vel = Eigen::Vector3d(0.0, 0.0, 0.0);
        force = Eigen::Vector3d(0.0, 0.0, 0.0);
        userForce = Eigen::Vector3d(0.0, 0.0, 0.0);

        mass = m;
        damping = d;
        fixed = false; 
    }

    ~Mass(void){}

    void render();

    /* Getting and setting private variables */
    void setPos(Eigen::Vector3d p);
    Eigen::Vector3d getPos();
    void setX(double x);
    double getX();
    void setY(double y);
    double getY();
    void setZ(double z);
    double getZ();

    void setVel(Eigen::Vector3d v);
    Eigen::Vector3d getVel();
    void setVelX(double vx);
    double getVelX();
    void setVelY(double vy);
    double getVelY();
    void setVelZ(double vz);
    double getVelZ();

    void setForce(Eigen::Vector3d f);
    Eigen::Vector3d getForce();
    void setForceX(double fx);
    double getForceX();
    void setForceY(double fy);
    double getForceY();
    void setForceZ(double fz);
    double getForceZ();

    void addForce(Eigen::Vector3d f);

    void setMass(double m);
    double getMass();
    
    void setDamping(double d);
    double getDamping();

    void setFixed(bool fix);
    bool isFixed();

    void setUserForce(Eigen::Vector3d f);
    Eigen::Vector3d getUserForce();
  
    /* Copy data to new point */
    Mass copy();
};

#endif // _MASS_H_
