/******************************************************************
*
* Vec3.h
*
* Description: Code providing helper function for handling 3D
* vectors; standard operators and operators are provided  
*
*******************************************************************/

#ifndef __VEC3_H__
#define __VEC3_H__

#include <math.h>

class Vec3
{
public:
    double x, y, z;

public:
    Vec3(void){x=0.0; y=0.0; z=0.0;}
    Vec3(double x_, double y_, double z_){x=x_; y=y_;z=z_;}
    ~Vec3(void){}

    void operator+=(const Vec3& v)
    {
        x+=v.x; y+=v.y; z+=v.z;
    }

    void operator-=(const Vec3& v)
    {
        x-=v.x; y-=v.y; z-=v.z;
    }

    Vec3 operator+(const Vec3& v) const
    {
        return Vec3(x+v.x, y+v.y, z+v.z);
    }

    Vec3 operator-(const Vec3& v) const
    {
        return Vec3(x-v.x, y-v.y, z-v.z);
    }
   
    Vec3 operator-() const
    {
        return Vec3(-x, -y, -z);
    }

    Vec3 operator*(const double k) const
    {
        return Vec3(k*x, k*y, k*z);
    }

    Vec3 operator/(const double k) const
    {
        return Vec3(x/k, y/k, z/k);
    }
	
    friend Vec3 operator*(double k, const Vec3& v)
    {
        return Vec3(k*v.x, k*v.y, k*v.z);
    }

    double dot(const Vec3& v) const
    {
        return x*v.x+y*v.y+z*v.z;
    }
	/*
    double cross(const Vec2& v) const
    {
        return x*v.y-y*v.x;
    }*/

    double length(void) const
    {
        return sqrt(x*x+y*y+z*z);
    }

    Vec3 normalize() const
    {
        return Vec3(x,y,z) / Vec3(x,y,z).length();
    }
};

#endif 
