#include "main.h"

// To prevent double-declaration of globals
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// Pneumatics
extern pros::ADIDigitalOut stringRelease;
extern pros::ADIDigitalOut catapultRelease;
extern pros::ADIDigitalOut shieldRelease;

// Controllers
extern pros::Controller master;

// Motors
extern pros::Motor mBRO;
extern pros::Motor mBRI;
extern pros::Motor mFRO;
extern pros::Motor mFRI;
extern pros::Motor mBLO;
extern pros::Motor mBLI;
extern pros::Motor mFLO;
extern pros::Motor mFLI;

// Sensors
extern pros::IMU imu;
extern pros::GPS gps;

#endif // _GLOBALS_H_