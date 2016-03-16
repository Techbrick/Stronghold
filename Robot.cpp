#include "Robot.h"

void updateThreadFunction(bool *keepRunning, Joystick *driveStick, Position *position) { //will need to be updated if drivesticks are updated
	//TODO: Incorporate the cantalon to be able to sent the ticks
	bool movingForward = false;
	while (*keepRunning == true) {
		if (driveStick->GetY() > 0) {
			movingForward = true;
		}
		if (!position->IsTurning()) {
			//position->Update(movingForward);
			Wait(.02);
		}
	}
}

Robot::Robot() :
	driveTrain(Constants::driveLeftMasterID, Constants::driveLeftSlaveID, Constants::driveRightMasterID, Constants::driveRightSlaveID, &position),
	driveStick(Constants::driveJoystickChannel),
	operatorStick(1),
	shooter(Constants::shooterLeftTalonID, Constants::shooterRightTalonID, Constants::shooterAimTalonID, &position),
	position(),
	aimer(),
	servo(0)
{
	driveTrain.SetExpiration(0.1); // safety feature
}

void Robot::RobotInit()
{
	CameraServer::GetInstance()->SetQuality(50);
	CameraServer::GetInstance()->StartAutomaticCapture("cam0");
}

void Robot::OperatorControl() //teleop code
{
	bool updateThreadRun = true;
	std::thread updateThread(updateThreadFunction, &updateThreadRun, &driveStick, &position);
	float throttle;
	float moveValue;
	float rotateValue;
	float distToTower;
	float angleToShoot;
	float angleToTower;
	bool readyToShoot = false;
	bool shooterPreparing = false;

	shooter.Enable();
	driveTrain.Enable();

	while(IsOperatorControl() && IsEnabled())
	{
		throttle = (((-driveStick.GetRawAxis(Constants::driveL2)) + 1.0)/4.0) + 0.5; //[0, 1]
		moveValue = throttle * driveStick.GetY();
		rotateValue = -driveStick.GetX();

		SmartDashboard::PutNumber("Throttle Value", throttle);
		SmartDashboard::PutNumber("Move Value", moveValue);
		SmartDashboard::PutNumber("Rotate Value", rotateValue);
		
		driveTrain.ArcadeDrive(moveValue, rotateValue, true);

		if (shooterPreparing)
		{
			readyToShoot = (abs(shooter.WheelSpeed() - 1.0) < 0.01) && (abs(shooter.Angle() - angleToTower) < 0.1);

			if (readyToShoot)
			{
				shooterPreparing = false;
			}
		}

		if (driveStick.GetRawButton(Constants::calibrateButton)) {
			//position.Calibrate();
		}
		if (operatorStick.GetRawButton(Constants::prepareToShootButton)) {// TODO: thread this
			if (shooter.HasBall())
			{
				shooterPreparing = true;
				shooter.PrepareShooter();	
				angleToTower = position.AngleToTower();
				driveTrain.TurnToRelativeAngle(angleToTower);
				angleToTower = aimer.GetAngleToTower();
				driveTrain.TurnToRelativeAngle(angleToTower);
				angleToShoot = aimer.GetAngleToShoot();
				distToTower = aimer.GetDistanceToTower();
				shooter.PrepareShooter(angleToShoot, 1.0);
			}
		}
		if (operatorStick.GetRawButton(Constants::shootButton)) {
			if (readyToShoot)
			{
				readyToShoot = false;
				shooter.Shoot();
			}
		}
		if (abs(operatorStick.GetRawAxis(1)) > .05) {
			shooter.Move(operatorStick.GetRawAxis(1));
		}
		if (abs(operatorStick.GetRawAxis(1) < .05)) {
			shooter.Move(0);
		}
		SmartDashboard::PutNumber("xPos", position.GetX());
		SmartDashboard::PutNumber("yPos", position.GetY());
		SmartDashboard::PutString("Version", "0.9");
	}

	shooter.Disable();
	driveTrain.Disable();
	
	updateThreadRun = false;
	updateThread.join();
	
	driveTrain.SetSafetyEnabled(true);
}

START_ROBOT_CLASS(Robot);
