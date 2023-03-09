#include "main.h"
#include "globals.h"

void eliScoreRoller(){
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
void strafeViaIMU(double dist, double heading){ //positive dist is strafe right
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
			mFRO.move_velocity(-200 + rotation);
			mFRI.move_velocity(-200 + rotation);
			mBLO.move_velocity(-200 + rotation);
			mBLI.move_velocity(-200 + rotation);
			mFLO.move_velocity(200 + rotation);
			mFLI.move_velocity(200 + rotation);
			pos = (mFLI.get_position() + mFLO.get_position())/2;
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
			mFRO.move_velocity(200 - rotation);
			mFRI.move_velocity(200 - rotation);
			mBLO.move_velocity(200 + rotation);
			mBLI.move_velocity(200 + rotation);
			mFLO.move_velocity(-200 + rotation);
			mFLI.move_velocity(-200 + rotation);
			pos = (mFLI.get_position() + mFLO.get_position())/2;
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

void driveViaTime(double time, bool forward, double vel){
	startTime = pros::millis();
	if (forward){
		while(pros::millis() - startTime < time){
				mBRO.move_velocity(vel);
				mBRI.move_velocity(vel);
				mFRO.move_velocity(vel);
				mFRI.move_velocity(vel);
				mBLO.move_velocity(vel);
				mBLI.move_velocity(vel);
				mFLO.move_velocity(vel);
				mFLI.move_velocity(vel);
			}
			mBRO.move_velocity(0);
			mBRI.move_velocity(0);
			mFRO.move_velocity(0);
			mFRI.move_velocity(0);
			mBLO.move_velocity(0);
			mBLI.move_velocity(0);
			mFLO.move_velocity(0);
			mFLI.move_velocity(0);
	} else{
		while(pros::millis() - startTime < time){
			mBRO.move_velocity(-vel);
			mBRI.move_velocity(-vel);
			mFRO.move_velocity(-vel);
			mFRI.move_velocity(-vel);
			mBLO.move_velocity(-vel);
			mBLI.move_velocity(-vel);
			mFLO.move_velocity(-vel);
			mFLI.move_velocity(-vel);
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

}


void matchLoadDisks(){ 
	pros::delay(750);
	mBLO.tare_position();
	mBLI.tare_position();
	double pos = (mBLO.get_position() + mBLI.get_position())/2;
	while(pos < 1){ // Distance to arc turn
		mBLO.move_velocity(80);
		mBLI.move_velocity(80);
		mFLO.move_velocity(80);
		mFLI.move_velocity(80);
		pos = (mBLO.get_position() + mBLI.get_position())/2;
		pros::delay(5);
	}
	pros::delay(100);
	catapultRelease.set_value(true);
	pros::Task taskReloadCatapult(reloadCatapult, "Reload Catapult");
	mBLO.tare_position();
	mBLI.tare_position();
	pos = (mBLO.get_position() + mBLI.get_position())/2;
	while(pos > -1){
		mBLO.move_velocity(-80);
		mBLI.move_velocity(-80);
		mFLO.move_velocity(-80);
		mFLI.move_velocity(-80);
		pos = (mBLO.get_position() + mBLI.get_position())/2;
		pros::delay(5);
	}
	startTime = pros::millis();
	while (pros::millis() - startTime > 100){
		mBLO.move_velocity(-80);
		mBLI.move_velocity(-80);
		mFLO.move_velocity(-80);
		mFLI.move_velocity(-80);
		mBRO.move_velocity(-80);
		mBRI.move_velocity(-80);
		mFRO.move_velocity(-80);
		mFRI.move_velocity(-80);
	}
	mBLO.move_velocity(0);
	mBLI.move_velocity(0);
	mFLO.move_velocity(0);
	mFLI.move_velocity(0);
	mBRO.move_velocity(0);
	mBRI.move_velocity(0);
	mFRO.move_velocity(0);
	mFRI.move_velocity(0);

}