#ifndef Triangle_H
#define Triangle_H

#include "Node2.h"
#include "Vector3.h"

class Triangle {
    private:
         Vector3 n0, n1, n2;

    public:
        Triangle(void){}

        Triangle(Vector3 n0, Vector3 n1, Vector3 n2){
            this->n0=n0;
            this->n1=n1;
            this->n2=n2;
        }

        Vector3 get(int i){
            switch(i){
                case 0: return this->n0;
                case 1: return this->n1;
                case 2: return this->n2;
            }
        }

        bool isTriangle(){
            if(this->n0.isEqual(this->n1)||this->n0.isEqual(this->n2)||this->n1.isEqual(this->n2))return false;
            return true;
        }

        bool isNeighbour(Triangle t){
            int n=0;
            for(int i=0;i<3;i++)
                for(int j=0;j<3;j++)
                    if(t.get(i).isEqual(t.get(j)))n++;
            if(n==2)return true;
            return false;
        }

        void getWeakNeighbourVector(Triangle t1, Triangle t2, Node2 &crossNodesOfNeighbour, float &distance){
            int vi[2];
            int vj[2];
            if(t1.isNeighbour(t2)){
                int n=0;
                for(int i=0;i<3;i++){
                    for(int j=0;j<3;j++){
                        if(t1.get(i).isEqual(t2.get(j))){
                           vi[n]=i;
                           vj[n]=j;
                           n++;
                        }
                    }
                }
                Vector3 v0;
                Vector3 v1;
                for(int i=0;i<3;i++){
                    if(i!=vi[0]&&i!=vi[1]) v0 = t1.get(i);
                    if(i!=vj[0]&&i!=vj[1]) v1 = t2.get(i);
                }
                crossNodesOfNeighbour = Node2(v0,v1);
                distance = v0.getDistanceTo(v1);
            }
        }
};

#endif
