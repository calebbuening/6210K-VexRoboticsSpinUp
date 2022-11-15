#include "main.h"

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

#define GYRO 7
#define GPS_PORT 8
#define ROLLER_VISION 9

#define PI 3.14159265358979323846

extern pros::Motor left1;
extern pros::Motor left2;
extern pros::Motor left3;
extern pros::Motor left4;
extern pros::Motor right1;
extern pros::Motor right2;
extern pros::Motor right3;
extern pros::Motor right4;

extern pros::MotorGroup left;
extern pros::MotorGroup right;

// Uncomment when motor groups start compiling
// extern okapi::MotorGroup rightMotor;
// extern okapi::MotorGroup leftMotor;

// Controllers
extern pros::Controller master;

// Sensors
extern pros::Vision vision;
extern pros::Imu imu;
extern pros::GPS gps;

// Pneumatics
extern pros::ADIDigitalOut stringRelease;
extern pros::ADIDigitalOut shieldRelease;