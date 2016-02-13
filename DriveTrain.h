#include "WPILib.h"

#ifndef SRC_DRIVETRAIN_H
#define SRC_DRIVETRAIN_H

class driveTrain : public RobotDrive
{
	CANTalon left, right;
public:
	driveTrain(uint32_t leftDeviceID, uint32_t rightDeviceID);

};

#endif