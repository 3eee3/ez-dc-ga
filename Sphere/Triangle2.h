#ifndef Triangle2_H
#define Triangle2_H

#include "Triangle.h"

class Triangle2 {
    private:
        Triangle t0, t1;

    public:
        Triangle2(void){}

        Triangle2(Triangle t0, Triangle t1){
            this->t0=t0;
            this->t1=t1;
        }

        ~Triangle2(void){}

        Triangle get(int i){
            switch(i){
                case 0: return this->t0;
                case 1: return this->t1;
            }
        }
};

#endif
