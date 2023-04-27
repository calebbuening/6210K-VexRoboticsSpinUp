#include "main.h"
#include "api.h"
#include "globals.h"
#include <fstream>
#include <string>

#ifndef _AUTONMETHODS_H_
#define _AUTONMETHODS_H_
extern void eliScoreRoller();

// Returns -1, 0, and 1 based on number direction
extern int sgn(double d); // Mimimcs the mathematical sgn function

extern void turnViaIMU(double heading);

// Drive guided by IMU
extern void driveViaIMU(double dist, double heading, double vel = 200);

// Auton Methods
extern void driveViaTime(double time, double vel);
extern void logData(double leftJoy);
extern double updateMSDTime();
extern void giveInstruction();

#endif
