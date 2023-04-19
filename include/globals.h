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
extern bool initialized;
extern bool catapultState;
extern bool shieldReleased;
extern bool highReleased;
extern bool stringLauncherAuto;
extern bool shieldLauncherAuto;
extern bool highReleaseAuto;

// Pneumatics
extern pros::ADIDigitalOut stringRelease;
extern pros::ADIDigitalOut catapultRelease;
extern pros::ADIDigitalOut shieldRelease;
extern pros::ADIDigitalOut highRelease;

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

// Sensors
extern pros::IMU imu;
extern pros::Distance lsd;
extern pros::Distance msd;
extern pros::Distance bsd;

#endif // _GLOBALS_H_