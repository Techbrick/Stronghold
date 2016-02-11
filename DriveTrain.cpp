#include "DriveTrain.h"

DriveTrain::DriveTrain (uint32_t leftMotorChannel, uint32_t rightMotorChannel)
	: leftTalon(leftMotorChannel), rightTalon(rightMotorChannel), RobotDrive(leftTalon, rightTalon)
{}

void DriveTrain::EnableTalons()
{
	if (leftTalon.IsEnabled() == false)
		leftTalon.Enable();
		
	if (rightTalon.IsEnabled() == false)
		rightTalon.Enable();
}

void DriveTrain::DisableTalons ()
{
	if (leftTalon.IsEnabled())
		leftTalon.Enable();

	if (rightTalon.IsEnabled())
		rightTalon.Disable();
}