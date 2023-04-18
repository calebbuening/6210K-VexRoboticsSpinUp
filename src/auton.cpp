#include "main.h"
#include "globals.h"
#include "autonmethods.h"

#define STD 10 // The standard task delay

void autonomous(){

	// TO PREVENT INSANE LEVELS OF DRIFT FROM SITTING AROUND
	imu.tare();

	// THE LEAGUE AUTON
	if(auton == 'S' || auton == 'Z'){
		// Score the first roller
		int autonAdjust = 1;
		if(auton=='Z'){
			autonAdjust = -1;
			driveViaIMU(3.2, 0, 200);
			turnViaIMU(90);
		}

		imu.tare(); // Call this zero so that everything else works, even though S and Z have different
					// starting positions

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
		driveViaIMU(-7.4, 45 * autonAdjust, 200);

		// Turn towards the goal
		turnViaIMU(143 * autonAdjust); // was 141.5
		
		// Delay to make sure the discs are at rest
		pros::delay(1000);

		// Fire the catapult
		catapultRelease.set_value(true);
		catapultState = true;
	}	

	if(auton == 'A'){

		// Wheels also hold
		mBRO.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		mBRI.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		mFRO.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		mFRI.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		mBLO.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		mBLI.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		mFLO.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		mFLI.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

		// Flash the screen to indicate the start of the program
		pros::Task taskFlashScreen(flashScreen, "Flash Screen");

		// Record the currect distance from the wall, to use when realigning
		double distTarget = lsd.get();
		while (distTarget < 1 || distTarget >= 5200){//filter to reasonable values
			distTarget = lsd.get();
			pros::delay(5);
		}


		// Manually take a third shot to avoid the latter part of the previously written loop
		// NOTE: The following values should mirror the scoring loop
		pros::delay(800);
		driveViaIMU(.75, 0); //.5
		turnViaIMU(90);
		driveViaIMU(2.1, 90); // 1.9
		pros::delay(200); //uncomment if it isn't working, this is delay for shot
		catapultRelease.set_value(true);
		catapultState = true;

		// Turn and drive to face roller, the score
		driveViaIMU(-7.7, 90); // -7.5
		turnViaIMU(180);
		eliScoreRoller();

		// drive and score roller 2
		driveViaIMU(-2.5, 180, 200); //was2.5
		turnViaIMU(270);
		driveViaIMU(1.2, 270, 300); //1.0
		eliScoreRoller();

		// drive across field to other loader
		driveViaIMU(-.5, 270, 200);
		turnViaIMU(220); // was 225, same below
		driveViaIMU(-17, 220, 600); //12
		driveViaIMU(1, 220, 500);
		turnViaIMU(180);
		driveViaIMU(-1, 180, 300); // Added this to keep it straight over the discs
		driveViaTime(1000, -500); //1000was200 Bang up against the wall to realign ourself

		// Use the distance sensor to align with the loading area
		double dist = lsd.get();
		while (0 > dist || dist >= 5200) {
			dist = lsd.get();
			pros::delay(10);
		}
		if (dist < (distTarget - 40)){
			while (dist < (distTarget - 40)){
				mBRO.move_velocity(200);
				mBRI.move_velocity(200);
				mFRO.move_velocity(-200);
				mFRI.move_velocity(-200);
				mBLO.move_velocity(-200); //adjust for motor
				mBLI.move_velocity(-200);
				mFLO.move_velocity(200);
				mFLI.move_velocity(200);

				dist = lsd.get();
				pros::delay(10);
			}
		}
		if (dist > (distTarget + 40)){
			while (dist > (distTarget + 40)){ //strafe right
			 	mBRO.move_velocity(-200);
				mBRI.move_velocity(-200);
				mFRO.move_velocity(200);
				mFRI.move_velocity(200);
				mBLO.move_velocity(200);
				mBLI.move_velocity(200);
				mFLO.move_velocity(-200);
				mFLI.move_velocity(-200);
				dist = lsd.get();
				pros::delay(10);
			}
		}

		// Alert for loading and press up against the wall whilst loading again
		pros::Task taskFlashScreen2(flashScreen, "Flash Screen");
		driveViaTime(200, -400);


		// Zero the imu to make everything from here on out a lot easier
		imu.set_rotation(imu.get_rotation()-180);

		// Take a shot
		// Load the preload again
		pros::delay(800);

		// Take a second shot
		// NOTE: This should also mirror the scoring loop
		driveViaIMU(.75, 0, 200); //.5
		turnViaIMU(90);
		driveViaIMU(2.1, 90);
		pros::delay(200);

		catapultRelease.set_value(true);
		catapultState = true;

		// Reload the catapult again because idk why

		// TODO: Check and see how much this mirrors the original
		// Turn and drive to face roller, score
		driveViaIMU(-7.9, 90, 400); //-7.5
		turnViaIMU(180);
		eliScoreRoller();

		// drive and score roller 4
		driveViaIMU(-2.5, 180, 200); //was2.5
		turnViaIMU(270);
		driveViaIMU(1.2, 270, 300); //1.0
		driveViaTime(1000, 400);
		eliScoreRoller();

		// Line up for endgame
		driveViaIMU(-1, 270, 200);
		turnViaIMU(225);

		// Endgame
		stringRelease.set_value(true);

		// Wheels also hold
		mBRO.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		mBRI.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		mFRO.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		mFRI.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		mBLO.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		mBLI.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		mFLO.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
		mFLI.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
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
		driveViaIMU(3, 0, 200);
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
	if(auton == 'T'){
		while(true){
			double dist = lsd.get();
			std::cout << dist << std::endl;
			pros::delay(1000);
		}
	}

	if(auton == 'E'){
		driveViaIMU(2, 0);
	}
}
