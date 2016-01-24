#ifndef Vector3_H
#define Vector3_H

#include<math.h>

class Vector3{
    private:
        GLfloat x, y, z;
 //       Vector3 n0, Vector3 n1, Vector3 n2;
    public:
        Vector3(void){
            x=0.0; y=0.0; z=0.0;
        }
 /*       Vector3(Vector3 n0, Vector3 n1, Vector3 n2){
            this->n0=n0;
            this->n1=n1;
            this->n2=n2;
        }
*/
        Vector3(GLfloat x, GLfloat y, GLfloat z){
            this->x=x;
            this->y=y;
            this->z=z;
        }

        GLfloat get(int i){ //Vec(x,y,z)
            switch(i){
                case 0: return x;
                case 1: return y;
                case 2: return z;
            }
        }
/*
        Vector3 get(int i){ //Vec(x,y,z)
            switch(i){
                case 0: return this->n0;
                case 1: return this->n1;
                case 2: return this->n2;
            }
        }
*/
        bool isEqual(Vector3 n){
            if(n.get(0)== this->x && n.get(1)==this->y && n.get(2)==this->z) return true;
            return false;
        }

        float getDistanceTo(Vector3 n){
            return sqrt((float)(pow(n.get(0)-x,2) + pow(n.get(1)-y,2) + pow(n.get(2)-z,2)));
        }
};

#endif

