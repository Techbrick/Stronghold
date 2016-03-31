#include "Robot.h"

Robot::Robot() :
	position(),
	driveTrain(0, 1, 0, 0, 0, 0, &position),
	driveStick(1)
{

}

void Robot::OperatorControl()
{
	driveTrain.SetSafetyEnabled(false);
	SmartDashboard::PutNumber("kF", 0.0);
	SmartDashboard::PutNumber("kP", 0.0);
	SmartDashboard::PutNumber("kI", 0.0);
	SmartDashboard::PutNumber("kD", 0.0);

	while (IsOperatorControl() && IsEnabled())
	{
		float joystickY = driveStick.GetRawAxis(1);
		joystickY = (abs(joystickY) < 0.1) ? 0.0 : joystickY;
		SmartDashboard::PutNumber("joystickY", joystickY);
		driveTrain.TankDriveSpeed(joystickY*1000, joystickY*1000);
	}
	
	driveTrain.TankDrive(0.0, 0.0);
}

void Robot::Autonomous()
{
	driveTrain.SetSafetyEnabled(false);
	driveTrain.TurnToRelativeAngle(30);
}

START_ROBOT_CLASS(Robot);
