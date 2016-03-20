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
	Arm arm;
	Servo servo;
public:
	Robot();

	void OperatorControl();
	void Autonomous();
};

#endif /* SRC_ROBOT_H_ */
