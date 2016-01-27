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
 * This definition contains the hard-coded path to the device-config-file.
 * TODO. do it in a generic way - some changes may be necessary to maintain multiple devices
 */
#define CFG_FILENAME "accelerometer.config"

/**
 * This class handles the connection to the accelerometer device of a notebook.
 *
 * Actually it supports the accelerometer of type lis3lv02d.
 * Other devices are unsupported, but they may work with proper settings in the configuration file.\n
 * \n
 * Configuration file:\n
 * The file follows the specifications of a windows INI file. Here is an example setup:
 * \code{.ini}
 * [accelerometer] ; this section must be present
 * device = "/sys/devices/platform/lis3lv02d/position" ; must be an absolute path to the unix device
 * sensitivity = 0.001 ; multiplier to scale the accelerometer output to fit it to gravity force
 * \endcode
 */
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

#endif /* ACCELEROMETER_H_ */
