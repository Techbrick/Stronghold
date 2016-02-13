#include "WPILib.h"

#include "DriveTrain.h"
#include "Constants.h"
#include "Shooter.h"

#include <math.h>
#include <thread>

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

class Robot : public SampleRobot {
	DriveTrain driveTrain;
	Joystick driveStick;
	Shooter shooter;
public:
	Robot();
	
	void OperatorControl();
};

#endif /* SRC_ROBOT_H_ */
