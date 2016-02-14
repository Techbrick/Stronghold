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

void updateThreadFunction(bool *keepRunning, Joystick *driveStick, Position *position) { //will need to be updated if drivesticks are updated
	bool movingForward = false;
	while (*keepRunning == true) {
		if (driveStick->GetY() > 0) {
			movingForward = true;
		}
		position->Update(movingForward);
	}
}

Robot::Robot() :
	driveTrain(Constants::driveLeftTalonID, Constants::driveRightTalonID),
	driveStick(Constants::driveJoystickChannel),
	shooter(Constants::shooterLeftTalonID, Constants::shooterRightTalonID, Constants::shooterAimTalonID),
	position()
{
	driveTrain.SetExpiration(0.1); // safety feature
}
	
void Robot::OperatorControl() //teleop code
{
	CameraServer::GetInstance()->SetQuality(50);
	CameraServer::GetInstance()->StartAutomaticCapture("cam0");

	bool testThreadRun = true;
	bool updateThreadRun = true;
	std::thread testThread(threadTestFunction, &testThreadRun);
	std::thread updateThread(updatePositionFunction, &updateThreadRun, &driveStick, &position);

	shooter.Enable();
	
	while(IsOperatorControl() && IsEnabled())
	{
		float throttle = (((-driveStick.GetRawAxis(Constants::driveL2)) + 1.0)/2.0); //[0, 1]
		float moveValue = throttle * driveStick.GetY();
		float rotateValue = -driveStick.GetX();
		
		SmartDashboard::PutNumber("Throttle Value", throttle);
		SmartDashboard::PutNumber("Move Value", moveValue);
		SmartDashboard::PutNumber("Rotate Value", rotateValue);
		
		float shooterSpeed = -driveStick.GetRawAxis(Constants::driveRightStickY);
		if (shooterSpeed < -0.35)
			shooterSpeed = -0.35;
		
		driveTrain.ArcadeDrive(moveValue, rotateValue, true);
		shooter.setSpeed(shooterSpeed, -shooterSpeed);
	}
	
	shooter.Disable();
	
	testThreadRun = false;
	testThread.join();
	updateThreadRun = false;
	updateThread.join();
	
	driveTrain.SetSafetyEnabled(true);
}

START_ROBOT_CLASS(Robot);
