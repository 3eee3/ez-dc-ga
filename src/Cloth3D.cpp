/* Standard includes */
#include <sstream>
#include <stdlib.h>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glut.h> 

/* Local includes */
#include "Cloth3D.h"
#include "Vec3.h"
#include "Point.h"
#include "Spring.h"

std::vector<Point> points;
std::vector<Spring> structural_springs;
std::vector<Spring> shear_springs;
std::vector<Spring> bend_springs;

bool inline withinBoundary(int x, int y, int res){
	if(x<0 || y<0 || x>res || y> res) 
		return false;
	else 
		return true;
}

Cloth3D::Cloth3D(int res_)
{
	//Init cloth as square 0,0 -> 1,1
	res = res_;
	double deltaGrid=1/(double)res;

	points.reserve((res+1)*(res+1));
	
	//Init positions
	for(int y=0;y<res+1;y++){
		for(int x=0;x<res+1;x++){
			points[x+y*res]=Point(Vec3(x*deltaGrid,y*deltaGrid,0.0));
		}
	}

	//Init springs, stiffness, restlength
	for(int y=0;y<res+1;y++){
		for(int x=0;x<res+1;x++){
			//structural springs
			if(withinBoundary(x-1,y,res)){
				structural_springs.push_back(Spring(&points[x+y*res],&points[x-1+y*res],10.0));
			}
			if(withinBoundary(x,y-1,res)){
				structural_springs.push_back(Spring(&points[x+y*res],&points[x+(y-1)*res],10.0));
			}
			if(withinBoundary(x+1,y,res)){
				structural_springs.push_back(Spring(&points[x+y*res],&points[x+1+y*res],10.0));
			}
			if(withinBoundary(x,y+1,res)){
				structural_springs.push_back(Spring(&points[x+y*res],&points[x+(y+1)*res],10.0));
			}

			//shear springs
			if(withinBoundary(x-1,y-1,res)){
				shear_springs.push_back(Spring(&points[x+y*res],&points[x-1+(y-1)*res],10.0));
			}
			if(withinBoundary(x+1,y-1,res)){
				shear_springs.push_back(Spring(&points[x+y*res],&points[x+1+(y-1)*res],10.0));
			}
			if(withinBoundary(x-1,y+1,res)){
				shear_springs.push_back(Spring(&points[x+y*res],&points[x-1+(y+1)*res],10.0));
			}
			if(withinBoundary(x+1,y+1,res)){
				shear_springs.push_back(Spring(&points[x+y*res],&points[x+1+(y+1)*res],10.0));
			}

			//bend springs
			if(withinBoundary(x-2,y,res)){
				bend_springs.push_back(Spring(&points[x+y*res],&points[x-2+y*res],1.0));
			}
			if(withinBoundary(x,y-2,res)){
				bend_springs.push_back(Spring(&points[x+y*res],&points[x+(y-2)*res],1.0));
			}
			if(withinBoundary(x+2,y,res)){
				bend_springs.push_back(Spring(&points[x+y*res],&points[x+2+y*res],1.0));
			}
			if(withinBoundary(x,y+2,res)){
				bend_springs.push_back(Spring(&points[x+y*res],&points[x+(y+2)*res],1.0));
			}
		}
	}
}


void Cloth3D::step(double dt)
{
		
}

void Cloth3D::draw(){
	glPushMatrix();
	glTranslatef(0.25,0.25,0.0);
    glScalef(0.5,0.5,0.5);
	for(int y=0;y<res;y++){
		glBegin(GL_TRIANGLE_STRIP);
		for(int x=0;x<res;x++){
			glColor4f(0.5, 0.5, 0.5, 1.0);
    		glVertex3f (points[x+(y+1)*res].getX(), points[x+(y+1)*res].getY(), points[x+(y+1)*res].getZ());
			glColor4f(0.9, 0.9, 0.9, 1.0);
			glVertex3f (points[x+y*res].getX(), points[x+y*res].getY(), points[x+y*res].getZ());
		}
		glEnd();
	}
	glPopMatrix();	
}

void Cloth3D::drawPoints(){
	glPushMatrix();
	glTranslatef(0.25,0.25,0.0);
    glScalef(0.5,0.5,0.5);
		int y = 0;
		glPointSize(5.0);
		glBegin(GL_POINTS);
		//first triangle
    	glVertex3f (points[0+y*res].getX(), points[0+y*res].getY(), points[0+y*res].getZ());
		glVertex3f (points[(y+1)*res].getX(), points[(y+1)*res].getY(), points[(y+1)*res].getZ());
		for(int x=1;x<res;x++){
			//other triangles
			glColor4f(0.5, 0.5, 0.5, 1.0);
    		glVertex3f (points[x+(y+1)*res].getX(), points[x+(y+1)*res].getY(), points[x+(y+1)*res].getZ());
			glColor4f(0.9, 0.9, 0.9, 1.0);
			glVertex3f (points[x+y*res].getX(), points[x+y*res].getY(), points[x+y*res].getZ());
		}
		glEnd();
	glPopMatrix();	
}




