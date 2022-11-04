#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "6210K Mechanum Drive");
	pros::ADIDigitalOut piston ('A');
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

	pros::ADIDigitalOut piston ('A');
	pros::ADIDigitalOut piston2 ('H');

	pros::Motor left1(14, false);	// Rear Right outboard
	pros::Motor left2(13, true);	// Rear Right inboard
	pros::Motor left3(5, false);	// Front Right outboard
	pros::Motor left4(3, true);		// Front Right inboard

	pros::Motor right1(12, true);	// Rear Left outboard
	pros::Motor right2(11, false);	// Rear Left inboard
	pros::Motor right3(4, true);	// Front Left outboard
	pros::Motor right4(2, false);	// Front Left inboard

	piston.set_value(false);

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

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

// This would not compile
//double average(auto v){
//	return std::accumulate(v.begin(), v.end(), 0.0) / v.length();
//}

void opcontrol() {
	pros::ADIDigitalOut piston ('A');
	pros::ADIDigitalOut piston2 ('H');
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	pros::Motor left1(14, false);	// Rear Right outboard
	pros::Motor left2(13, true);	// Rear Right inboard
	pros::Motor left3(5, false);	// Front Right outboard
	pros::Motor left4(3, true);		// Front Right inboard

	pros::Motor right1(12, true);	// Rear Left outboard
	pros::Motor right2(11, false);	// Rear Left inboard
	pros::Motor right3(4, true);	// Front Left outboard
	pros::Motor right4(2, false);	// Front Left inboard

	piston.set_value(false);

	while (true) {
		double leftJoy = master.get_analog(ANALOG_LEFT_Y) / 127;
		double strafeJoy = master.get_analog(ANALOG_LEFT_X) / 127;
		double rightJoy = master.get_analog(ANALOG_RIGHT_X) / 127;

		if(std::fabs(leftJoy) < .05) leftJoy = 0;
		if(std::fabs(rightJoy) < .05) rightJoy = 0;
		if(std::fabs(strafeJoy) < .05) strafeJoy = 0;

		// Drag adjustment for crappy motors
		double actualFrontDiffPct = 0; //(average(frontLeft.get_actual_velocity()) - average(frontRight.get_actual_velocity())) / 400;
		double frontTurnAdj = 0; //actualFrontDiffPct - rightJoy;
		double actualBackDiffPct = 0; //(average(backLeft.get_actual_velocity()) - average(backRight.get_actual_velocity())) / 400;
		double backTurnAdj = 0; //actualBackDiffPct - rightJoy;
		
		// Assign speeds after scaling them back to 100
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

    	if (master.get_digital(DIGITAL_R1)) {
      		piston.set_value(true);
    	}
	if (master.get_digital(DIGITAL_R2)) {
		piston2.set_value(true);
	}

	pros::delay(20);
	}
}
