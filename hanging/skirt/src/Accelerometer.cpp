/*
 * Accelerometer.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: littlelion
 */

#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <cstring>

#include "ResourcePath.h"
#include "Accelerometer.h"

std::string Accelerometer::devicePath;
std::thread Accelerometer::updaterThread;
std::mutex Accelerometer::mtx;
std::ifstream Accelerometer::device;
double Accelerometer::sensitivity = 1.0;
bool Accelerometer::present = false;
bool Accelerometer::done = false;

double Accelerometer::x = 0.0;
double Accelerometer::y = 0.0;
double Accelerometer::z = 0.0;

//int objectCounter(int count);
//void updateValues();
//void initUpdateThread();
//void closeUpdateThread();

Accelerometer::Accelerometer(int argc, char* argv[]) {
	accelerometerInitDevice(argc, argv);
	initUpdateThread();
}

Accelerometer::Accelerometer() {
	initUpdateThread();
}

Accelerometer::~Accelerometer() {
	closeUpdateThread();
}

void Accelerometer::accelerometerInitDevice(int argc, char* argv[]) {
	if (devicePath.empty() && argv != nullptr) {
		deviceConfigFromFile(getResourcePath(CFG_FILENAME, "", argc, argv));
//		cout << "Accelerometer: device path = " << devicePath << ", sensitivity = " << sensitivity << endl;//XXX
	}
}

bool Accelerometer::isAvailable() {
	return present;
}

double Accelerometer::getX() {
	return x;
}

double Accelerometer::getY() {
	return y;
}

double Accelerometer::getZ() {
	return z;
}

void Accelerometer::updateValues() {
	while (!done) {
		double x, y, z;
		char sep;
		device.seekg(0, std::ios::beg);
		device >> sep >> x >> sep >> y >> sep >> z;
		Accelerometer::x = x * sensitivity;
		Accelerometer::y = y * sensitivity;
		Accelerometer::z = z * sensitivity;
//		std::cout << "Accelerometer read: (" << x << ", " << y << ", " << z << ")" << std::endl;//XXX
	}
}

/**
 * Initialize the updater thread, which reads periodically the state of the accelerometer.
 * This method is called each times an Accelerometer object is created.
 */
void Accelerometer::initUpdateThread() {
	mtx.lock();
	if (objectCounter(0) < 1) {
		device.open(devicePath);
		if (device.good()) {
			updaterThread = std::thread(updateValues);
			present = true;
		} else {
			present = false;
			x = 0.0;
			y = 0.0;
			z = 10.0;
		}
	}
	objectCounter(1);
	mtx.unlock();
}

/**
 * Close the updater thread if the last instance of Accelerometer is destroyed.
 * This method is called by the destructor of the Accelerometer class.
 */
void Accelerometer::closeUpdateThread() {
//	std::cout << "closeUpdateThread() called." << std::endl; //XXX
	mtx.lock();
	if(objectCounter(-1) < 1 && present) {
		done = true;
		updaterThread.join();
		device.close();
		present = false;
//		std::cout << "Updater destroyed" << std::endl; //XXX
	}
	mtx.unlock();
}

/**
 * Count the number of open accelerometers
 * @param count 1 count up, -1 count down or 0 to retrieve without change
 * @return the counter after change
 */
int Accelerometer::objectCounter(int count) {
	static int openAccelerometers = 0;
	openAccelerometers += count;
	return openAccelerometers;
}

/**
 * Read configuration for accelerometer device from config file.
 */
void Accelerometer::deviceConfigFromFile(std::string configPath) {
#define _PRIV_LINE_LEN 100
	ifstream cfg;
	cfg.open(configPath);
	if (cfg.good()) {
		bool targetSection = false;
		string line;
		while(!cfg.eof()) {
			getline(cfg, line);
			size_t start = line.find_first_not_of(" \t");
			if (start < std::string::npos && line.at(start) == '[') {
				size_t end = line.find(']');
				if (!line.compare(start+1, end-start-1, "accelerometer")) {
					targetSection = true;
				} else {
					targetSection = false;
				}
			} else if (start < std::string::npos && targetSection) {
				if (!line.compare(start, 6, "device")) {
					size_t offs = line.find_first_not_of(" \t='\"", start+6);
					size_t len = line.find_first_of("'\"\t;", offs);
					len = line.find_last_not_of(' ', len-1) - offs + 1;
					devicePath = line.substr(offs, len);
				} else if (!line.compare(start, 11, "sensitivity")) {
					size_t offs = line.find_first_not_of(" \t='\"", start+11);
					try {
						sensitivity = std::stod(line.substr(offs));
						if (sensitivity <= 0.0) {
							sensitivity = 1.0; // use default value on invalid values
						}
					} catch (...) { // use default value on any conversion error
						sensitivity = 1.0;
					}
				}
			}
		}
		cfg.close();
	}
#undef _PRIV_LINE_LEN
}
