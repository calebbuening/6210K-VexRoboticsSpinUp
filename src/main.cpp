#include "main.h"

// Pneumatics
pros::ADIDigitalOut stringRelease ('A');
pros::ADIDigitalOut shieldRelease ('H');

// Controllers
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Motors
pros::Motor left1(14, false);	// Rear Right outboard
pros::Motor left2(13, true);	// Rear Right inboard
pros::Motor left3(5, false);	// Front Right outboard
pros::Motor left4(3, true);		// Front Right inboard
pros::Motor right1(12, true);	// Rear Left outboard
pros::Motor right2(11, false);	// Rear Left inboard
pros::Motor right3(4, true);	// Front Left outboard
pros::Motor right4(2, false);	// Front Left inboard

// State variables
double startTime = 0;
bool fiveSecondWarningTriggered = false;
bool shieldReleased = false;
bool stringReleased = false;

void initialize() {
	// Make sure all pneumatics are in the off state
	stringRelease.set_value(false);
	shieldRelease.set_value(false);
}

void disabled() {}

void competition_initialize() {}

void autonomous(){

	pros::delay(100);
	int loop=0;
	while(loop<4){

		// Rear Right
		left1 = 50;
		left2 = 50 ;
		// Rear Left
		left3 = 50 ;
		left4 = 50 ;
		// Front Right
		right1 = 50 ; 
		right2 = 50 ; 
		// Rear Right
		right3 = 50;
		right4 = 50;
		pros::delay(500);
		
		// Rear Right		
		left1 = 0;
		left2 = 0 ;
		// Rear Left
		left3 = (0) ;
		left4 = (0) ;
		// Front Right
		right1 = 0 ; 
		right2 = 0 ; 
		// Rear Right
		right3 = 0;
		right4 = 0;
		pros::delay(500);

		// Rear Right
		left1 = -50;
		left2 = -50 ;
		// Rear Left
		left3 = -50 ;
		left4 = -50 ;
		// Front Right
		right1 = -50 ; 
		right2 = -50 ; 
		// Rear Right
		right3 = -50;
		right4 = -50;
		pros::delay(300);

		// Rear Right
		left1 = 0;
		left2 = 0 ;
		// Rear Left
		left3 = (0) ;
		left4 = (0) ;
		// Front Right
		right1 = 0 ; 
		right2 = 0 ; 
		// Rear Right
		right3 = 0;
		right4 = 0;
		pros::delay(500);

		loop=loop+1;
	}
	
	// Rear Right
	left1 = 0;
	left2 = 0 ;
	// Rear Left
	left3 = (0) ;
	left4 = (0) ;
	// Front Right
	right1 = 0 ; 
	right2 = 0 ; 
	// Rear Right
	right3 = 0;
	right4 = 0;

	pros::delay(2000);
}

// This would not compile
//double average(auto v){
//	return std::accumulate(v.begin(), v.end(), 0.0) / v.length();
//}

void opcontrol() {
	
	// Start clock for measuring the driver part of the match
	if(pros::competition::is_connected() && !pros::competition::is_autonomous() && !pros::competition::is_disabled()){
		startTime = pros::millis();
	}

	while (true) {

		// Scale joysticks down to percentages
		double leftJoy = master.get_analog(ANALOG_LEFT_Y) / 127;
		double strafeJoy = master.get_analog(ANALOG_LEFT_X) / 127;
		double rightJoy = master.get_analog(ANALOG_RIGHT_X) / 127;

		// Deadzone by 10%
		if(std::fabs(leftJoy) < .05) leftJoy = 0;
		if(std::fabs(rightJoy) < .05) rightJoy = 0;
		if(std::fabs(strafeJoy) < .05) strafeJoy = 0;

		// Drag adjustment for crappy motors
		double actualFrontDiffPct = 0; //(average(frontLeft.get_actual_velocity()) - average(frontRight.get_actual_velocity())) / 400;
		double frontTurnAdj = 0; //actualFrontDiffPct - rightJoy;
		double actualBackDiffPct = 0; //(average(backLeft.get_actual_velocity()) - average(backRight.get_actual_velocity())) / 400;
		double backTurnAdj = 0; //actualBackDiffPct - rightJoy;
		
		// Assign speeds after scaling them back to 100 //

		// Front Left
		left1 = (leftJoy - rightJoy - frontTurnAdj + strafeJoy) * 127;
		left2 = (leftJoy - rightJoy - frontTurnAdj + strafeJoy) * 127;

		// Rear Left
		left3 = (leftJoy - rightJoy - backTurnAdj - strafeJoy) * 127;
		left4 = (leftJoy - rightJoy - backTurnAdj - strafeJoy) * 127;

		// Front Right
		right1 = (leftJoy + rightJoy + frontTurnAdj - strafeJoy) * 127; 
		right2 = (leftJoy + rightJoy + frontTurnAdj - strafeJoy) * 127; 

		// Rear Right
		right3 = (leftJoy + rightJoy + backTurnAdj + strafeJoy) * 127;
		right4 = (leftJoy + rightJoy + backTurnAdj + strafeJoy) * 127;

		// Shield launcher
		if(pros::millis() - startTime < 95000){
			shieldReleased = true;
			shieldRelease.set_value(true);
		}

		// String launchers
		if(shieldReleased && !stringReleased && master.get_digital_new_press(DIGITAL_R1)){
			stringReleased = true;
			stringRelease.set_value(true);
		}

		// 5 second warning for endgame stuff
		if(!fiveSecondWarningTriggered && pros::millis() - startTime > 100000){
			fiveSecondWarningTriggered = true;
			master.rumble("---");
		}

		pros::delay(20);
	}
}
