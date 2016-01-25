#include <iostream>
#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include <GL/gl.h>
#include <iostream>

#include "Collision.h"
#include "Mass.h"

//#define Y 0
//#define Z 1
//#define X 2
#define X 0
#define Y 1
#define Z 2

namespace std {

bool vertexInTriangle(const Eigen::Vector3d &P, const Eigen::Vector3d &A, const Eigen::Vector3d &B, const Eigen::Vector3d &C, const float epsilon, float &dist, Eigen::Vector3d &N);
bool baryVertexInTriangle(const Eigen::Vector3d &P, const Eigen::Vector3d &A, const Eigen::Vector3d &B, const Eigen::Vector3d &C);

//for testing
//int main(int argc, char **argv)
//{
//
//	float dist;
//	Eigen::Vector3d P = Eigen::Vector3d(0.25,0.25,-0.25);
//	Eigen::Vector3d N;
//
//	Eigen::Vector3d A = Eigen::Vector3d(0.0,0.0,0.0);
//	Eigen::Vector3d B = Eigen::Vector3d(1.0,0.0,0.0);
//	Eigen::Vector3d C = Eigen::Vector3d(0.0,1.0,-1.0);
//
//	bool in = vertexInTriangle(P,A,B,C,0.01,dist,N);
//	cout << "in Triangle (" << P.x() << ","<< P.y() <<"," << P.z() << ") :" << in;
//	cout << "\ndist: " << dist;
//	cout << "\nnormal: " << "(" << N.x() << ","<< N.y() <<"," << N.z() << ")\n";
//	return 0;
//}

/**
 * Checks if there is a collision with mass spring points and object mesh.
 * Adds penalty forces for collisions.
 *
 * This algorithm can't deal with self collision, so assure the object_mesh
 * doesn't contain vertices owned by points
 *
 * @param points array of mass points
 * @param offsP offset in the points array
 * @param lenP number of mass points
 * @param object_mesh vertices array with all triangles (except them present in points)
 * @param offsO offset in terms of vertices
 * @param lenO number of vertices to compute
 */
void collisionDetectionAndResponse(vector<Mass> &points, size_t offsP, size_t lenP,
		GLfloat object_mesh[], size_t offsO, size_t lenO){
	cerr << "collision: range (" << offsP << "," << offsP+lenP-1 << ") ";//XXX
	const float repulsiveSpringConst=1.0;
	const float epsilon=5.0f;

	float dist;
	Eigen::Vector3d normal;
	//for each point of mass spring system
	for (size_t i=offsP; i<points.size() && i<lenP; i++){
		cerr << "i=" << i;//XXX
		points[i].setUserForce(Eigen::Vector3d(0.0, 0.0, 0.0));
		//for each triangle of object mesh
		for(size_t j=offsO/3; j<(offsO+lenO)/3;j++){
			//TODO: generate triangles
			Eigen::Vector3d A = Eigen::Vector3d(object_mesh[j*9+X], object_mesh[j*9+Y], object_mesh[j*9+Z]);
			Eigen::Vector3d B = Eigen::Vector3d(object_mesh[j*9+3+X], object_mesh[j*9+3+Y], object_mesh[j*9+3+Z]);
			Eigen::Vector3d C = Eigen::Vector3d(object_mesh[j*9+6+X], object_mesh[j*9+6+Y], object_mesh[j*9+6+Z]);
			//detect collision
			if(vertexInTriangle(points[i].getPos(),A,B,C,epsilon,dist,normal)){
			//respond with penalty force
				Eigen::Vector3d penaltyF = repulsiveSpringConst * (-1) * (dist - epsilon) * normal;
				points[i].setUserForce(penaltyF);
				cerr << ", detected: pt " << i+offsP << "(" << offsP << "," << offsP+lenP-1 << ")"
						<< ", vert " << 3*j << "(" << offsO << "," << offsO+lenO-1 << "), j=" << j
						<< ", normal = "<< normal[0] << ", " << normal[1] << ", " << normal[2];//XXX
				break; //collision with one triangle found (possible other collisions not considered)
				//points[i].setForce(penaltyF);
			}
			//XXX start
//			Eigen::Vector3d N = (B-A).cross(C-A);
//			cerr << "j=" << j << "N=(" << N[0] << "," << N[1] << "," << N[2] << ") ";
			//XXX end
		}
	}
	cerr << endl;//XXX
}

/*
	Compute barycentric coordinates to perform point in triangle check
*/
bool baryVertexInTriangle(const Eigen::Vector3d &P, const Eigen::Vector3d &A, const Eigen::Vector3d &B, const Eigen::Vector3d &C){
	// Compute vectors        
	Eigen::Vector3d v0 = C - A;
	Eigen::Vector3d v1 = B - A;
	Eigen::Vector3d v2 = P - A;

	// Compute dot products
	float dot00 = v0.dot(v0);
	float dot01 = v0.dot(v1);
	float dot02 = v0.dot(v2);
	float dot11 = v1.dot(v1);
	float dot12 = v1.dot(v2);

	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return (u >= 0) && (v >= 0) && (u + v < 1);
}

/*	uses a certain threshold (epsilon) for the distance to the surface
	returns true if vertex lies on surface of triangle mesh 
	returns penalty depth (dist), and surface normal (N)
*/
bool vertexInTriangle(const Eigen::Vector3d &P, const Eigen::Vector3d &A, const Eigen::Vector3d &B, const Eigen::Vector3d &C, const float epsilon, float &dist, Eigen::Vector3d &N){
		//signed distance of point from plane
		N = (B-A).cross(C-A); //N not normalized
		dist = N.dot(P) - N.dot(A); //distance not normalized (length of N constant for triangles with same area)
		cerr << "(dist=" << dist << ")";//XXX
		//check if point is near plane
		if(dist > epsilon || dist < -epsilon){
			return false;
		}

		//project point P on plane (closest point to plane normal:N point:A)
		Eigen::Vector3d P_ = P - ((N.dot(P-A))/(N.dot(N)))*N;

		//check if vertex P_ lies in triangle
		if(baryVertexInTriangle(P_,A,B,C)){
			return true;
		}
	  	return false;
}

/* Incomplete implementation
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

void inline compute_intervals(double p0, double p1, double p2, double d1, double d2, double d3, double &t1,double &t2){
	//cases for d1 , d2 , d3
	//TO DO
	t1 = p0 + (p1-p0)*d0/(d0-d1);
	t2 = p0 + (p2-p0)*d0/(d0-d2);	
}
*/

} // namespace std
