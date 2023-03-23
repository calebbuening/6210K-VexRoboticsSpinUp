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
			driveViaIMU(3.2, 0, 200);
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
		pros::Task taskChangeColor(changeColor, "Change Color");
		pros::delay(800);
		// screen normal
		driveViaIMU(.5, 0, 200);
		turnViaIMU(90);
		driveViaIMU(.54, 90, 200);
		pros::delay(300); //uncomment if it isn't working, this is delay for shot
		catapultRelease.set_value(true);
		catapultState = true;
		pros::Task taskReloadCatapult(reloadCatapult, "Reload Catapult");

		//turn and drive to face roller, score
		driveViaIMU(-7.5, 90, 600);
		turnViaIMU(180);
		eliScoreRoller();
		// drive and score roller 2
		driveViaIMU(-2.2, 180, 200);
		turnViaIMU(270);
		driveViaIMU(1, 270, 200);
		eliScoreRoller();
		// drive across field to other loader
		driveViaIMU(-.5, 270, 200);
		turnViaIMU(225);
		driveViaIMU(-10, 225, 600);
		turnViaIMU(180);
		driveViaTime(1000, -600);
		//strafe align with loader and get baseline dist
		double dist = lsd.get();
		while (0 > dist || dist >= 5200) {
			dist = lsd.get();
			pros::delay(10);
		}
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
		pros::Task taskChangeColor_2(changeColor, "Change Color");
		pros::delay(800);
		driveViaIMU(.5, 0, 200);
		turnViaIMU(90);
		driveViaIMU(.54, 90, 200);
		pros::delay(300);
		catapultRelease.set_value(true);
		catapultState = true;
		// next line is almighty omega jank
		pros::Task taskReloadCatapult_2(reloadCatapult, "Reload Catapult");

		//turn and drive to face roller, score
		driveViaIMU(-7.5, 90, 600);
		turnViaIMU(180);
		eliScoreRoller();
		// drive and score roller 4
		driveViaIMU(-2.2, 180, 200);
		turnViaIMU(270);
		driveViaIMU(1, 270, 200);
		eliScoreRoller();
		// Line up for endgame
		driveViaIMU(-1, 270, 200);
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
		static lv_style_t white_style;
		lv_style_copy(&white_style, &lv_style_plain);
		white_style.body.main_color = LV_COLOR_WHITE;
		white_style.body.grad_color = LV_COLOR_WHITE;

		static lv_style_t black_style;
		lv_style_copy(&black_style, &lv_style_plain);
		black_style.body.main_color = LV_COLOR_BLACK;
		black_style.body.grad_color = LV_COLOR_BLACK;

		lv_obj_set_style(lv_scr_act(), &white_style);
		pros::delay(800);
		lv_obj_set_style(lv_scr_act(), &black_style);
	}
}