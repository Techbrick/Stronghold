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
		if (driveStick.GetRawButton(Constants::prepareToShootButton)) {// TODO: thread this
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
		if (driveStick.GetRawButton(Constants::shootButton)) {
			if (readyToShoot)
			{
				readyToShoot = false;
				shooter.Shoot();
			}
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

<<<<<<< Updated upstream
void Robot::Test()
{
	uint32_t ID = 0;
	CANTalon *talon = new CANTalon(ID);
	bool buttonDown = false;
	while (IsEnabled())
	{
		if (driveStick.GetRawButton(4) && buttonDown == false)
		{
			std::cout << "Button!" << std::endl;
			ID++;
			ID %= 16;
			delete talon;
			talon = new CANTalon(ID);
			buttonDown = true;
		}

		if (driveStick.GetRawButton(4) == false)
		{
			buttonDown = false;
		}

		if (driveStick.GetRawButton(Constants::xButton)) {
			servo.Set(.6);
			Wait(2);
			servo.Set(.8);
		}

		float testMove = -driveStick.GetRawAxis(5);
		talon->Set(testMove);

		SmartDashboard::PutBoolean("Button 3", driveStick.GetRawButton(4));
		SmartDashboard::PutNumber("Talon ID", ID);
		SmartDashboard::PutNumber("Talon Encoder (position)", talon->GetEncPosition());
		SmartDashboard::PutNumber("Talon Front Switch", talon->IsFwdLimitSwitchClosed());
		SmartDashboard::PutBoolean("Talon Back Switch", talon->IsRevLimitSwitchClosed());
	}

	delete talon;
=======
void Robot::Test() {
	uint32_t ID = 0;
	while (IsTest() && IsEnabled()) {
		CANTalon *talon = new CANTalon(ID);
		if (driveStick.GetRawButton(3)) {
			ID++;
			ID %= 16;
		}
		float testMove = -driveStick.GetRawAxis(1);
		talon->Set(testMove);

		SmartDashboard::PutNumber("Talon Analog Velocity", talon->GetAnalogInVel());
		SmartDashboard::PutNumber("Talon Front Switch", talon->IsFwdLimitSwitchClosed());
		SmartDashboard::PutBoolean("Talon Back Switch", talon->IsRevLimitSwitchClosed());
	}
>>>>>>> Stashed changes
}

START_ROBOT_CLASS(Robot);
