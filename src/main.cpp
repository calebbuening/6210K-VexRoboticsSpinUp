#include "main.h"
#include "globals.h"
#include "auton.h"

void initialize() {
	// Make sure all pneumatics are in the off state
	stringRelease.set_value(false);
	catapultRelease.set_value(true);
 	shieldRelease.set_value(false);
}

void disabled() {}

double average(std::vector<pros::Motor> &v){
	double sum = 0;
	for(pros::Motor m : v){
		sum += m.get_actual_velocity();
	}
	return sum / v.size();
}

void opcontrol() {


	lv_obj_t * label1 =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label1, "Every time a bell rings, a robot gets its wings...");
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

	if(!initialized){
		
		// Calibrate the IMU
		master.print(0, 0, "Calibrating IMU...");
		pros::delay(60);
		imu.tare();
		imu.reset();
		while(imu.is_calibrating()) pros::delay(60);
		master.clear();

		pros::delay(60);

		// Make sure the catapult holds
		mCATA.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

		// Lock in the catapult pneumatic
		master.print(0, 0, "Press A once");
		pros::delay(60);
		master.print(1, 0, "Pneumatics are ready");
		while(!master.get_digital_new_press(DIGITAL_A)) pros::delay(60);
		catapultRelease.set_value(false);
		pros::delay(60);

		// Set current position to 0, then load catapult
		mCATA.tare_position();
		while (mCATA.get_position() < 3.6){ 
			mCATA = 127;
		}
		mCATA.brake();
		mCATA = 0;
		master.clear();

		// Select mode
		pros::delay(60);
		master.print(0,0, "Select a mode:");
		pros::delay(60);
		bool typeSelected = false;
		int selectionIndex = 0;
		while(!typeSelected){
			// Adjust which type we have selected
			if(master.get_digital_new_press(DIGITAL_RIGHT)) selectionIndex++;
			if(master.get_digital_new_press(DIGITAL_LEFT)) selectionIndex--;
			if(selectionIndex > 3) selectionIndex = 0;
			if(selectionIndex < 0) selectionIndex = 3;

			// Print selected type
			switch(selectionIndex){
				case 0: master.print(1, 0, "     Competition    "); master.print(2, 0, "    mBRO enabled    "); skills = false; mBROState = true; break;
				case 1: master.print(1, 0, "        Skills      "); master.print(2, 0, "    mBRO enabled    "); skills = true; mBROState = true; break;
				case 2: master.print(1, 0, "        Skills*     "); master.print(2, 0, "    Re-tensioner    "); skills = true; mBROState = false; break;
				case 3: master.print(1, 0, "     Competition*   "); master.print(2, 0, "    Re-tensioner    "); skills = false; mBROState = false; break;
			}
			
			if(master.get_digital_new_press(DIGITAL_A)){
				typeSelected = true;

			pros::delay(60);
			}
		}
		master.clear();

		// If we are in catapult mode, make sure the catapult is in the right port
		if(!mBROState){
			pros::delay(60);
			master.print(1, 0, " Ensure mCata in 17 ");
			while(!master.get_digital_new_press(DIGITAL_A)) pros::delay(60);
		}
		master.clear();

		// Select auton type
		selectionIndex = 0;
		pros::delay(60);
		master.print(0, 0, "Select an auton:");
		pros::delay(60);
		bool autonSelected = false;
		selectionIndex = 0;
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

	int loopCounter = 0;
	bool masterDot;
	bool masterDash;
	bool partnerDot;
	bool partnerDash;

	while (true) {

		// Fire catapult
		if(master.get_digital_new_press(DIGITAL_A)){
			catapultState = true;
			catapultRelease.set_value(catapultState);
		
		}

		// If using the catapult
		if(!mBROState){
			// If not at the loaded position, get there
			if (mCATA.get_position() > -.1 && catapultState){
				mCATA = -127;
			}

			// If at the loaded position and we have fired, reload the pneumatic
			if (mCATA.get_position() <= 0 && catapultState){
				catapultState = false;
				catapultRelease.set_value(catapultState);
			}
			
			// If near and not fired, release tension
			if (mCATA.get_position() < 3.6 && !catapultState){ 
				mCATA = 127;
			}

			// If at the very bottom and not fired, hold
			if (mCATA.get_position() >= 3.6 && !catapultState){
				mCATA = 0;
				mCATA.brake();
			}
		}
		std::cout << mCATA.get_position() << std::endl;

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

		double leftMotorAdjust = 1;
		double rightMotorAdjust = 1;

		if(!mBROState){
			leftMotorAdjust = .75;
		}

		int intOverflow = 2147483647; //value returned when unable to communicate

		// left side adjustments
		if (mBROState && mBRO.get_voltage() == intOverflow){
			leftMotorAdjust = leftMotorAdjust * .5;
		}
		if (mBRI.get_voltage() == intOverflow){
			leftMotorAdjust = leftMotorAdjust * .5;
		}
		if (mFRO.get_voltage() == intOverflow){
			leftMotorAdjust = leftMotorAdjust * .5;
		}
		if (mFRI.get_voltage() == intOverflow){
			leftMotorAdjust = leftMotorAdjust * .5;
		}

		// Right side adjustment
		if (mBLO.get_voltage() == intOverflow){
			rightMotorAdjust = rightMotorAdjust * .5;
		}
		if (mBLI.get_voltage() == intOverflow){
			rightMotorAdjust = rightMotorAdjust * .5;
		}
		if (mFLO.get_voltage() == intOverflow){
			rightMotorAdjust = rightMotorAdjust * .5;
		}
		if (mFLI.get_voltage() == intOverflow){
			rightMotorAdjust = rightMotorAdjust * .5;
		}

		// Front Left
		mBRO = (leftJoy - rightJoy - frontTurnAdj + strafeJoy) * 127 * speedMultiplier * rightMotorAdjust;
		mBRI = (leftJoy - rightJoy - frontTurnAdj + strafeJoy) * 127 * speedMultiplier * rightMotorAdjust;

		// Rear Left
		mFRO = (leftJoy - rightJoy - backTurnAdj - strafeJoy) * 127 * speedMultiplier * rightMotorAdjust;
		mFRI = (leftJoy - rightJoy - backTurnAdj - strafeJoy) * 127	* speedMultiplier * rightMotorAdjust;

		// Front Right
		mBLO = (leftJoy + rightJoy + frontTurnAdj - strafeJoy) * 127 * speedMultiplier * leftMotorAdjust;
		mBLI = (leftJoy + rightJoy + frontTurnAdj - strafeJoy) * 127 * speedMultiplier * leftMotorAdjust;

		// Rear Right
		mFLO = (leftJoy + rightJoy + backTurnAdj + strafeJoy) * 127 * speedMultiplier * leftMotorAdjust;
		mFLI = (leftJoy + rightJoy + backTurnAdj + strafeJoy) * 127 * speedMultiplier * leftMotorAdjust;

		// Second controller code (runs every 3 loops to prevent controller rendering errors
		// . means normal - means somethin is up
		if(partner.get_digital_new_press(DIGITAL_L1)){
			shieldLauncherAuto = !shieldLauncherAuto;
			if(shieldLauncherAuto){
				partnerDot = true;
				masterDot = true;
			}else{
				partnerDash = true;
				masterDash = true;
			}
		}
		if(partner.get_digital_new_press(DIGITAL_L2)){
			stringLauncherAuto = !stringLauncherAuto;
			if(stringLauncherAuto){
				partnerDot = true;
				masterDot = true;
			}else{
				partnerDash = true;
				masterDash = true;
			}
		}
		if(partner.get_digital_new_press(DIGITAL_R1)){
			clockOverride = !clockOverride;
			if(!clockOverride){
				partnerDot = true;
				masterDot = true;
			}else{
				partnerDash = true;
				masterDash = true;
			}
		}
		
		// Timing delays to avoid controller communication errors
		if(loopCounter % 15 == 0){
			// Print line 1
			partner.print(0, 0, "Shld: %d  Ovrd: %d", shieldLauncherAuto, clockOverride);
		}
		if(loopCounter % 15 == 3){
			// Print line 2
			partner.print(1, 0, "Str: %d", stringLauncherAuto);
		}
		if(loopCounter % 15 == 6){
			// Print line 3
			partner.print(2, 0, "Clock: %i  ", (int)(105 - ((pros::millis() - startTime)/1000)));
		}
		if(loopCounter % 15 == 9){
			// Vibrations
			if(masterDot){
				master.rumble(".");
				masterDot = false;
			}else if(masterDash){
				master.rumble("-");
				masterDash = false;
			}
		}
		if(loopCounter % 15 == 12){
			// Vibrations for partner controller
			if(partnerDot){
				partner.rumble(".");
				partnerDot = false;
			}else if(partnerDash){
				partner.rumble("-");
				partnerDash = false;
			}
		}
		

		if(!clockOverride){
			if(!skills){
				// String launchers
				if(pros::millis() - startTime > 95000 && !stringReleased && master.get_digital_new_press(DIGITAL_R1)){
					stringReleased = true;
					stringRelease.set_value(true);
				}

				// Auto string launchers
				if(stringLauncherAuto && pros::millis() - startTime > 104000 && !stringReleased){
					stringReleased = true;
					stringRelease.set_value(true);
				}

				// Auto shield launcher
				if(shieldLauncherAuto && !shieldReleased && pros::millis() - startTime > 95000){
					shieldReleased = true;
					shieldRelease.set_value(true);
				}
				
				// Manual shield launcher
				if(master.get_digital_new_press(DIGITAL_UP) && !shieldReleased && pros::millis() - startTime > 95000){
					shieldReleased = true;
					shieldRelease.set_value(true);
				}

				// 5 second warning for endgame stuff
				if(!fiveSecondWarningTriggered && pros::millis() - startTime > 100000){
					fiveSecondWarningTriggered = true;
					master.rumble("---");
				}
			} else{
				// String launchers
				if(pros::millis() - startTime > 50000 && !stringReleased && master.get_digital_new_press(DIGITAL_R1)){
					stringReleased = true;
					stringRelease.set_value(true);
				}

				// Auto string launchers
				if(stringLauncherAuto && pros::millis() - startTime > 59000 && !stringReleased){
					stringReleased = true;
					stringRelease.set_value(true);
				}
				
				// Auto shield launcher
				if(shieldLauncherAuto && !shieldReleased && pros::millis() - startTime > 57000){
					shieldReleased = true;
					shieldRelease.set_value(true);
				}

				// Manual shield launcher
				if(master.get_digital_new_press(DIGITAL_UP) && !shieldReleased && pros::millis() - startTime > 50000){
					shieldReleased = true;
					shieldRelease.set_value(true);
				}

				// 5 second warning for endgame stuff
				if(!fiveSecondWarningTriggered && pros::millis() - startTime > 55000){
					fiveSecondWarningTriggered = true;
					master.rumble("---");
				}
			}
			// Clock functions override
			if(master.get_digital_new_press(DIGITAL_B)){
				clockOverride = true;
				master.rumble("-");
			}
		}else{
			// Manual string launcher
			if(!stringReleased && master.get_digital_new_press(DIGITAL_R1)){
				stringReleased = true;
				stringRelease.set_value(true);
			}

			// Manual shield launcher
			if(!shieldReleased && master.get_digital_new_press(DIGITAL_UP)){
					shieldReleased = true;
					shieldRelease.set_value(true);
			}
		}

		loopCounter++;

		pros::delay(20);
	}
}
