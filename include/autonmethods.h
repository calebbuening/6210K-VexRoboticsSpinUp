#include "main.h"
extern void eliScoreRoller();

// Returns -1, 0, and 1 based on number direction
extern int sgn(double d); // Mimimcs the mathematical sgn function

extern void turnViaIMU(double heading);

// Drive guided by IMU
extern void driveViaIMU(double dist, double heading, double vel);

// Auton Methods
extern void matchLoadDisks(double lsdTarget);
extern void driveViaTime(double time, double vel);
extern double getLSD(float rangeStart, float rangeStop);