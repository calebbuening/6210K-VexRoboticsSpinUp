#include "main.h"
#include "globals.h"

// Returns -1, 0, and 1 based on number direction
int sgn(double d) // Mimimcs the mathematical sgn function
{
	if(d < 0){return -1;}
	if(d > 0){return 1;}
	return 0;
}

// Returns a filtered x value from the gps
double averageGPSX(double ms){
	double startTime = pros::millis();
	double total;
	double count;
	while(pros::millis() - startTime < ms){
		total += gps.get_status().x;
		count++;
		pros::delay(5);
	}
	return total / count;
}

// Returns a filtered y value from the gps
double averageGPSY(double ms){
	double startTime = pros::millis();
	double total;
	double count;
	while(pros::millis() - startTime < ms){
		total += gps.get_status().y;
		count++;
		pros::delay(5);
	}
	return total / count;
}

// Returns a filtered rotation value from the gps
double averageGPSR(double ms){
	double startTime = pros::millis();
	double total;
	double count;
	while(pros::millis() - startTime < ms){
		total += gps.get_rotation();
		count++;
		pros::delay(5);
	}
	return total / count;
}

// Drive guided by IMU
void driveViaIMU(double dist, double rotation)
{
    dist *= 39.3701 / (4 * PI); // To in. then to rev
}