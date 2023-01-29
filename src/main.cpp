#include "main.h"
#include "globals.h"

// Variables
double startTime = 0;
bool fiveSecondWarningTriggered = false;
bool stringReleased = false;
bool clockOverride = false;
char auton = 'N';
bool initialized = false;
bool catapultState = false;
bool shieldReleased = false;

void initialize() {
	// Make sure all pneumatics are in the off state
	stringRelease.set_value(false);
	catapultRelease.set_value(true);
 	shieldRelease.set_value(false);
}

void disabled() {}

void eliScoreRoller(){
	int loop=0;
		while(loop<3){

			// Rear Right
			mBRO = 50;
			mBRI = 50 ;
			// Rear Left
			mFRO = 50 ;
			mFRI = 50 ;
			// Front Right
			mBLO = 50 ; 
			mBLI = 50 ; 
			// Rear Right
			mFLO = 50;
			mFLI = 50;
			pros::delay(750);
			
			// Rear Right		
			mBRO = 0;
			mBRI = 0 ;
			// Rear Left
			mFRO = (0) ;
			mFRI = (0) ;
			// Front Right
			mBLO = 0 ; 
			mBLI = 0 ; 
			// Rear Right
			mFLO = 0;
			mFLI = 0;
			pros::delay(500);

			// Rear Right
			mBRO = -50;
			mBRI = -50 ;
			// Rear Left
			mFRO = -50 ;
			mFRI = -50 ;
			// Front Right
			mBLO = -50 ; 
			mBLI = -50 ; 
			// Rear Right
			mFLO = -50;
			mFLI = -50;
			pros::delay(150);

			// Rear Right
			mBRO = 0;
			mBRI = 0 ;
			// Rear Left
			mFRO = (0) ;
			mFRI = (0) ;
			// Front Right
			mBLO = 0 ; 
			mBLI = 0 ; 
			// Rear Right
			mFLO = 0;
			mFLI = 0;
			pros::delay(500);

			loop=loop+1;
		}
		
		// Rear Right
		mBRO = 0;
		mBRI = 0 ;
		// Rear Left
		mFRO = (0) ;
		mFRI = (0) ;
		// Front Right
		mBLO = 0 ; 
		mBLI = 0 ; 
		// Rear Right
		mFLO = 0;
		mFLI = 0;

		pros::delay(500);
}

// Returns -1, 0, and 1 based on number direction
int sgn(double d) // Mimimcs the mathematical sgn function
{
	if(d < 0){return -1;}
	if(d > 0){return 1;}
	return 0;
}

void turnViaIMU(double heading){
	double error = heading - imu.get_rotation();
	int rotation;
	while(std::fabs(error) > .5)
	{
		if(std::fabs(error) < 30){
			rotation = -(6 * error); // Was 6
		}else{
			rotation = -200 * sgn(error); // was 200
		}

		mBRO.move_velocity(rotation);
		mBRI.move_velocity(rotation);
		mFRO.move_velocity(rotation);
		mFRI.move_velocity(rotation);
		mBLO.move_velocity(-rotation);
		mBLI.move_velocity(-rotation);
		mFLO.move_velocity(-rotation);
		mFLI.move_velocity(-rotation);
		error = heading - imu.get_rotation();
		pros::delay(5);
	}
	rotation = 120 * sgn(error); // was 30
	mBRO.move_velocity(rotation);
	mBRI.move_velocity(rotation);
	mFRO.move_velocity(rotation);
	mFRI.move_velocity(rotation);
	mBLO.move_velocity(-rotation);
	mBLI.move_velocity(-rotation);
	mFLO.move_velocity(-rotation);
	mFLI.move_velocity(-rotation);
	pros::delay(50);
	mBRO.move_velocity(0);
	mBRI.move_velocity(0);
	mFRO.move_velocity(0);
	mFRI.move_velocity(0);
	mBLO.move_velocity(0);
	mBLI.move_velocity(0);
	mFLO.move_velocity(0);
	mFLI.move_velocity(0);
}

// Drive guided by IMU
void driveViaDist(double dist)
{
	// To in. then to rev, then to square 39.3701 instead of 24 for meters
	double pos = (mBRO.get_position() + mBLO.get_position())/2;
	mBRO.move_relative(dist, 200);
	mBRI.move_relative(dist, 200);
	mFRO.move_relative(dist, 200);
	mFRI.move_relative(dist, 200);
	mBLO.move_relative(dist, 200);
	mBLI.move_relative(dist, 200);
	mFLO.move_relative(dist, 200);
	mFLI.move_relative(dist, 200);
	while (!((((mBRO.get_position() + mBLO.get_position())/2) < (pos + dist) + .1) && (((mBRO.get_position() + mBLO.get_position())/2) > (pos + dist) - .1))) {
		pros::delay(1000);
	}
}

void driveViaIMU(double dist, double heading){
	mBRI.tare_position();
	mBLI.tare_position();
	double pos = (mBRI.get_position() + mBLI.get_position())/2;
	dist += pos;
	if(dist > pos){
		while(pos < dist){
			double error = heading - imu.get_rotation();
			int rotation;
			if(std::fabs(error) < 15){ // Was 30
				rotation = (12 * error); // Was 6
			}else{
				rotation = 200 * sgn(error); // was 200
			}
			mBRO.move_velocity(200 - rotation);
			mBRI.move_velocity(200 - rotation);
			mFRO.move_velocity(200 - rotation);
			mFRI.move_velocity(200 - rotation);
			mBLO.move_velocity(200 + rotation);
			mBLI.move_velocity(200 + rotation);
			mFLO.move_velocity(200 + rotation);
			mFLI.move_velocity(200 + rotation);
			pos = (mBRI.get_position() + mBLI.get_position())/2;
			pros::delay(5);
		}
	}else{
		while(pos > dist){
			double error = heading - imu.get_rotation();
			int rotation;
			if(std::fabs(error) < 30){
				rotation = (6 * error); // Was 6
			}else{
				rotation = 200 * sgn(error); // was 200
			}
			mBRO.move_velocity(-200 - rotation);
			mBRI.move_velocity(-200 - rotation);
			mFRO.move_velocity(-200 - rotation);
			mFRI.move_velocity(-200 - rotation);
			mBLO.move_velocity(-200 + rotation);
			mBLI.move_velocity(-200 + rotation);
			mFLO.move_velocity(-200 + rotation);
			mFLI.move_velocity(-200 + rotation);
			pos = (mBRI.get_position() + mBLI.get_position())/2;
			pros::delay(5);
		}
	}
	mBRO.move_velocity(0);
	mBRI.move_velocity(0);
	mFRO.move_velocity(0);
	mFRI.move_velocity(0);
	mBLO.move_velocity(0);
	mBLI.move_velocity(0);
	mFLO.move_velocity(0);
	mFLI.move_velocity(0);
}

void autonomous(){

	// THE LEAGUE AUTON
	if(auton == 'S' || auton == 'Z'){
		// Score the first roller
		int autonAdjust = 1;
		if(auton=='Z'){
			autonAdjust = -1;
			driveViaIMU(3, 0);
			turnViaIMU(90);
		}
		imu.tare();

		int loop = 0;
		while(loop < 2){
			// Rear Right
			mBRO = 50;
			mBRI = 50 ;
			// Rear Left
			mFRO = 50 ;
			mFRI = 50 ;
			// Front Right
			mBLO = 50 ; 
			mBLI = 50 ; 
			// Rear Right
			mFLO = 50;
			mFLI = 50;
			pros::delay(400);
			
			// Rear Right		
			mBRO = 0;
			mBRI = 0 ;
			// Rear Left
			mFRO = (0) ;
			mFRI = (0) ;
			// Front Right
			mBLO = 0 ; 
			mBLI = 0 ; 
			// Rear Right
			mFLO = 0;
			mFLI = 0;
			pros::delay(400);

			// Rear Right
			mBRO = -50;
			mBRI = -50 ;
			// Rear Left
			mFRO = -50 ;
			mFRI = -50 ;
			// Front Right
			mBLO = -50 ; 
			mBLI = -50 ; 
			// Rear Right
			mFLO = -50;
			mFLI = -50;
			pros::delay(150);

			// Rear Right
			mBRO = 0;
			mBRI = 0 ;
			// Rear Left
			mFRO = (0) ;
			mFRI = (0) ;
			// Front Right
			mBLO = 0 ; 
			mBLI = 0 ; 
			// Rear Right
			mFLO = 0;
			mFLI = 0;
			pros::delay(400);

			loop=loop+1;
		}
		
		// Rear Right
		mBRO = 0;
		mBRI = 0 ;
		// Rear Left
		mFRO = (0) ;
		mFRI = (0) ;
		// Front Right
		mBLO = 0 ; 
		mBLI = 0 ; 
		// Rear Right
		mFLO = 0;
		mFLI = 0;
		
		// Turn 45 degrees
		turnViaIMU(45 * autonAdjust);

		// Back up 8 feet
		driveViaIMU(-7.4, 45 * autonAdjust);

		// Turn towards the goal
		turnViaIMU(142 * autonAdjust); // was 141.5
		
		// Delay to make sure the discs are at rest
		pros::delay(1000);

		// Fire the catapult
		catapultRelease.set_value(true);
	}	

	if(auton == 'A'){
		// Score the first roller
		eliScoreRoller();

		// Score the second roller
		driveViaIMU(-2.3, 0);
		turnViaIMU(90);
		driveViaIMU(.5, 90);
		eliScoreRoller();

		// Cross the field and turn towards the next roller
		driveViaIMU(-3.3, 90); // -2.5, -2.8
		turnViaIMU(225);
		driveViaIMU(17.3, 225);
		turnViaIMU(180);

		// Score the third roller
		driveViaIMU(1.5, 180); // was 2.1
		eliScoreRoller();

		// Score the fourth roller
		driveViaIMU(-2.5, 180); // 2.25
		turnViaIMU(270);
		driveViaIMU(2.25, 270);
		eliScoreRoller();

		// Turn and launch string
		driveViaIMU(-2.5, 270);
		turnViaIMU(225);
		driveViaIMU(1.25, 225);

		// Endgame
		stringRelease.set_value(true);

		pros::delay(1000);
		
		// Get some string slack
		driveViaIMU(1, 225);
		driveViaIMU(-.5, 225);
	}

	if(auton == 'L'){
		int loop=0;
		while(loop<2){

			// Rear Right
			mBRO = 50;
			mBRI = 50 ;
			// Rear Left
			mFRO = 50 ;
			mFRI = 50 ;
			// Front Right
			mBLO = 50 ; 
			mBLI = 50 ; 
			// Rear Right
			mFLO = 50;
			mFLI = 50;
			pros::delay(500);
			
			// Rear Right		
			mBRO = 0;
			mBRI = 0 ;
			// Rear Left
			mFRO = (0) ;
			mFRI = (0) ;
			// Front Right
			mBLO = 0 ; 
			mBLI = 0 ; 
			// Rear Right
			mFLO = 0;
			mFLI = 0;
			pros::delay(500);

			// Rear Right
			mBRO = -50;
			mBRI = -50 ;
			// Rear Left
			mFRO = -50 ;
			mFRI = -50 ;
			// Front Right
			mBLO = -50 ; 
			mBLI = -50 ; 
			// Rear Right
			mFLO = -50;
			mFLI = -50;
			pros::delay(150);

			// Rear Right
			mBRO = 0;
			mBRI = 0 ;
			// Rear Left
			mFRO = (0) ;
			mFRI = (0) ;
			// Front Right
			mBLO = 0 ; 
			mBLI = 0 ; 
			// Rear Right
			mFLO = 0;
			mFLI = 0;
			pros::delay(500);

			loop=loop+1;
		}
		
		// Rear Right
		mBRO = 0;
		mBRI = 0 ;
		// Rear Left
		mFRO = (0) ;
		mFRI = (0) ;
		// Front Right
		mBLO = 0 ; 
		mBLI = 0 ; 
		// Rear Right
		mFLO = 0;
		mFLI = 0;

		pros::delay(2000);
	}

	if(auton == 'R'){
		int loop=0;
		driveViaIMU(3, 0);
		turnViaIMU(90);
		while(loop<2){

			// Rear Right
			mBRO = 50;
			mBRI = 50 ;
			// Rear Left
			mFRO = 50 ;
			mFRI = 50 ;
			// Front Right
			mBLO = 50 ; 
			mBLI = 50 ; 
			// Rear Right
			mFLO = 50;
			mFLI = 50;
			pros::delay(500);
			
			// Rear Right		
			mBRO = 0;
			mBRI = 0 ;
			// Rear Left
			mFRO = (0) ;
			mFRI = (0) ;
			// Front Right
			mBLO = 0 ; 
			mBLI = 0 ; 
			// Rear Right
			mFLO = 0;
			mFLI = 0;
			pros::delay(500);

			// Rear Right
			mBRO = -50;
			mBRI = -50 ;
			// Rear Left
			mFRO = -50 ;
			mFRI = -50 ;
			// Front Right
			mBLO = -50 ; 
			mBLI = -50 ; 
			// Rear Right
			mFLO = -50;
			mFLI = -50;
			pros::delay(150);

			// Rear Right
			mBRO = 0;
			mBRI = 0 ;
			// Rear Left
			mFRO = (0) ;
			mFRI = (0) ;
			// Front Right
			mBLO = 0 ; 
			mBLI = 0 ; 
			// Rear Right
			mFLO = 0;
			mFLI = 0;
			pros::delay(500);

			loop=loop+1;
		}
		
		// Rear Right
		mBRO = 0;
		mBRI = 0 ;
		// Rear Left
		mFRO = (0) ;
		mFRI = (0) ;
		// Front Right
		mBLO = 0 ; 
		mBLI = 0 ; 
		// Rear Right
		mFLO = 0;
		mFLI = 0;

		pros::delay(2000);
	}
}

double average(std::vector<pros::Motor> &v){
	double sum = 0;
	for(pros::Motor m : v){
		sum += m.get_actual_velocity();
	}
	return sum / v.size();
}

void opcontrol() {
	if(!initialized){
		
		// Calibrate the IMU
		master.print(0, 0, "Calibrating IMU...");
		pros::delay(60);
		imu.tare();
		imu.reset();
		while(imu.is_calibrating()) pros::delay(60);
		master.clear();
		pros::delay(60);

		master.print(0, 0, "Press A once");
		pros::delay(60);
		master.print(1, 0, "pneumatics are ready");
		while(!master.get_digital_new_press(DIGITAL_A)) pros::delay(60);
		catapultRelease.set_value(false);
		master.clear();
		pros::delay(60);
		master.print(0, 0, "Select an auton:");
		pros::delay(60);
		bool autonSelected = false;
		int selectionIndex = 0;
		while(!autonSelected){
			// Adjust which auton program we have selected
			if(master.get_digital_new_press(DIGITAL_RIGHT)) selectionIndex++;
			if(master.get_digital_new_press(DIGITAL_LEFT)) selectionIndex--;
			if(selectionIndex > 5) selectionIndex = 0;
			if(selectionIndex < 0) selectionIndex = 5;

			// Print selected auton
			switch(selectionIndex){
				case 0: master.print(1, 0, "        None        "); auton = 'N'; break;
				case 1: master.print(1, 0, "     Full Auton     "); auton = 'A'; break;
				case 2: master.print(1, 0, "  Right Side Auton  "); auton = 'R'; break;
				case 3: master.print(1, 0, "   Left Side Auton  "); auton = 'L'; break;
				case 4: master.print(1, 0, "     League Left    "); auton = 'S'; break;
				case 5: master.print(1, 0, "     League Right   "); auton = 'Z'; break;
			}

			// Break when done
			if(master.get_digital_new_press(DIGITAL_A)){
				autonSelected = true;
			}else if(master.get_digital_new_press(DIGITAL_X)){
				autonomous();
			}

			// Delay to prevent the controller from ignoring commands
			pros::delay(60);
		}
		master.clear();

		initialized = true;
	}

	
	// Start clock for measuring the driver part of the match
	if(pros::competition::is_connected() && !pros::competition::is_autonomous() && !pros::competition::is_disabled()){
		startTime = pros::millis();
	}

	while (true) {

		if(master.get_digital_new_press(DIGITAL_A)){
			catapultState = !catapultState;
			catapultRelease.set_value(catapultState);
		}

		// Scale joysticks down to percentages
		double leftJoy = master.get_analog(ANALOG_LEFT_Y) / 127;
		double strafeJoy = master.get_analog(ANALOG_LEFT_X) / 127;
		double rightJoy = master.get_analog(ANALOG_RIGHT_X) / 127;

		// Deadzone by 10%
		if(std::fabs(leftJoy) < .05) leftJoy = 0;
		if(std::fabs(rightJoy) < .05) rightJoy = 0;
		if(std::fabs(strafeJoy) < .05) strafeJoy = 0;

		// Drag adjustment for crappy motors
		double actualFrontDiffPct = 0; // (average(frontLeft.get_actual_velocity()) - average(frontRight.get_actual_velocity())) / 400;
		double frontTurnAdj = 0; // actualFrontDiffPct - rightJoy;
		double actualBackDiffPct = 0; // (average(backLeft.get_actual_velocity()) - average(backRight.get_actual_velocity())) / 400;
		double backTurnAdj = 0; // actualBackDiffPct - rightJoy;
		
		// Assign speeds after scaling them back to 100 //

		double speedMultiplier = 1;

		if(master.get_digital(DIGITAL_L1)) speedMultiplier = .5;

		// Front Left
		mBRO = (leftJoy - rightJoy - frontTurnAdj + strafeJoy) * 127 * speedMultiplier;
		mBRI = (leftJoy - rightJoy - frontTurnAdj + strafeJoy) * 127 * speedMultiplier;

		// Rear Left
		mFRO = (leftJoy - rightJoy - backTurnAdj - strafeJoy) * 127 * speedMultiplier;
		mFRI = (leftJoy - rightJoy - backTurnAdj - strafeJoy) * 127	* speedMultiplier;

		// Front Right
		mBLO = (leftJoy + rightJoy + frontTurnAdj - strafeJoy) * 127 * speedMultiplier;
		mBLI = (leftJoy + rightJoy + frontTurnAdj - strafeJoy) * 127 * speedMultiplier;

		// Rear Right
		mFLO = (leftJoy + rightJoy + backTurnAdj + strafeJoy) * 127 * speedMultiplier;
		mFLI = (leftJoy + rightJoy + backTurnAdj + strafeJoy) * 127 * speedMultiplier;

		if(!clockOverride){

			// String launchers
			if(pros::millis() - startTime > 95000 && !stringReleased && master.get_digital_new_press(DIGITAL_R1)){
				stringReleased = true;
				stringRelease.set_value(true);
			}

			// Auto string launchers
			if(pros::millis() - startTime > 104000 && !stringReleased){
				stringReleased = true;
				stringRelease.set_value(true);
			}

			// // Shield launcher
			// if(pros::millis() - startTime > 95000){
			// 	shieldReleased = true;
			// 	shieldRelease.set_value(true);
			// }

			// 5 second warning for endgame stuff
			if(!fiveSecondWarningTriggered && pros::millis() - startTime > 100000){
				fiveSecondWarningTriggered = true;
				master.rumble("---");
			}

			// Clock functions override
			if(master.get_digital_new_press(DIGITAL_B)){
				clockOverride = true;
				master.rumble(".");
			}
		}else{
			// Manual string launcher
			if(master.get_digital_new_press(DIGITAL_R1)){
				if(!stringReleased){
					stringReleased = true;
					stringRelease.set_value(true);
				}
			}

			// Manual shield launcher
			if(!shieldReleased && master.get_digital_new_press(DIGITAL_UP)){
					shieldReleased = true;
					shieldRelease.set_value(true);
			}
		}

		pros::delay(20);
	}
}
