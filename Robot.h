#include "WPILib.h"

#include "Constants.h"
#include "Shooter.h"

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

class Robot : public SampleRobot {
	RobotDrive robotDrive;
	Joystick driveStick;
	Shooter shooter;
	Position position;
public:
	Robot();
	
	void OperatorControl();
};

#endif /* SRC_ROBOT_H_ */
