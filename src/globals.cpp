#include "main.h"

// Port definitions
// Motors
#define BRO 10
#define BRI 9
#define FRO 7
#define FRI 8

#define BLO 1
#define BLI 2
#define FLO 20
#define FLI 6

#define STRING 'H'
#define CATA 'A'
#define SHIELD 'G'

#define GYRO 16
#define GPS_PORT 11
#define GPS_OFFSET_X 0
#define GPS_OFFSET_Y 0

extern double startTime = 0;
extern bool fiveSecondWarningTriggered = false;
extern bool stringReleased = false;
extern bool clockOverride = false;
extern char auton = 'N';
extern bool initialized = false;
extern bool catapultState = false;
extern bool shieldReleased = false;

// Pneumatics
pros::ADIDigitalOut stringRelease(STRING);
pros::ADIDigitalOut catapultRelease(CATA);
pros::ADIDigitalOut shieldRelease(SHIELD);

// Controllers
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Motors
pros::Motor mBRO(BRO, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_ROTATIONS);	// Rear Right outboard
pros::Motor mBRI(BRI, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_ROTATIONS);	// Rear Right inboard
pros::Motor mFRO(FRO, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_ROTATIONS);	// Front Right outboard
pros::Motor mFRI(FRI, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_ROTATIONS);		// Front Right inboard
pros::Motor mBLO(BLO, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_ROTATIONS);	// Rear Left outboard
pros::Motor mBLI(BLI, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_ROTATIONS);	// Rear Left inboard
pros::Motor mFLO(FLO, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_ROTATIONS);	// Front Left outboard
pros::Motor mFLI(FLI, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_ROTATIONS);	// Front Left inboard

// Sensors
pros::IMU imu(GYRO);
pros::GPS gps(GPS_PORT, GPS_OFFSET_X, GPS_OFFSET_Y);