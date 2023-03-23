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

void driveViaIMU(double dist, double heading, double vel){
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
				rotation = vel * sgn(error); // was 200
			}
			mBRO.move_velocity(vel - rotation);
			mBRI.move_velocity(vel - rotation);
			mFRO.move_velocity(vel - rotation);
			mFRI.move_velocity(vel - rotation);
			mBLO.move_velocity(vel + rotation * (.75 + mBROState));
			mBLI.move_velocity(vel + rotation * (.75 + mBROState));
			mFLO.move_velocity(vel + rotation * (.75 + mBROState));
			mFLI.move_velocity(vel + rotation * (.75 + mBROState));
			pos = (mBRI.get_position() + mBLI.get_position())/2;
			pros::delay(5);
		}
	}else{
		while(pos > dist){
			double error = heading - imu.get_rotation();
			int rotation;
			if(std::fabs(error) < 15){
				rotation = (12 * error); // Was 6
			}else{
				rotation = vel * sgn(error); // was 200
			}
			mBRO.move_velocity(-vel - rotation);
			mBRI.move_velocity(-vel - rotation);
			mFRO.move_velocity(-vel - rotation);
			mFRI.move_velocity(-vel - rotation);
			mBLO.move_velocity(-vel + rotation * (.75 + mBROState));
			mBLI.move_velocity(-vel + rotation * (.75 + mBROState));
			mFLO.move_velocity(-vel + rotation * (.75 + mBROState));
			mFLI.move_velocity(-vel + rotation * (.75 + mBROState));
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

void strafeViaIMU(double dist, double heading, double motorAdjust){ //positive dist is strafe right
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
			mBLO.move_velocity(-200 + rotation * (.75 + mBROState)); //adjust for motor
			mBLI.move_velocity(-200 + rotation * (.75 + mBROState));
			mFLO.move_velocity(200 + rotation * (.75 + mBROState));
			mFLI.move_velocity(200 + rotation * (.75 + mBROState));
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
			mBLO.move_velocity(200 + rotation * (.75 + mBROState));
			mBLI.move_velocity(200 + rotation * (.75 + mBROState));
			mFLO.move_velocity(-200 + rotation * (.75 + mBROState));
			mFLI.move_velocity(-200 + rotation * (.75 + mBROState));
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

void driveViaTime(double time, double vel){
	startTime = pros::millis();
	while(pros::millis() - startTime < time){
		mBRI.move_velocity(vel);
		mFRO.move_velocity(vel);
		mFRI.move_velocity(vel);
		mBLO.move_velocity(vel);
		mBLI.move_velocity(vel);
		mFLO.move_velocity(vel);
		mFLI.move_velocity(vel);
		pros::delay(10);
	}
	mBRI.move_velocity(0);
	mFRO.move_velocity(0);
	mFRI.move_velocity(0);
	mBLO.move_velocity(0);
	mBLI.move_velocity(0);
	mFLO.move_velocity(0);
	mFLI.move_velocity(0);
}

double getLSD(float rangeStart, float rangeStop){
	double dist;
	std::vector<double> values;

	for (double i = rangeStart; i <= rangeStop; i ++){
		values.push_back(i);
	}
	bool isInRange = false;
	while (!isInRange){
		dist = lsd.get();
		for (int i = 0; i < values.size() + 1; i++){
			if (std::abs(dist - values[i]) < .0001){
				isInRange = true;
				break;
			}
		}
	if (isInRange) break;
	}
	return dist;
}

void matchLoadDisks(double lsdTarget){

	/* Function order:
	 * 1. Shoot the preload
	 * 2. Begin reloading catapult
	 * 3. Undo the arc
	 * 4. Drive reverse into loading area
	 * 5. Wait for disc to be loaded
	 * 6. Arc towards the goal
	 * 7. Repeat steps 1-6
	*/
	pros::Task taskChangeColor(changeColor, "Change Color");
	pros::delay(800);
	driveViaIMU(.5, 0, 200);
	turnViaIMU(90);
	driveViaIMU(.54, 90, 200);
	pros::delay(300); // uncomment if not working
	catapultRelease.set_value(true);
	catapultState = true;
	pros::Task taskReloadCatapult(reloadCatapult, "Reload Catapult");
	driveViaIMU(-2.2, 90, 200);
	turnViaIMU(0);
	driveViaIMU(-.4, 0, 200);
	driveViaTime(200, -100);
	double dist = getLSD(lsdTarget - 500, lsdTarget + 500);
	if (dist < (lsdTarget - 40)){
		while (dist < (lsdTarget - 40)){
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
	if (dist > (lsdTarget + 40)){
		while (dist > (lsdTarget + 40)){ //strafe right
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
	// TODO: Add code that turns brain screen green when in position to accept disks

}