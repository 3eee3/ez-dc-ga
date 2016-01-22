#include <iostream>
#include <vector>
#include <cmath>
#include <Eigen/Dense>

using namespace std;

bool vertexInTriangle(const Eigen::Vector3d &P, const Eigen::Vector3d &A, const Eigen::Vector3d &B, const Eigen::Vector3d &C, float &dist, Eigen::Vector3d &N);
bool baryVertexInTriangle(const Eigen::Vector3d &P, const Eigen::Vector3d &A, const Eigen::Vector3d &B, const Eigen::Vector3d &C);

//for testing
int main(int argc, char **argv)
{
	
	float dist;
	Eigen::Vector3d P = Eigen::Vector3d(0.25,0.25,-0.25);
	Eigen::Vector3d N;

	Eigen::Vector3d A = Eigen::Vector3d(0.0,0.0,0.0);
	Eigen::Vector3d B = Eigen::Vector3d(1.0,0.0,0.0);
	Eigen::Vector3d C = Eigen::Vector3d(0.0,1.0,-1.0);
	
	bool in = vertexInTriangle(P,A,B,C,dist,N);
	cout << "in Triangle (" << P.x() << ","<< P.y() <<"," << P.z() << ") :" << in;
	cout << "\ndist: " << dist;
	cout << "\nnormal: " << "(" << N.x() << ","<< N.y() <<"," << N.z() << ")\n";
	return 0;
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
bool vertexInTriangle(const Eigen::Vector3d &P, const Eigen::Vector3d &A, const Eigen::Vector3d &B, const Eigen::Vector3d &C, float &dist, Eigen::Vector3d &N){
		//signed distance of point from plane
		N = (B-A).cross(C-A); //N not normalized
		dist = N.dot(P) - N.dot(A); //distance not normalized (length of N constant for triangles with same area)
	
		//check if point is near plane
		float epsilon = 0.01f;
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
