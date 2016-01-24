#ifndef Node2_H
#define Node2_H

#include "Vector3.h"

class Node2 {
    private:
        Vector3 n0, n1;

    public:
        Node2(void){}

        Node2(Vector3 n0, Vector3 n1){
            this->n0=n0;
            this->n1=n1;
        }

        ~Node2(void){}

        Vector3 get(int i){
            switch(i){
                case 0: return this->n0;
                case 1: return this->n1;
            }
        }
};

#endif
