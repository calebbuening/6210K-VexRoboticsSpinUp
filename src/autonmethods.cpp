#include "main.h"
#include "api.h"
#include "globals.h"
#include "src/model.h"
#include <fstream>
#include <string>

using keras2cpp::Model;
using keras2cpp::Tensor;
double timeSinceMSD = 0;
double count = 0;

void eliScoreRoller(){
	int loop=0;
		while(loop<1){

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
			//pros::delay(500);

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
}

// Returns -1, 0, and 1 based on number direction
int sgn(double d) // Mimimcs the mathematical sgn function
{
	if(d < 0){return -1;}
	if(d > 0){return 1;}
	return 0;
}

double filtered_average(std::vector<double> values) {
    // Sort the values in ascending order
    std::sort(values.begin(), values.end());

    // Calculate the median value
    double median = values[values.size() / 2];

    // Filter out any values that are more than 1.5 times the interquartile range from the median
    double q1 = values[values.size() / 4];
    double q3 = values[3 * values.size() / 4];
    double iqr = q3 - q1;
    std::vector<double> filtered_values;
    for (double x : values) {
        if (x >= median - 1.5 * iqr && x <= median + 1.5 * iqr) {
            filtered_values.push_back(x);
        }
    }

    // Calculate the average of the filtered values
    double sum = 0;
	for (double x : filtered_values) {
		sum += x;
	}
	return sum / filtered_values.size();
}

void turnViaIMU(double heading){
	double error = heading - imu.get_rotation();
	int rotation;
	while(std::fabs(error) > .25)
	{
		if(std::fabs(error) < 30){
			rotation = -(4.5 * error); // Was 9
		}else{
			rotation = -100 * sgn(error); // was 200
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
	rotation = 30 * sgn(error); // was 30
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

void driveViaIMU(double dist, double heading, double vel = 200){
	vel = std::fabs(vel); // Make sure velocity is a magnitude, since direction is automatically determined
	mBRI.tare_position();
	mBLI.tare_position();
	double pos = (mBRI.get_position() + mBLI.get_position())/2;
	dist += pos;
	if(dist > pos){
		while(pos < dist){
			double error = heading - imu.get_rotation();
			int rotation;
			if(std::fabs(error) < 15){ // Was 30
				rotation = (6 * error); // Was 12
			}else{
				rotation = vel * sgn(error); // was 200
			}
			mBRO.move_velocity(vel - rotation);
			mBRI.move_velocity(vel - rotation);
			mFRO.move_velocity(vel - rotation);
			mFRI.move_velocity(vel - rotation);
			mBLO.move_velocity(vel + rotation);
			mBLI.move_velocity(vel + rotation);
			mFLO.move_velocity(vel + rotation);
			mFLI.move_velocity(vel + rotation);
			pos = (mBRI.get_position() + mBLI.get_position())/2;
			pros::delay(5);
		}
	}else{
		while(pos > dist){
			double error = heading - imu.get_rotation();
			int rotation;
			if(std::fabs(error) < 15){
				rotation = (6 * error); // Was 6
			}else{
				rotation = vel * sgn(error); // was 200
			}
			mBRO.move_velocity(-vel - rotation);
			mBRI.move_velocity(-vel - rotation);
			mFRO.move_velocity(-vel - rotation);
			mFRI.move_velocity(-vel - rotation);
			mBLO.move_velocity(-vel + rotation);
			mBLI.move_velocity(-vel + rotation);
			mFLO.move_velocity(-vel + rotation);
			mFLI.move_velocity(-vel + rotation);
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

/////////////////PROTOTYPE METHODS//////////////////////
double updateMSDTime() {
    if (msd.get() < 5280 && count < 3) {
        // If the LSD just detected a value less than 5280, update the timeSinceMSD variable
        count++;
		timeSinceMSD += 20;
    } else if (msd.get() < 5280 && count >=3){
		timeSinceMSD = 0;
		count = 0;
	}
	else{
		timeSinceMSD += 20;
	}
	return timeSinceMSD;
}

void logData(double leftJoy){
// CURRENT CODE - UNTESTED
	std::vector<double> motor_positions(8);
	motor_positions[0] = mBRO.get_position();
	motor_positions[1] = mBRI.get_position();
	motor_positions[2] = mFRO.get_position();
	motor_positions[3] = mFRI.get_position();
	motor_positions[4] = mBLO.get_position();
	motor_positions[5] = mBLI.get_position();
	motor_positions[6] = mFLO.get_position();
	motor_positions[7] = mFLI.get_position();

	double MSD_TIME = updateMSDTime();
	//saves all data to sd card
	std::ofstream dataFile;
	dataFile.open("/usd/data.csv", std::ios_base::app);
	dataFile << float(127 * leftJoy) << ", " << float(filtered_average(motor_positions)) << ", " << float(lsd.get()) << ", " << float(msd.get()) << ", " << float(bsd.get()) << ", " << float(LSD_TIME) << std::endl;
	dataFile.close();
}

void giveInstruction(){
	auto model = Model::load("/usd/auton_blocker.model");
	if (count > 2){
		count = 0;
		// convert everything to floats so the tensor doesn't cry
		std::vector<double> motor_positions(8);
		motor_positions[0] = mBRO.get_position();
		motor_positions[1] = mBRI.get_position();
		motor_positions[2] = mFRO.get_position();
		motor_positions[3] = mFRI.get_position();
		motor_positions[4] = mBLO.get_position();
		motor_positions[5] = mBLI.get_position();
		motor_positions[6] = mFLO.get_position();
		motor_positions[7] = mFLI.get_position();

		float pos = filtered_average(motor_positions);
		float Lsd = lsd.get();
		float Msd = msd.get();
		float Bsd = bsd.get();
		double time_since_msd = updateMSDTime();
		time_since_msd = float(time_since_msd);
		// load model and create input tensor
		model = Model::load("/usd/auton_blocker.model");
		Tensor in{1, 5};
		in.data_[0] = pos;
		in.data_[1] = Lsd;
		in.data_[2] = Msd;
		in.data_[3] = Bsd;
		in.data_[4] = time_since_msd;

		// Run prediction
		Tensor out = model(in);
		double speed = out.data_[0];
		if(speed < 2 && speed > -2){
			speed = 0;
		}
		if(speed > 2){
			speed = 127;
		}
		if(speed < -2){
			speed = -127;
		}
		mBRO = speed;
		mBRI = speed;
		mFRO = speed;
		mFRI = speed;
		mBLO = speed;
		mBLI = speed;
		mFLO = speed;
		mFLI = speed;
	
		std::cout << speed << std::endl;
	} else{
		count ++;
	}
	
}