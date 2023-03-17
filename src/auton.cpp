#include "main.h"
#include "globals.h"
#include "autonmethods.h"

void autonomous(){

	// THE LEAGUE AUTON
	if(auton == 'S' || auton == 'Z'){
		// Score the first roller
		int autonAdjust = 1;
		if(auton=='Z'){
			autonAdjust = -1;
			driveViaIMU(3.2, 0);
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
		turnViaIMU(143 * autonAdjust); // was 141.5
		
		// Delay to make sure the discs are at rest
		pros::delay(1000);

		// Fire the catapult
		catapultRelease.set_value(true);
		catapultState = true;
	}	

	if(auton == 'A'){
		//Score match loads
		for(int i = 0; i < 1; i++){
			matchLoadDisks();
		}

		/*
		// get out from wall
		driveViaIMU(1, 0);
		//turn and drive to face roller, score
		turnViaIMU(90);
		driveViaIMU(-4, 90);
		turnViaIMU(180);
		eliScoreRoller();
		// drive and score roller 2
		driveViaIMU(-1.5, 180);
		turnViaIMU(270);
		eliScoreRoller();
		// drive through low zone and score roller 3
		driveViaIMU(-1, 270);
		turnViaIMU(360);
		//driveViaTime(4000, true, 200);
		imu.tare();
		driveViaIMU(-.1, 0);
		turnViaIMU(90);
		driveViaIMU(3, 90);
		//strafeViaIMU(2, 90);
		driveViaIMU(4.5, 90);
		turnViaIMU(0);
		eliScoreRoller();
		//score roller 4
		driveViaIMU(-1.5, 0);
		turnViaIMU(90);
		driveViaIMU(.5, 90);
		eliScoreRoller();
		// fire endgame
		driveViaIMU(-1.5, 90);
		turnViaIMU(45);
		stringRelease.set_value(true);
		driveViaIMU(1, 45);
		driveViaIMU(-.6, 45);
		*/
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
	if(auton == 'T'){
		driveViaIMU(3, 0);
	}
}