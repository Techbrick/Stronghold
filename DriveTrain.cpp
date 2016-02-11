#include "DriveTrain.h"

DriveTrain::DriveTrain (uint32_t leftMotorChannel, uint32_t rightMotorChannel)
	: RobotDrive(leftMotorChannel, rightMotorChannel)
{}

DriveTrain::DriveTrain (uint32_t frontLeftMotorChannel, uint32_t rearLeftMotorChannel, uint32_t frontRightMotorChannel, uint32_t rearRightMotorChannel)
	: RobotDrive (frontLeftMotorChannel, rearLeftMotorChannel, frontRightMotorChannel, rearRightMotorChannel) 
{}

DriveTrain::DriveTrain (SpeedController *leftMotor, SpeedController *rightMotor)
	: RobotDrive (leftMotor, rightMotor)
{}
DriveTrain::DriveTrain (SpeedController *frontLeftMotor, SpeedController *rearLeftMotor, SpeedController *frontRightMotor, SpeedController *rearRightMotor)
	: RobotDrive (frontLeftMotor, rearLeftMotor, frontRightMotor, rearRightMotor)
{}

DriveTrain::DriveTrain (SpeedController &frontLeftMotor, SpeedController &rearLeftMotor, SpeedController &frontRightMotor, SpeedController &rearRightMotor)
	: RobotDrive (frontLeftMotor, rearLeftMotor, frontRightMotor, rearRightMotor)
{}
