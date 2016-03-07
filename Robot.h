#include "WPILib.h"

#include "DriveTrain.h"
#include "Constants.h"
#include "Shooter.h"
#include "Position.h"
#include "Aimer.h"

#include <exception>
#include <math.h>
#include <thread>
#include <string>

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

class logger {
	std::string msg;
public:
	logger(const char* msg_);
};

class Robot : public SampleRobot {
	logger l_1;
	DriveTrain driveTrain;
	logger l_2;
	Joystick driveStick;
	logger l_3;
	Shooter shooter;
	logger l_4;
	Position position;
	logger l_5;
	Aimer aimer;
	logger l_6;
public:
	Robot();

	void OperatorControl();
};

#endif /* SRC_ROBOT_H_ */
