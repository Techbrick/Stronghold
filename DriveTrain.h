#include "WPILib.h"

#ifndef SRC_DRIVETRAIN_H
#define SRC_DRIVETRAIN_H

class DriveTrain : public RobotDrive
{
	CANTalon left, right;
public:
	DriveTrain(uint32_t leftDeviceID, uint32_t rightDeviceID);

};

#endif