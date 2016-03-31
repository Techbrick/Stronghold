#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

#include "WPILib.h"
#include "DriveTrain.h"

class Robot : public SampleRobot {
	Position position;
	DriveTrain driveTrain;
	Joystick driveStick;
public:
	Robot();
	void OperatorControl();
	void Autonomous();
};

#endif /* SRC_ROBOT_H_ */
