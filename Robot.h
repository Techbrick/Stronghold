#include "WPILib.h"

#include "Constants.h"
#include "Shooter.h"

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

class Robot : public SampleRobot {
	RobotDrive robotDrive;
	Joystick driveStick;
	Shooter shooter;
public:
	Robot();
	
	void OperatorControl();
};

#endif /* SRC_ROBOT_H_ */
