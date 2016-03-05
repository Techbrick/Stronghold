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

logger::logger(const char* msg_) : msg(msg_)
{
	std::cout << msg << std::endl;
}		

Robot::Robot() :
	l_1("logger 1"),
	driveTrain(Constants::driveLeftMasterID, Constants::driveLeftSlaveID, Constants::driveRightMasterID, Constants::driveRightSlaveID, &position),
	l_2("logger 2"),
	driveStick(Constants::driveJoystickChannel),
	l_3("logger 3"),
	shooter(Constants::shooterLeftTalonID, Constants::shooterRightTalonID, Constants::shooterAimTalonID, &position),
	l_4("logger 4"),
	position(),
	l_5("logger 5"),
	aimer(),
	l_6("logger 6")
{
	driveTrain.SetExpiration(0.1); // safety feature
}

void Robot::OperatorControl() //teleop code
{
	//CameraServer::GetInstance()->SetQuality(50);
	//CameraServer::GetInstance()->StartAutomaticCapture("cam0");

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
	std::cout << "Before main loop" << std::endl;

	while(IsOperatorControl() && IsEnabled())
	{
		std::cout << "Main loop" << std::endl;
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

void Robot::Test()
{
	uint32_t ID = 0;
	while (IsTest() && IsEnabled())
	{
		CANTalon *talon = new CANTalon(ID);
		if (driveStick.GetRawButton(3))
		{
			ID++;
			ID %= 16;
		}
		float testMove = -driveStick.GetRawAxis(1);
		talon->Set(testMove);

		SmartDashboard::PutNumber("Talon Analog Velocity", talon->GetAnalogInVel());
		SmartDashboard::PutNumber("Talon Front Switch", talon->IsFwdLimitSwitchClosed());
		SmartDashboard::PutBoolean("Talon Back Switch", talon->IsRevLimitSwitchClosed());
	}
}

START_ROBOT_CLASS(Robot);
