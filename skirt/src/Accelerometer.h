/*
 * Accelerometer.h
 *
 *  Created on: Nov 12, 2015
 *      Author: littlelion
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <string>

/**
 * hard-coded device
 * TODO. do it in a generic way - some changes may be necessary to maintain multiple devices
 */
#define CFG_FILENAME "accelerometer.config"

//namespace Accelerometer {

class Accelerometer {

public:
	Accelerometer();
	Accelerometer(int argc, char* argv[]);
	~Accelerometer();

	double getX();
	double getY();
	double getZ();

	static bool isAvailable();
	static void accelerometerInitDevice(int argc, char* argv[]);

private:
	static std::string devicePath;
	static std::thread updaterThread;
	static std::mutex mtx;
	static std::ifstream device;
	static double sensitivity;
	static bool present;
	static bool done;

	static double x;
	static double y;
	static double z;

	static int objectCounter(int count);
	static void updateValues();
	static void initUpdateThread();
	static void closeUpdateThread();
	static void deviceConfigFromFile(std::string configPath);
};

//} /* namespace std */

#endif /* ACCELEROMETER_H_ */
