#include "main.h"
#include "api.h"
#include "globals.h"
#include "src/model.h"
#include <fstream>
#include <string>

using keras2cpp::Model;
using keras2cpp::Tensor;
double timeSinceLSD = 0;
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
	while(std::fabs(error) > .5)
	{
		if(std::fabs(error) < 30){
			rotation = -(9 * error); // Was 6
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
				rotation = (12 * error); // Was 6
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
				rotation = (12 * error); // Was 6
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

double getLSD(double rangeStart, double rangeStop){
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

void flashScreen(){
	static lv_style_t white_style;
	lv_style_copy(&white_style, &lv_style_plain);
	white_style.body.main_color = LV_COLOR_WHITE;
	white_style.body.grad_color = LV_COLOR_WHITE;
	
	static lv_style_t black_style;
	lv_style_copy(&black_style, &lv_style_plain);
	black_style.body.main_color = LV_COLOR_BLACK;
	black_style.body.grad_color = LV_COLOR_BLACK;

	lv_obj_set_style(lv_scr_act(), &white_style);
	pros::delay(1000);
	lv_obj_set_style(lv_scr_act(), &black_style);
}

/////////////////PROTOTYPE METHODS//////////////////////
double updateLSDTime() {
    if (lsd.get() < 5280 && count < 3) {
        // If the LSD just detected a value less than 5280, update the timeSinceLSD variable
        count++;
		timeSinceLSD += 20;
    } else if (lsd.get() < 5280 && count >=3){
		timeSinceLSD = 0;
		count = 0;
	}
	else{
		timeSinceLSD += 20;
	}
	return timeSinceLSD;
}

void logData(){
// CURRENT CODE - UNTESTED
	// create vector of motor values
	std::vector<double> motor_speeds(8);
	motor_speeds[0] = mBRO.get_actual_velocity();
	motor_speeds[1] = mBRI.get_actual_velocity();
	motor_speeds[2] = mFRO.get_actual_velocity();
	motor_speeds[3] = mFRI.get_actual_velocity();
	motor_speeds[4] = mBLO.get_actual_velocity();
	motor_speeds[5] = mBLI.get_actual_velocity();
	motor_speeds[6] = mFLO.get_actual_velocity();
	motor_speeds[7] = mFLI.get_actual_velocity();

	std::vector<double> motor_positions(8);
	motor_positions[0] = mBRO.get_position();
	motor_positions[1] = mBRI.get_position();
	motor_positions[2] = mFRO.get_position();
	motor_positions[3] = mFRI.get_position();
	motor_positions[4] = mBLO.get_position();
	motor_positions[5] = mBLI.get_position();
	motor_positions[6] = mFLO.get_position();
	motor_positions[7] = mFLI.get_position();

	double LSD_TIME = updateLSDTime();
	//saves all data to sd card
	std::ofstream dataFile;
	dataFile.open("/usd/data.csv", std::ios_base::app);
	dataFile << float(filtered_average(motor_speeds)) << ", " << float(filtered_average(motor_positions)) << ", " << float(lsd.get()) << ", " << float(msd.get()) << ", " << float(bsd.get()) << ", " << float(LSD_TIME) << std::endl;
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
		double time_since_lsd = updateLSDTime();
		time_since_lsd = float(time_since_lsd);
		// load model and create input tensor
		model = Model::load("/usd/auton_blocker.model");
		Tensor in{1, 5};
		in.data_[0] = pos;
		in.data_[1] = Lsd;
		in.data_[2] = Msd;
		in.data_[3] = Bsd;
		in.data_[4] = time_since_lsd;

		// Run prediction
		Tensor out = model(in);
		double speed = out.data_[0];
		mBRO = speed;
		mBRI = speed;
		mFRO = speed;
		mFRI = speed;
		mBLO = speed;
		mBLI = speed;
		mFLO = speed;
		mFLI = speed;
	
		std::cout << speed << std::endl;
		master.clear();
		master.print(0,0, "Speed: %d", speed);
	} else{
		count ++;
	}
	
}