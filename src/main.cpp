#include "main.h"

void initialize() {
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

double average(auto v){
	return std::accumulate(v.begin(), v.end(), 0.0) / v.length();
}

void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	pros::Motor left1(4, true);
	pros::Motor left2(3);
	pros::Motor left3(2);
	pros::Motor left4(19, true);

	pros::Motor right1(7, false);
	pros::Motor right2(9, true);
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

		// Drag adjustment for crappy motors
		double actualFrontDiffPct = (average(frontLeft.get_actual_velocity()) - average(frontRight.get_actual_velocity())) / 400;
		double frontTurnAdj = actualFrontDiffPct - rightJoy;
		double actualBackDiffPct = (average(backLeft.get_actual_velocity()) - average(backRight.get_actual_velocity())) / 400;
		double backTurnAdj = actualBackDiffPct - rightJoy;
		
		// Assign speeds after scaling them back to 100
		frontLeft = (leftJoy + rightJoy - frontTurnAdj + strafeJoy) * 127;
		backLeft = (leftJoy + rightJoy - backTurnAdj - strafeJoy) * 127;
		frontRight = (leftJoy - rightJoy + frontTurnAdj - strafeJoy) * 127; 
		backRight = (leftJoy - rightJoy + backTurnAdj - strafeJoy) * 127;

		pros::delay(20);
	}
}