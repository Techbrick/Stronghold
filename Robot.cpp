#include "Robot.h"

void threadTestFunction(bool* keepRunning)
{
	int a = 0;
	while (*keepRunning == true)
	{
		a++; a %= 10;
		SmartDashboard::PutNumber("SmartDash Number", a);
		Wait(1);
	}
	
	SmartDashboard::PutNumber("SmartDash Number", 99999);
}

Robot::Robot() :
	driveTrain(Constants::driveLeftTalonID, Constants::driveRightTalonID),
	driveStick(Constants::driveJoystickChannel),
	shooter(Constants::shooterLeftTalonID, Constants::shooterRightTalonID)
{
	driveTrain.SetExpiration(0.1); // safety feature
}
	
void Robot::OperatorControl() //teleop code
{
	CameraServer::GetInstance()->SetQuality(50);
	CameraServer::GetInstance()->StartAutomaticCapture("cam0");

	bool testThreadRun = true;
	std::thread testThread(threadTestFunction, &testThreadRun);
	
	shooter.Enable();
	
	while(IsOperatorControl() && IsEnabled())
	{
		float throttle = (((-driveStick.GetRawAxis(Constants::driveL2)) + 1.0)/2.0); //[0, 1]
		float moveValue = throttle * driveStick.GetY();
		float rotateValue = -driveStick.GetX();
		
		SmartDashboard::PutNumber("Throttle Value", throttle);
		SmartDashboard::PutNumber("Move Value", moveValue);
		SmartDashboard::PutNumber("Rotate Value", rotateValue);
		
		driveTrain.ArcadeDrive(moveValue, rotateValue, true);
		shooter.setSpeed(-driveStick.GetRawAxis(Constants::driveRightStickY), driveStick.GetRawAxis(Constants::driveRightStickY));
	}
	
	shooter.Disable();
	
	testThreadRun = false;
	testThread.join();
	
	driveTrain.SetSafetyEnabled(true);
}

START_ROBOT_CLASS(Robot);
