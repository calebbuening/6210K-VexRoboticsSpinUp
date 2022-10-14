#include "main.h"

void initialize() {
}

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
void autonomous() {}

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
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	pros::Motor left1(4, true);
	pros::Motor left2(5);
	pros::Motor left3(2);
	pros::Motor left4(19, true);

	pros::Motor right1(7, false);
	pros::Motor right2(8, true);
	pros::Motor right3(20, true);
	pros::Motor right4(11, false);

	pros::Motor_Group frontLeft ({left1, left2});
	pros::Motor_Group backLeft ({left3, left4});
	pros::Motor_Group frontRight ({right1, right2});
	pros::Motor_Group backRight ({right3, right4});

	while (true) {
		double leftJoy = master.get_analog(ANALOG_LEFT_Y) / 127;
		double strafeJoy = master.get_analog(ANALOG_LEFT_X) / 127;
		double rightJoy = master.get_analog(ANALOG_RIGHT_X) / 127;

		if(std::fabs(leftJoy) < .1) leftJoy = 0;
		if(std::fabs(rightJoy) < .1) rightJoy = 0;
		if(std::fabs(strafeJoy) < .1) strafeJoy = 0;
		
		frontLeft = (leftJoy + rightJoy + strafeJoy) * 127;
		backLeft = (leftJoy + rightJoy - strafeJoy) * 127;
		frontRight = (leftJoy - rightJoy - strafeJoy) * 127; 
		backRight = (leftJoy - rightJoy + strafeJoy) * 127;

		pros::delay(20);
	}
}
