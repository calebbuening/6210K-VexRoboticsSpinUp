#include "main.h"

// Port definitions
// Motors
#define LEFT_1 14
#define LEFT_2 13
#define LEFT_3 5
#define LEFT_4 3

#define RIGHT_1 12
#define RIGHT_2 11
#define RIGHT_3 4
#define RIGHT_4 2

#define STRING 'A'
#define SHIELD 'H'
#define SIDES 'B'

#define GYRO 7
#define GPS_PORT 8
#define GPS_OFFSET_X 0
#define GPS_OFFSET_Y 0
#define ROLLER_VISION 9

// Pneumatics
pros::ADIDigitalOut stringRelease(STRING);
pros::ADIDigitalOut shieldRelease(SHIELD);
pros::ADIDigitalOut sidesRelease(SIDES);

// Controllers
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Motors
pros::Motor left1(LEFT_1, false);	// Rear Right outboard
pros::Motor left2(LEFT_2, true);	// Rear Right inboard
pros::Motor left3(LEFT_3, false);	// Front Right outboard
pros::Motor left4(LEFT_4, true);		// Front Right inboard
pros::Motor right1(RIGHT_1, true);	// Rear Left outboard
pros::Motor right2(RIGHT_2, false);	// Rear Left inboard
pros::Motor right3(RIGHT_3, true);	// Front Left outboard
pros::Motor right4(RIGHT_4, false);	// Front Left inboard

// Motor groups
pros::MotorGroup left({left1, left2, left3, left4});
pros::MotorGroup right({right1, right2, right3, right4});

// Sensors
pros::IMU imu(GYRO);
pros::GPS gps(GPS_PORT, GPS_OFFSET_X, GPS_OFFSET_Y);
pros::Vision vision(ROLLER_VISION, pros::E_VISION_ZERO_CENTER);