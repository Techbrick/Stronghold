#include "WPILib.h"

#include "DriveTrain.h"
#include "Constants.h"
#include "Shooter.h"
#include "Position.h"
#include "Aimer.h"
#include "Arm.h"

#include <math.h>
#include <thread>
#include <fstream>

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

class Robot : public SampleRobot {
	DriveTrain driveTrain;
	Joystick driveStick;
	Joystick operatorStick;
	Shooter shooter;
	Position position;
	Aimer aimer;
	CANTalon testCANTalon;
	Servo servo;
	Servo servo1;
	Servo servo2;
	Servo servo3;
	Servo servo4;
	Servo servo5;
	Servo servo6;
	Servo servo7;
	Servo servo8;
	Servo servo9;
	int pwmPin;
public:
	Robot();
	void RobotInit();
	void OperatorControl();
	void Autonomous();
};

#endif /* SRC_ROBOT_H_ */
