#include <vector>
#include <cmath>
#include <Eigen/Dense>

/*
	Returns true if the triangles p0,p1,p2 and q0,q1,q2 intersect  
*/

namespace std{

vector<Eigen::Vector3d> p;
vector<Eigen::Vector3d> q;

bool triangleTriangleIntersection(vector<Eigen::Vector3d> &p, vector<Eigen::Vector3d> &q){
	//p0,p1,p2 and q0,q1,q2
	
	//first step (half plane test for vertices p0,p1,p2 against triangle(q0,q1,q2) and vice versa
	Eigen::Vector3d nq = (q[1]-q[0]).cross(q[2]-q[0]);
	Eigen::Vector3d np = (p[1]-p[0]).cross(p[2]-p[0]);

	double dp[3];
	double dq[3];
	//signbit(x) returns true if the sign of x is negative otherwise false (for x=0 it returns false);
	//d = 0 for vertices of co-planar triangles (define as no intersection)
	for(int i=0;i<3;i++){
		dp[i] = nq.dot(p[i]-q[0]);
		dq[i] = np.dot(q[i]-p[0]);
	}
	//if vertices have same sign -> no intersection
	if(signbit(s1[0])==signbit(s1[1]) && signbit(s1[0])==signbit(s1[2]) 
		&& signbit(s2[0])==signbit(s2[1]) && signbit(s2[0])==signbit(s2[2])){
		return false;
	}
	//else planes of 2 triangles intersect
	else{
		double t1, t2, tt1, tt2; //t1,t2 for triangle p   tt1,tt2 for triangle q
		//direction of intersection line D
		Eigen::Vector3d D = nq.cross(np);
		
		//project vertices onto L (optimized, Thomas Möller)
		double pp0 = D.dot(p[0]);//pV1_0
		double pp1 = D.dot(p[1]);//pV1_1
		double pp2 = D.dot(p[2]);//pV1_2
		double qp0 = D.dot(q[0]);//pV2_0
		double qp1 = D.dot(q[1]);//pV2_1
		double qp2 = D.dot(q[2]);//pV2_2

		//compute scalar interval of intersection between triangles and L = P + t*D
		//Triangle p
		compute_intervals(pp0,pp1,pp2,dp[0],dp[1],dp[2],t1,t2);
		if(t1>t2){double tmp=t1; t1=t2;t2=tmp;};

		//Triangle q
		compute_intervals(qp0,qp1,qp2,dq[0],dq[1],dq[2],tt1,tt2);
		if(tt1>tt2){double tmp=tt1; tt1=tt2;tt2=tmp;};

		if(tt1<t2 && t1<tt2){//check if intervals overlap
			return true;
		}else{
			return false;
		}
	}
}

}//namespace std

void inline compute_intervals(double p0, double p1, double p2, double d1, double d2, double d3, double &t1,double &t2){
	//cases for d1 , d2 , d3
	t1 = p0 + (p1-p0)*d0/(d0-d1);
	t2 = p0 + (p2-p0)*d0/(d0-d2);	
}


