/*
 * Scene.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: littlelion
 */

#include <GL/freeglut.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "Scene.h"
#include "Mass.h"
#include "Spring.h"
#include "Simulation.h"
#include "Collision.h"
#include "model_mapping.h"

namespace std {

/**
 * Default constructor
 */
Scene::Scene() : step(0.003), mass(0.15), stiffness(60.0), weak_stiff(5.0), damping(0.08) {}

/**
 * Constructor which initializes the objectives with values given by command line parameters.
 * @param argc arguments counter supplied by the main function
 * @param argv arguments vector of length @c argc
 */
Scene::Scene(int argc, char* argv[]) : step(0.003), mass(0.15), stiffness(60.0), weak_stiff(5.0), damping(0.08) {
	int arg = 1;
	while (arg < argc) {
		if(arg < argc +1) {
			if (!strcmp(argv[arg], "-step")) {
				double par = atof(argv[++arg]);
				step = par > 0.0 ? par : 0.003;
			}
			if (!strcmp(argv[arg], "-mass")) {
				double par = atof(argv[++arg]);
				mass = par > 0.0 ? par : 0.15;
			}
			if (!strcmp(argv[arg], "-stiff")) {
				double par = atof(argv[++arg]);
				stiffness = par > 0.0 ? par : 60.0;
			}
			if (!strcmp(argv[arg], "-wstiff")) {
				double par = atof(argv[++arg]);
				weak_stiff = par > 0.0 ? par : 5.0;
			}
			if (!strcmp(argv[arg], "-damp")) {
				double par = atof(argv[++arg]);
				damping = par > 0.0 ? par : 0.08;
			}
		}
		if (!strcmp(argv[arg], "-h")) {
			cout << "Arguments:\n";
			cout << "\t-step <val>     time step in seconds (default 0.003)\n";
			cout << "\t-mass <val>     mass for each mass point (0.15)\n";
			cout << "\t-stiff <val>    stiffness factor for stron springs (60.0)\n";
			cout << "\t-wstiff <val>   stiffness for weak springs (5.0)\n";
			cout << "\t-damp <val>     damping factor of masses (friction) (0.08)\n";
			cout << "\t-h              print this help message and exit\n";
			exit(EXIT_SUCCESS);
		}
		++arg;
	}
}

/**
 * Default destructor.
 */
Scene::~Scene() {}

/**
 * Initialize structural (strong) and bending (weak) springs. No diagonal springs, because of triangular grid
 *
 * @param i the i-th 3D-object to process
 */
void Scene::initSpringsVector(size_t i) {
	/* done? prevent from adding springs twice, upper triangle reserved for weak springs,
	 * lower triangle reserved for strong structural springs, diagonal is left free
	 */
	vector<vector<bool> > done = vector<vector<bool> >(model3dMasses[i],
			vector<bool>(model3dMasses[i], false));

	// for all triangles of object with mass
	for (size_t j = 0; j < model3dMassVertices[i] / 3; ++j) {

		// initialize structural springs - process each edge of all triangles
		for (size_t k = 0; k < 3; ++k) {
			size_t first = model3dRevIndex[3 * j + model3dMassRevOffs[i] + k];
			size_t second = model3dRevIndex[3 * j + model3dMassRevOffs[i]
					+ (k + 1) % 3];
			if (first > second
					&& !done[first - model3dMassFwdOffs[i]][second
							- model3dMassFwdOffs[i]]) {
				springs.push_back(Spring(stiffness));
				springs.back().init(&points[first], &points[second]);
				done[first - model3dMassFwdOffs[i]][second
						- model3dMassFwdOffs[i]] = true;
//				cerr << "adding structural spring to (" << first << "," << second << "), j = " << j << ", k = " << k
//						<< ", done[" << first-model3dMassFwdOffs[i] << "][" << second-model3dMassFwdOffs[i] << "]" << endl;//XXX
			} else if (first < second
					&& !done[second - model3dMassFwdOffs[i]][first
							- model3dMassFwdOffs[i]]) {
				springs.push_back(Spring(stiffness));
				springs.back().init(&points[second], &points[first]);
				done[second - model3dMassFwdOffs[i]][first
						- model3dMassFwdOffs[i]] = true;
//				cerr << "adding structural spring to (" << second << "," << first << "), j = " << j << ", k = " << k
//						<< ", done[" << second-model3dMassFwdOffs[i] << "][" << first-model3dMassFwdOffs[i] << "]" << endl;//XXX
//			} else {//XXX
//				cerr << "wrong structural spring at " << first << endl;//XXX
			}
		}
//			cerr << "j=" << j << endl;//XXX

		// initialize bending springs - process all triangles after the j-th, they may share an edge with the j-th triangle
		// connect the mass-points which aren't part of the shared edge
		for (size_t k = j + 1; k < model3dMassVertices[i] / 3; ++k) {
//			cerr << "k=" << k << endl;//XXX
			size_t found[4], first, second; // points: of shared edge if found, end-points of weak spring
			int n = 0; // number of found shared points

			for (size_t l = 0; l < 3; ++l) {
				// check all combinations of vertices of the two compared triangles
				for (size_t m = 0; m < 3 && n < 4; ++m) {
//					cerr << "i=" << i << ", j=" << j << ", k=" << k << ", l=" << l << ", m=" << m << ", n=" << n << endl;//XXX
					if (model3dRevIndex[3 * j + model3dMassRevOffs[i] + l]
							== model3dRevIndex[3 * k + model3dMassRevOffs[i] + m]) {
						// store the index of the vertex if found equal
						found[n++] = l;
						found[n++] = m;
//						cerr << "found " << model3dRevIndex[3*j+model3dMassRevOffs[i]+l] << "@" << l << "," << m << endl;//XXX
						break;
					}
				}
				if (n >= 4) {
					// 'found' contains 4 entries if two equal vertices in both triangles
					// indices of points for potentially missing springs, first, second are those for shared edges
					first = model3dRevIndex[3 * j + model3dMassRevOffs[i] + 3
							- found[0] - found[2]];
					second = model3dRevIndex[3 * k + model3dMassRevOffs[i] + 3
							- found[1] - found[3]];
					if (first < second
							&& !done[first - model3dMassFwdOffs[i]][second
									- model3dMassFwdOffs[i]]) {
						springs.push_back(Spring(weak_stiff));
						springs.back().init(&points[first], &points[second]);
						done[first - model3dMassFwdOffs[i]][second
								- model3dMassFwdOffs[i]] = true;
//						cerr << "adding bending spring to (" << first << "," << second << "), j = " << j << ", k = " << k
//								<< ", done[" << first-model3dMassFwdOffs[i] << "][" << second-model3dMassFwdOffs[i] << "]" << endl;//XXX
					} else if (first > second
							&& !done[second - model3dMassFwdOffs[i]][first
									- model3dMassFwdOffs[i]]) {
						springs.push_back(Spring(weak_stiff));
						springs.back().init(&points[second], &points[first]);
						done[second - model3dMassFwdOffs[i]][first
								- model3dMassFwdOffs[i]] = true;
//						cerr << "adding bending spring to (" << second << "," << first << "), j = " << j << ", k = " << k
//								<< ", done[" << second-model3dMassFwdOffs[i] << "][" << first-model3dMassFwdOffs[i] << "]" << endl;//XXX
//					} else {//XXX
//						cerr << "wrong bending spring at " << first << endl;//XXX
					}

					break;
				}
			}
		}
	}
//	for(size_t ii = 0; ii < done.size(); ++ii) {//XXX begin: done[] array to console
//		for (size_t jj = 0; jj < done[ii].size(); ++jj) {
//			cerr << done[ii][jj] << " ";
//		}
//		cerr << endl;
//	}//XXX end
}

/**
 * Initialize all mass-points.
 *
 * This method connects all mass-points processed by the physics simulation to the vertices
 * in the ...Positions vector for the renderer.
 * The vector of points must be filled with the correct number of Mass objects initialized
 * with default values.
 * @param i the i-th 3d-object
 * @param n a pointer to a counter value, it is incremented with each initialized mass-point.
 *          it must be initialized with 0 before the first call of this method.
 */
void Scene::initPointsVector(size_t i, size_t *n) {
	// for all objects with mass-spring definition
	// initialize mass points with data given by the model header
	for (size_t j = 0; j < model3dMasses[i]; ++j, ++(*n)) {
		//			points.push_back(Mass(mass, damping));
		points[*n].mass = mass;
		points[*n].damping = damping;
		for (size_t k = 0; k < model3dFwdIndexLength[j]; ++k) {
			points[*n].registerVertex(
					&model3dPositions[model3dFwdIndex[j + model3dMassFwdOffs[i]][k]
							* 3]);
			//				cerr << "register vtx " << model3dFwdIndex[j+model3dMassFwdOffs[i]][k]*3
			//						<< ", j=" << j << ", k=" << k << endl;//XXX
		}
	}
}

/**
 * Initialize the scene for rendering and physical simulation.
 */
void Scene::initialize() {
//	GLfloat model3dColor[model3dVertices*3];/XXX
//	for(int i = 0; i < model3dVertices*3;++i) model3dColor[i]=1/(float)(rand()%254+1);
//	int model3dIndices[model3dVertices*3];
//	for(int i = 0; i < model3dVertices;++i) model3dIndices[i]=i;

	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, model3dPositions);
//	glColorPointer(3, GL_FLOAT, 0, model3dColor);
	glTexCoordPointer(2, GL_FLOAT, 0, model3dTexels);
	glNormalPointer(GL_FLOAT, 0, model3dNormals);

	GLfloat mShininess[] = {128};
	GLfloat whiteSpecularMaterial[] = {1.0, 1.0, 1.0};
	GLfloat whiteDiffuseMaterial[] = {0.4f, 0.4f, 0.4f};
	GLfloat blankMaterial[] = {0.0, 0.0, 0.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteDiffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blankMaterial);

	size_t nMasses = 0;
	for (size_t i = 0; i < model3dObjectsWithMass; ++i) {
		nMasses += model3dMasses[i];
	}
	points.resize(nMasses);
	size_t n = 0;
	for (size_t i = 0; i < model3dObjectsWithMass; ++i) { // for all objects with mass-spring definition
		// initialize mass points with data given by the model header
		initPointsVector(i, &n);

		initSpringsVector(i);
//		points[328].fixed = true;//XXX fixation of cloth
	}
}

/**
 * Call the collision detection algorithm for each 3D-object with mass.
 */
void Scene::detectCollisions() {
	//FIXME re-implementing the collision detection with self-collision will make the for loop obsolete
	// for all objects with mass-spring definition
	// LEAVE OUT all vertices of the object which is tested for collisions
	size_t j = 0;
	for (size_t i = 0; i < model3dObjectsWithMass; ++i) {
		while (j < model3dObjects
				&& model3dObjectOffset[j] < model3dMassRevOffsOrig[i]) {
			++j; // j-th object is the actually processed object in the points array
		}
		size_t next = j < model3dObjects ? j + 1 : model3dObjects;
		//		cout << "entered coll loop i=" << i << ", j=" << j << ", model3dObjects=" << model3dObjects
		//				<< ", model3dObjectOffset[j]" << model3dObjectOffset[j]
		//				<< ", model3dMassRevOffsOrig[i]" << model3dMassRevOffsOrig[i] << endl;//XXX
		// collisions with all preceding objects
		if (model3dMassRevOffsOrig[i] > 0) {
			//			cerr << "prev collision " << i << " - " << "[0 len " << model3dObjectOffset[j] << "]" << endl;//XXX
			std::collisionDetectionAndResponse(points, model3dMassFwdOffs[i],
					model3dMasses[i], model3dPositions, 0,
					model3dObjectOffset[j]);
		}
		// collisions with all succeeding objects
		if (model3dMassRevOffsOrig[i]
				< model3dObjectOffset[model3dObjects - 1]) {
			//			cerr << "post collision " << i << " - " << "[" << model3dObjectOffset[next] << " len: " << model3dVertices - model3dObjectOffset[next] << "]" << endl;//XXX
			std::collisionDetectionAndResponse(points, model3dMassFwdOffs[i],
					model3dMasses[i], model3dPositions,
					model3dObjectOffset[next],
					model3dVertices - model3dObjectOffset[next]);
		}
	} //FIXME collision detection doesn't work properly
//	cerr << endl;//XXX
}

/**
 * Update the simulation step and collision detection.
 */
void Scene::update() {
	Simulation::step(step, Simulation::SYMPLECTIC, points, springs);

	detectCollisions();

	// for dice.h
//	std::collisionDetectionAndResponse(points, 0, 8, model3dPositions, 0, 6);
//	std::collisionDetectionAndResponse(points, 0, 8, model3dPositions, 42, 36);
//	std::collisionDetectionAndResponse(points, 8, 8, model3dPositions, 0, 6);
//	std::collisionDetectionAndResponse(points, 8, 8, model3dPositions, 6, 36);
	// for dice1.h
//	std::collisionDetectionAndResponse(points, 0, 8, model3dPositions, 0, 384);
//	std::collisionDetectionAndResponse(points, 0, 8, model3dPositions, 420, 36);
//	std::collisionDetectionAndResponse(points, 8, 8, model3dPositions, 0, 384);
//	std::collisionDetectionAndResponse(points, 8, 8, model3dPositions, 384, 36);

}

/**
 * Render the scene.
 */
void Scene::render() {
	// TODO Auto-generated destructor stub
	glLoadIdentity();
	gluLookAt(-4.5, 4.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	updateNormals();

//	for (size_t i = 0; i < model3dVertices * 3; ++i) {//XXX
//		if (model3dPositions[i] > 10.0f || model3dPositions[i] < -10 ) {
//			std::cerr << "model3dPositions[" << i << "] out of bounds: " << model3dPositions[i] << std::endl;//XXX
//		}
//	}
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)model3dVertices);//FIXME re-enable
//	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)model3dObjectOffset[2]);
//	int model3dIndices[model3dVertices*3];
//	for(int i = 0; i < model3dVertices;++i) model3dIndices[i]=i;
//	glDrawElements(GL_TRIANGLES, (GLsizei)model3dVertices, GL_UNSIGNED_BYTE, model3dIndices);
	glFlush();
}

/**
 * Get the time step for the simulation.
 * @return step in seconds
 */
double Scene::getStep() const {
	return step;
}

/**
 * Recalculate all normals for all faces modified by the simulaton step.
 */
void Scene::updateNormals() {
	for (size_t i = 0; i < model3dVertices/3; ++i) {
		GLfloat* a_x = &model3dPositions[i*9];
		GLfloat* n_x = &model3dNormals[i*9];

		Eigen::Vector3f a, b, c, n;
		a = Eigen::Vector3f(*a_x, *(a_x+1), *(a_x+2));
		b = Eigen::Vector3f(*(a_x+3), *(a_x+4), *(a_x+5));
		c = Eigen::Vector3f(*(a_x+6), *(a_x+7), *(a_x+8));
		n = (b-a).cross(c-a).normalized();

		if (!n.hasNaN()) {
			*n_x = n[0];
			*(n_x+1) = n[1];
			*(n_x+2) = n[2];
			*(n_x+3) = n[0];
			*(n_x+4) = n[1];
			*(n_x+5) = n[2];
			*(n_x+6) = n[0];
			*(n_x+7) = n[1];
			*(n_x+8) = n[2];
		}
	}
}

} /* namespace std */
