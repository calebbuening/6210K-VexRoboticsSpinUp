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

// Sensors
pros::IMU imu(GYRO);
pros::GPS gps(GPS_PORT, GPS_OFFSET_X, GPS_OFFSET_Y);
pros::Vision vision(ROLLER_VISION, pros::E_VISION_ZERO_CENTER);

// Variables
double startTime = 0;
bool fiveSecondWarningTriggered = false;
bool shieldReleased = false;
bool stringReleased = false;
bool clockOverride = false;
char auton = 'N';
bool initialized = false;
bool sidesReleased = false;

void initialize() {
	// Make sure all pneumatics are in the off state
	stringRelease.set_value(false);
	shieldRelease.set_value(false);
}

void disabled() {}

void autonomous(){

	if(auton == 'E'){
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
}

// This would not compile
//double average(auto v){
//	return std::accumulate(v.begin(), v.end(), 0.0) / v.length();
//}

void opcontrol() {
	if(!initialized){
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
			if(selectionIndex > 2) selectionIndex = 0;
			if(selectionIndex < 0) selectionIndex = 2;

			// Print selected auton
			switch(selectionIndex){
				case 0: master.print(1, 0, "        None        "); auton = 'N'; break;
				case 1: master.print(1, 0, "     Eli's Auton    "); auton = 'E'; break;
				case 2: master.print(1, 0, "    Sensor Auton    "); auton = 'S'; break;
			}

			// Break when done
			if(master.get_digital_new_press(DIGITAL_A)){
				autonSelected = true;
			}

			// Delay to prevent the controller from ignoring commands
			pros::delay(60);
		}
		master.clear();
		pros::delay(60);

		// Safety reminder
		master.rumble("..");
		pros::delay(60);
		master.print(0, 0, "Remove safety, then");
		pros::delay(60);
		master.print(1, 0, "press A");
		while(!master.get_digital_new_press(DIGITAL_A)) pros::delay(60);
		master.clear();

		initialized = true;
	}

	
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

		if(!clockOverride){
			// Shield launcher
			if(pros::millis() - startTime > 95000){
				shieldReleased = true;
				shieldRelease.set_value(true);
			}

			// String launchers
			if(pros::millis() - startTime > 95000 && shieldReleased && !stringReleased && master.get_digital_new_press(DIGITAL_R1)){
				stringReleased = true;
				stringRelease.set_value(true);
			}

			// Side expansion
			if(!sidesReleased && master.get_digital_new_press(DIGITAL_L1)){
				sidesReleased = true;
				sidesRelease.set_value(true);
			}

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
			if(master.get_digital_new_press(DIGITAL_R1)){
				if(!shieldReleased){
					shieldReleased = true;
					shieldRelease.set_value(true);
				}else if(!stringReleased){
					stringReleased = true;
					stringRelease.set_value(true);
				}
			}

			if(!sidesReleased && master.get_digital_new_press(DIGITAL_L1)){
				sidesReleased = true;
				sidesRelease.set_value(true);
			}
		}

		pros::delay(20);
	}
}
