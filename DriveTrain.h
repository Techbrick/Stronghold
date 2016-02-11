#ifndef SRC_DRIVETRAIN_H
#define SRC_DRIVETRAIN_H

#include "WPILib.h"

class DriveTrain : public RobotDrive
{
	CANTalon leftTalon, rightTalon;

public:
	DriveTrain (uint32_t leftMotorChannel, uint32_t rightMotorChannel);
	
	void EnableTalons();
	
	void DisableTalons();
	
}

#endif