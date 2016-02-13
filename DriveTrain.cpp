#include "DriveTrain.h"

driveTrain::driveTrain(uint32_t leftDeviceID, uint32_t rightDeviceID)
	: left(leftDeviceID), right(rightDeviceID), RobotDrive(left, right)
{
}