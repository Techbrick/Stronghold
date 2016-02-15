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
		Wait(.02);
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
	float throttle;
	float moveValue;
	float rotateValue;
	float shooterSpeed;
	float shooterAngle;
	
	shooter.Enable();
	driveTrain.Enable();
	
	while(IsOperatorControl() && IsEnabled())
	{
		throttle = (((-driveStick.GetRawAxis(Constants::driveL2)) + 1.0)/2.0); //[0, 1]
		moveValue = throttle * driveStick.GetY();
		rotateValue = -driveStick.GetX();
		shooterSpeed = -driveStick.GetRawAxis(Constants::driveRightStickY); //change
		shooterAngle = 0; //change
		
		SmartDashboard::PutNumber("Throttle Value", throttle);
		SmartDashboard::PutNumber("Move Value", moveValue);
		SmartDashboard::PutNumber("Rotate Value", rotateValue);
		
		float shooterSpeed = -driveStick.GetRawAxis(Constants::driveRightStickY);
		if (shooterSpeed < -0.35) {
			shooterSpeed = -0.35;
		}
		
		driveTrain.ArcadeDrive(moveValue, rotateValue, true);
		shooter.setSpeed(shooterSpeed, -shooterSpeed);
		
		if (driveStick.GetRawButton(Constants::calibrateButton)) {
			position.Calibrate();
		}
		if (driveStick.GetRawButton(Constants::shootButton)) {
			if (!shooter.HasBall()) {
				shooter.LoadBall();
			}
			if (shooter.HasBall()) {
				shooter.PrepareShooter(shooterAngle, shooterSpeed); //TODO: Change these. Get physics major's equation
			}
			Wait(1); //how long to wait to shoot. Will need to be adjusted with testing
			if (shooter.Angle() > shooterAngle - 2 && shooter.Angle() < shooterAngle + 2 && shooter.WheelSpeed() == shooterSpeed) {
				shooter.Shoot();
			}
		}
		SmartDashboard::PutNumber("xPos", position.GetX());
		SmartDashboard::PutNumber("yPos", position.GetY());
		SmartDashboard::PutString("Version", "Noah 2/14/16, 11:16 PM")
	}
	
	shooter.Disable();
	driveTrain.Disable();
	
	testThreadRun = false;
	testThread.join();
	updateThreadRun = false;
	updateThread.join();
	
	driveTrain.SetSafetyEnabled(true);
}

START_ROBOT_CLASS(Robot);
