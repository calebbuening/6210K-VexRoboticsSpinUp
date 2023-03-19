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
		pros::delay(1000);
		driveViaIMU(.5, 0);
		turnViaIMU(90);
		driveViaIMU(.54, 90);
		pros::delay(700);
		catapultRelease.set_value(true);
		catapultState = true;
		pros::Task taskReloadCatapult(reloadCatapult, "Reload Catapult");

		//turn and drive to face roller, score
		driveViaIMU(-8, 90);
		turnViaIMU(180);
		eliScoreRoller();
		// drive and score roller 2
		driveViaIMU(-2.5, 180);
		turnViaIMU(270);
		driveViaIMU(1, 270);
		eliScoreRoller();
		// drive through low zone evadining pole and barrier
		driveViaIMU(-.5, 270);
		turnViaIMU(360);
		// get baseline distance from wall
		distTarget = lsd.get();
		while (distTarget < 1 || distTarget >= 5200){//filter to possible values
			distTarget = lsd.get();
			pros::delay(5);
		}
		// gun it into wall using sensor to avoid pole and barrier
		double start = pros::millis();
		while (pros::millis() - start < 5000){
			dist = getLSD(distTarget - 500, distTarget + 500); // filter to reasonable values
			if(dist < distTarget - 40){
				while (dist < (lsdTarget - 40)){
					mBRO.move_velocity(200);
					mBRI.move_velocity(200);
					mFRO.move_velocity(-200);
					mFRI.move_velocity(-200);
					mBLO.move_velocity(-200 * (.75 + mBROState));
					mBLI.move_velocity(-200 * (.75 + mBROState));
					mFLO.move_velocity(200 * (.75 + mBROState));
					mFLI.move_velocity(200 * (.75 + mBROState));
					dist = getLSD(distTarget - 500, distTarget + 500);
					pros::delay(20);
				}
			} else if (dist > distTarget + 40){
				while (dist > (distTarget + 40)){
					mBRO.move_velocity(-200);
					mBRI.move_velocity(-200);
					mFRO.move_velocity(200);
					mFRI.move_velocity(200);
					mBLO.move_velocity(200 * (.75 + mBROState));
					mBLI.move_velocity(200 * (.75 + mBROState));
					mFLO.move_velocity(-200 * (.75 + mBROState));
					mFLI.move_velocity(-200 * (.75 + mBROState));
					dist = getLSD(distTarget - 500, distTarget + 500);
					pros::delay(20);
				}
			} else{
				mBRO.move_velocity(-200);
				mBRI.move_velocity(-200);
				mFRO.move_velocity(200);
				mFRI.move_velocity(200);
				mBLO.move_velocity(200 * (.75 + mBROState));
				mBLI.move_velocity(200 * (.75 + mBROState));
				mFLO.move_velocity(-200 * (.75 + mBROState));
				mFLI.move_velocity(-200 * (.75 + mBROState));
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

		//driveViaTime(4000, true, 200);
		imu.tare();
		driveViaIMU(-.4, 0);
		turnViaIMU(90);
		driveViaIMU(10, 90);
		/*
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
		while(true){
			double dist = lsd.get();
			std::cout << dist << std::endl;
			pros::delay(1000);
		}
	}
}