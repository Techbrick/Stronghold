#include "Robot.h"

void updateThreadFunction(bool *keepRunning, Joystick *driveStick, Position *position) { //will need to be updated if drivesticks are updated
	//TODO: Incorporate the cantalon to be able to sent the ticks
	bool movingForward = false;
	while (*keepRunning == true)
	{
		if (driveStick->GetY() > 0)
		{
			movingForward = true;
		}
		if (!position->IsTurning())
		{
			//position->Update(movingForward);
			Wait(.02);
		}
	}
}

void autoUpdateThreadFunction(bool *keepRunning, Position *position) { //will need to be updated if drivesticks are updated
	//TODO: Incorporate the cantalon to be able to sent the ticks
	if (!position->IsTurning())
	{
		//position->Update(movingForward);
		Wait(.02);
	}
}

Robot::Robot() :
	driveTrain(Constants::driveLeftMasterID, Constants::driveLeftSlaveID, Constants::driveRightMasterID, Constants::driveRightSlaveID, &position),
	driveStick(Constants::driveJoystickChannel),
	operatorStick(1),
	shooter(Constants::shooterLeftTalonID, Constants::shooterRightTalonID, Constants::shooterAimTalonID, &position),
	position(),
	aimer(),
	arm(Constants::armTalonPin),
	servo(0)
{
	driveTrain.SetExpiration(0.1); // safety feature
	CameraServer::GetInstance()->SetQuality(50);
	CameraServer::GetInstance()->StartAutomaticCapture("cam0");
}

void Robot::OperatorControl() //teleop code
{
	driveTrain.SetSafetyEnabled(false);
	bool updateThreadRun = true;
	//std::thread updateThread(updateThreadFunction, &updateThreadRun, &driveStick, &position);
	float throttle;
	float leftMoveValue;
	float rightMoveValue;
	float rotateValue;
	float distToTower;
	float angleToShoot;
	float angleToTower;
	bool readyToShoot = false;
	bool shooterPreparing = false;

	shooter.Enable();
	driveTrain.Enable();
	arm.Enable();

	while (IsOperatorControl() && IsEnabled())
	{
		throttle = (((driveStick.GetRawAxis(Constants::driveL2)) + 1.0)/4.0) + 0.5; //[1, .5]
		leftMoveValue = throttle * driveStick.GetRawAxis(1);
		rightMoveValue = -throttle * driveStick.GetRawAxis(5);

		SmartDashboard::PutNumber("Throttle Value", throttle);
		SmartDashboard::PutNumber("Left Move Value", leftMoveValue);
		SmartDashboard::PutNumber("Right Move Value", rightMoveValue);
		driveTrain.TankDrive(leftMoveValue, rightMoveValue, false);

		/*		if (shooterPreparing)
				{
				readyToShoot = (abs(shooter.WheelSpeed() - 1.0) < 0.01) && (abs(shooter.Angle() - angleToTower) < 0.1);

				if (readyToShoot)
				{
				shooterPreparing = false;
				}
				}
				*/
		if (driveStick.GetRawButton(Constants::calibrateButton))
		{
			//position.Calibrate();
		}
		/*		if (operatorStick.GetRawButton(Constants::prepareToShootButton))
				{
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
				*/
		if (operatorStick.GetRawButton(Constants::shootButton))
		{
			if (readyToShoot)
			{
				readyToShoot = false;
				shooter.Shoot();
			}
		}
		if (operatorStick.GetRawButton(Constants::xButton))
		{
			shooter.SetSpeed(1);
			Wait(.5);
			shooter.Shoot();
			Wait(.5);
			shooter.SetSpeed(0);
		}
		float shooterAngleInput = -operatorStick.GetRawAxis(1);
		shooterAngleInput = abs(shooterAngleInput) > 0.005 ? shooterAngleInput : 0.0;
		shooter.Move(shooterAngleInput);

		float manualMoveBeltInput = operatorStick.GetRawAxis(5);
		manualMoveBeltInput = abs(manualMoveBeltInput) > 0.005 ? manualMoveBeltInput : 0.0;
		arm.ManualMoveBelt(manualMoveBeltInput / 5);

		if (operatorStick.GetRawButton(Constants::stopShooterWheels))
		{
			shooter.SetSpeed(0);
		}
		if (operatorStick.GetPOV() == 0)
		{
			shooter.SetSpeed(1);
		}
		else if (operatorStick.GetPOV() == 180)
		{
			shooter.LoadBall();
		}
		if (operatorStick.GetRawButton(Constants::ejectButton)) {
			shooter.SetSpeed(.4);
			Wait(.05);
			shooter.Shoot();
		}
		/*if (operatorStick.GetRawButton(5)) {
			shooter.SetAngle(20);
		}
		if (operatorStick.GetRawButton(6)) {
			shooter.SetAngle(50);
		}
		if (operatorStick.GetRawButton(3)) {
			driveTrain.TurnToAngle(30);
		}*/
		/*if (operatorStick.GetRawButton(3)) {
			driveTrain.TurnToRelativeAngle(30);
		}*/
		SmartDashboard::PutNumber("getPOV", operatorStick.GetPOV());
		SmartDashboard::PutString("Version", "1.1");
		SmartDashboard::PutBoolean("Has Ball", shooter.HasBall());
		SmartDashboard::PutBoolean("Target Acquired", aimer.GetAge() < 3);
		SmartDashboard::PutBoolean("Aimed", abs(aimer.GetAngleToTower()) < 3.0);
		SmartDashboard::PutBoolean("Angled", abs(shooter.Angle() - aimer.GetAngleToShoot()) < 5.0);
		SmartDashboard::PutNumber("Shooter Angle", shooter.Angle());
		SmartDashboard::PutNumber("Angle to Tower", aimer.GetAngleToTower());
	}

	shooter.Disable();
	driveTrain.Disable();
	arm.Disable();

	updateThreadRun = false;
	//updateThread.join();

	driveTrain.SetSafetyEnabled(true);
}

void Robot::Autonomous()
{
	driveTrain.SetSafetyEnabled(false);
	driveTrain.Enable();
	shooter.Enable();
	//bool updateThreadRun = true;
	//std::thread updateThread(autoUpdateThreadFunction, &updateThreadRun, &autoPosition);
	std::fstream logfile; logfile.open("logfile.txt", std::fstream::out);
	//Timer timer;
	//timer.Start();
	//timer.Reset();
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("datatable");
	int startPos = table->GetNumber("startPos", 5);
	float timeTo10 = table->GetNumber("timeTo10", 3);
	float over9000 = table->GetNumber("powerLevel", 0.75);
	logfile << startPos << " " << timeTo10 << " " << over9000 << std::endl;
	//  .net rules!!!!!!!!!!!!
		int failSafe = 0;
		if(startPos == 0|| timeTo10 == 0||over9000 == 0)
		{
			logfile<<"ERROR.  RETURNING WITHOUT REGRETS BECAUSE ONE OF THE VALUES WAS SET TO 0!!!"<<std::endl;
			return;
		}
		//drive over defense
		logfile<<"Over the mountain" << std::endl;
	driveTrain.TankDrive(-over9000 - .2, over9000 + .185);
	Wait(timeTo10);
	driveTrain.TankDrive(0.0, 0.0);
	//turn 180 unless it's at the ends then turn  145
	logfile << "Twist'n, baby!"<< std::endl;
	float fraction = 1;
	if (startPos == 1 || startPos == 5)
	{
		if(startPos == 1)
			fraction = 215;
		else fraction = 145;
	}else fraction = 180;
	//timer.Reset();
	float timeToTurn = .75;
	//driveTrain.ArcadeDrive(0.0, fraction / 180, false);
	/*while(timer.Get() <= timeToTurn && failSafe < 200)
	{
		Wait(0.01);
		failSafe++;
		logfile<<"loop 1: "<<failSafe;

	}*/
	auto failsafe = 0;
	if (position.GetAngleDegrees() < fraction) {
		while (position.GetAngleDegrees() < fraction && failsafe < 500) {
			driveTrain.TankDrive(.25, .25);
			Wait(0.01);
			failsafe++;
		}
	} else if (position.GetAngleDegrees() > fraction) {
		while (position.GetAngleDegrees() > fraction && failsafe < 500) {
			driveTrain.TankDrive(-.25, -.25);
			Wait(0.01);
			failsafe++;
		}
	}
	driveTrain.TankDrive(0.0, 0.0);

	float angleToTower = aimer.GetAngleToTower();
	driveTrain.TurnToRelativeAngle(angleToTower);

	driveTrain.ArcadeDrive(0.0, 0.0, false);
	//aim at tower
	int age = aimer.GetAge();
	if (age < 3)
	{
		float angleToShoot = aimer.GetAngleToShoot();
		shooter.SetAngle(angleToShoot);
		shooter.SetSpeed(1.0);
		logfile << "Angle to shoot: " << angleToShoot << std::endl;
		Wait(0.5);
		shooter.Shoot();
	}

	shooter.Disable();
	driveTrain.Disable();
	logfile << "Auto ended" << std::endl;
	logfile.close();
}
START_ROBOT_CLASS(Robot);
