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
	left.tare_position();
	right.tare_position();

	double d = (left.get_position() + right.get_position()) / 2;

	if(sgn(dist) > 0){
		while(std::fabs(dist-d) > 0.1){
			d = (left.get_position() + right.get_position()) / 2;
			double error = (imu.get_rotation() - rotation)* 4.5;
			left.move_velocity(200 + error);
			right.move_velocity(200 - error);
			pros::delay(5);
		}
	}else{
		while(std::fabs(dist-d) > 0.1){
			d = (left.get_position() + right.get_position()) / 2;
			double error = (imu.get_rotation() - rotation)* 4.5;
			left.move_velocity(-200 + error);
			right.move_velocity(-200 - error);
			pros::delay(5);

		}
	}
	left.move_velocity(0);
	right.move_velocity(0);
}

void driveViaTime(double ms, double vel){
	left.move_velocity(vel);
	right.move_velocity(vel);
	pros::delay(ms);
	left.move_velocity(0);
	right.move_velocity(0);
}

void turnViaIMU(double heading){
	double error = heading - imu.get_rotation();
	int rotation;
	while(std::fabs(error) > 1)
	{
		if(std::fabs(error) < 30){
			rotation = (6 * error);
		}else{
			rotation = 200 * sgn(error);
		}

		left.move_velocity(rotation);
		right.move_velocity(-rotation);
		error = heading - imu.get_rotation();
		pros::delay(5);
	}
	rotation = -30 * sgn(error);
	left.move_velocity(rotation);
	right.move_velocity(-rotation);
	pros::delay(50);
	left.move_velocity(0);
	right.move_velocity(0);
}

// experimental distance strafe, also consider adding line sensor
void strafeViaTime(char direction, double ms, double vel){
	if(direction == 'L'){
		left1.move_velocity(-vel);
		left2.move_velocity(-vel);
		left3.move_velocity(vel);
		left4.move_velocity(vel);
		right1.move_velocity(vel);
		right2.move_velocity(vel);
		right3.move_velocity(-vel);
		right4.move_velocity(-vel);
		pros::delay(ms);
		left.move_velocity(0);
		right.move_velocity(0);
	}else if(direction == 'R'){
		left1.move_velocity(vel);
		left2.move_velocity(vel);
		left3.move_velocity(-vel);
		left4.move_velocity(-vel);
		right1.move_velocity(-vel);
		right2.move_velocity(-vel);
		right3.move_velocity(vel);
		right4.move_velocity(vel);
		pros::delay(ms);
		left.move_velocity(0);
		right.move_velocity(0);
	}	
}

// Drive guided by camera for rollers