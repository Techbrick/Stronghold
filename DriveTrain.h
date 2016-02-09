#ifndef SRC_DRIVETRAIN_H
#define SRC_DRIVETRAIN_H

#include "WPILib.h"

class DriveTrain : public RobotDrive
{
	DriveTrain (uint32_t leftMotorChannel, uint32_t rightMotorChannel);
	DriveTrain (uint32_t frontLeftMotorChannel, uint32_t rearLeftMotorChannel, uint32_t frontRightMotorChannel, uint32_t rearRightMotorChannel);
	DriveTrain (SpeedController *leftMotor, SpeedController *rightMotor);
	DriveTrain (SpeedController *frontLeftMotor, SpeedController *rearLeftMotor, SpeedController *frontRightMotor, SpeedController *rearRightMotor);
	DriveTrain (SpeedController &frontLeftMotor, SpeedController &rearLeftMotor, SpeedController &frontRightMotor, SpeedController &rearRightMotor);
}

#endif