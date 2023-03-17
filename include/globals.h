#include "main.h"

// To prevent double-declaration of globals
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// Variables
extern double startTime;
extern bool fiveSecondWarningTriggered;
extern bool stringReleased;
extern bool clockOverride;
extern char auton;
extern bool skills;
extern bool mBROState;
extern bool initialized;
extern bool catapultState;
extern bool shieldReleased;
extern bool stringLauncherAuto;
extern bool shieldLauncherAuto;

// Pneumatics
extern pros::ADIDigitalOut stringRelease;
extern pros::ADIDigitalOut catapultRelease;
extern pros::ADIDigitalOut shieldRelease;

// Controllers
extern pros::Controller master;
extern pros::Controller partner;

// Motors
extern pros::Motor mBRO;
extern pros::Motor mBRI;
extern pros::Motor mFRO;
extern pros::Motor mFRI;
extern pros::Motor mBLO;
extern pros::Motor mBLI;
extern pros::Motor mFLO;
extern pros::Motor mFLI;
extern pros::Motor mCATA;

// Sensors
extern pros::IMU imu;
extern pros::GPS gps;
extern pros::Distance lsd;

extern void reloadCatapult();

#endif // _GLOBALS_H_