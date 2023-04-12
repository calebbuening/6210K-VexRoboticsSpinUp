#include "main.h"

#ifndef _AUTONMETHODS_H_
#define _AUTONMETHODS_H_
extern void eliScoreRoller();

// Returns -1, 0, and 1 based on number direction
extern int sgn(double d); // Mimimcs the mathematical sgn function

extern void turnViaIMU(double heading);

// Drive guided by IMU
extern void driveViaIMU(double dist, double heading, double vel = 200);

// Auton Methods
extern void matchLoadDisks(double lsdTarget);
extern void driveViaTime(double time, double vel);
extern void logData();
extern double getLSD(double rangeStart, double rangeStop);
extern void flashScreen();
extern void updateLSDTime();
extern void giveInstruction();

#endif
