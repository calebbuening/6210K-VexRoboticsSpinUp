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
		double distTarget = lsd.get();
		while (distTarget < 1 || distTarget >= 5200){//filter to possible values
			distTarget = lsd.get();
			pros::delay(5);
		} 
		for(int i = 0; i <= 1; i++){
			matchLoadDisks(distTarget);
		}
		// Third shot
		// screen green
		pros::delay(800);
		// screen normal
		driveViaIMU(.5, 0);
		turnViaIMU(90);
		driveViaIMU(.54, 90);
		//pros::delay(700); //uncomment if it isn't working, this is delay for shot
		catapultRelease.set_value(true);
		catapultState = true;
		pros::Task taskReloadCatapult(reloadCatapult, "Reload Catapult");

		//turn and drive to face roller, score
		driveViaIMU(-8, 90);
		turnViaIMU(180);
		eliScoreRoller();
		// drive and score roller 2
		driveViaIMU(-2.2, 180);
		turnViaIMU(270);
		driveViaIMU(1, 270);
		eliScoreRoller();
		// drive across field to other loader
		driveViaIMU(-.5, 270);
		turnViaIMU(225);
		driveViaIMU(-10, 225);
		turnViaIMU(180);
		driveViaTime(2000, -200)
		//strafe align with loader and get baseline dist
		dist = lsd.get();
		while (0 > dist || dist >= 5200) {
			dist = lsd.get();
			pros::delay(10);
		}
		distTarget = getLSD(dist - 500, dist + 500);
		if (dist < (distTarget - 40)){
			while (dist < (distTarget - 40)){
				mBRO.move_velocity(50);
				mBRI.move_velocity(50);
				mFRO.move_velocity(-50);
				mFRI.move_velocity(-50);
				mBLO.move_velocity(-50);
				mBLI.move_velocity(-50);
				mFLO.move_velocity(50);
				mFLI.move_velocity(50);
				dist = lsd.get();
				pros::delay(10);
			}
		}
		if (dist > (distTarget + 40)){
			while (dist > (distTarget + 40)){ //strafe right
				mBRO.move_velocity(-50);
				mBRI.move_velocity(-50);
				mFRO.move_velocity(50);
				mFRI.move_velocity(50);
				mBLO.move_velocity(50 * (.75 + mBROState));
				mBLI.move_velocity(50 * (.75 + mBROState));
				mFLO.move_velocity(-50 * (.75 + mBROState));
				mFLI.move_velocity(-50 * (.75 + mBROState));
				dist = lsd.get();
				pros::delay(10);
			}
		}
		driveViaTime(200, -100);

		imu.tare();
		matchLoadDisks(distTarget);

		// Second shot
		pros::delay(1000);
		driveViaIMU(.5, 0);
		turnViaIMU(90);
		driveViaIMU(.54, 90);
		//pros::delay(700);
		catapultRelease.set_value(true);
		catapultState = true;
		// next line is almighty omega jank
		pros::Task taskReloadCatapult_2(reloadCatapult, "Reload Catapult");

		//turn and drive to face roller, score
		driveViaIMU(-8, 90);
		turnViaIMU(180);
		eliScoreRoller();
		// drive and score roller 4
		driveViaIMU(-2.2, 180);
		turnViaIMU(270);
		driveViaIMU(1, 270);
		eliScoreRoller();
		// Line up for endgame
		driveViaIMU(-1, 270);
		turnViaIMU(225);
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
		while(true){
			double dist = lsd.get();
			std::cout << dist << std::endl;
			pros::delay(1000);
		}
	}
}