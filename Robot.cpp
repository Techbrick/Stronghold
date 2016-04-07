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
	shooter(Constants::shooterLeftTalonID, Constants::shooterRightTalonID, Constants::shooterAimTalonID, Constants::shooterKickerTalonID, &position),
	position(),
	aimer()
{
	driveTrain.SetExpiration(0.1); // safety feature
	CameraServer::GetInstance()->SetQuality(50);
	CameraServer::GetInstance()->StartAutomaticCapture("cam0");
}

void Robot::RobotInit()
{
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

	SmartDashboard::PutNumber("auto_startPos", 5);
	SmartDashboard::PutNumber("auto_seconds", 2.5);
	SmartDashboard::PutNumber("auto_speed", 0.75);
	SmartDashboard::PutNumber("adjustSpeed", 1.0);

	shooter.Enable();
	driveTrain.Enable();

	while (IsOperatorControl() && IsEnabled())
	{
		throttle = (((driveStick.GetRawAxis(Constants::driveL2)) + 1.0)/4.0) + 0.5; //[1, .5]
		//leftMoveValue = .90 * throttle * driveStick.GetRawAxis(1);
		//rightMoveValue = -throttle * driveStick.GetRawAxis(5);
		leftMoveValue = -driveStick.GetRawAxis(1);
		rightMoveValue = -driveStick.GetRawAxis(5);

		if (driveStick.GetRawButton(8)) {
			leftMoveValue = -1 * leftMoveValue;
			rightMoveValue = -1 * rightMoveValue;
		}
		driveTrain.TankDriveSpeed(leftMoveValue, rightMoveValue);

		/*if (shooterPreparing)
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
		if (operatorStick.GetRawButton(Constants::prepareToShootButton))
		{
			//angleToTower = position.AngleToTower();
			//driveTrain.TurnToAngle(angleToTower);
			//Wait(2.0);
			angleToTower = aimer.GetAngleToTower();
			driveTrain.TurnToRelativeAngle(angleToTower);
		}
		if (operatorStick.GetRawButton(Constants::shootButton))
		{
			if (readyToShoot)
			{
				readyToShoot = false;
				shooter.Shoot();
			}
		}
		if (operatorStick.GetRawButton(5)) {
			angleToTower = aimer.GetAngleToTower();
			driveTrain.TurnToRelativeAngle(angleToTower);
			angleToShoot = aimer.GetAngleToShoot();
			shooter.SetAngle(angleToShoot);
		}
		if (operatorStick.GetRawButton(Constants::xButton))
		{
			shooter.Shoot();
		}
		float POVangle = driveStick.GetPOV();
		if (POVangle != -1)
		{
			driveTrain.TurnToAngle(POVangle);
		}
		float shooterAngleInput = -operatorStick.GetRawAxis(1);
		shooterAngleInput = abs(shooterAngleInput) > 0.005 ? shooterAngleInput : 0.0;
		shooter.Move(shooterAngleInput);

		if (operatorStick.GetRawButton(Constants::stopShooterWheels))
		{
			shooter.SetSpeed(0);
		}
		if (operatorStick.GetPOV() == 0)
		{
			shooter.SetSpeed(1);
		}
		if (operatorStick.GetPOV() == 270) {
			shooter.SetSpeed(.5);
		}
		if (operatorStick.GetPOV() == 180)
		{
			shooter.LoadBall();
		}
		if (operatorStick.GetPOV() == 90) {
			shooter.SetSpeed(.75);
		}
		if (operatorStick.GetRawButton(Constants::ejectButton)) {
			shooter.SetSpeed(.5);
			Wait(.5);
			shooter.Shoot();
			Wait(.5);
			shooter.SetSpeed(0);
		}

		int testAngle = 33;
		if(operatorStick.GetRawButton(4)) {
			testAngle++;
			shooter.SetAngle(testAngle);
		}
		SmartDashboard::PutNumber("Test Angle", testAngle);
		SmartDashboard::PutNumber("Pot Value", shooter.ReadPot());
		/*if (operatorStick.GetRawButton(5)) {
			shooter.SetAngle(20);
		}
		if (operatorStick.GetRawButton(6)) {
			shooter.SetAngle(50);
		}*/
		/*if (operatorStick.GetRawButton(3)) {
			driveTrain.TurnToAngle(40);
			std::cout << "Button Pressed";
		}
		if (operatorStick.GetRawButton(4)) {
			driveTrain.TurnToRelativeAngle(30);
		}*/
		SmartDashboard::PutBoolean("Has Ball", shooter.HasBall());
		SmartDashboard::PutBoolean("Target Acquired", aimer.GetAge() < 3);
		SmartDashboard::PutBoolean("Aimed", abs(aimer.GetAngleToTower()) < 3.0);
		SmartDashboard::PutBoolean("Angled", abs(shooter.Angle() - aimer.GetAngleToShoot()) < 5.0);
		SmartDashboard::PutNumber("Shooter Angle", shooter.Angle());
		SmartDashboard::PutNumber("Recommended Shooter Angle", aimer.GetAngleToShoot());
		SmartDashboard::PutNumber("Angle to Tower", aimer.GetAngleToTower());
		SmartDashboard::PutNumber("Rotation", position.GetAngleDegrees());
		SmartDashboard::PutBoolean("Close Enough to Shoot", aimer.GetAngleToShoot() < 48 && aimer.GetAngleToShoot() > 32);
		SmartDashboard::PutNumber("Shooter Speed", shooter.WheelSpeed());
	}

	shooter.Disable();
	driveTrain.Disable();

	updateThreadRun = false;
	//updateThread.join();

	driveTrain.SetSafetyEnabled(true);
}

void Robot::Autonomous()
{
	position.ZeroYaw();
	driveTrain.SetSafetyEnabled(false);
	driveTrain.Enable();
	shooter.Enable();
	//bool updateThreadRun = true;
	//std::thread updateThread(autoUpdateThreadFunction, &updateThreadRun, &autoPosition);
	std::fstream logfile; logfile.open("logfile.txt", std::fstream::out);
	//Timer timer;
	//timer.Start();
	//timer.Reset();
	//std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("datatable");
	int startPos = (int)(SmartDashboard::GetNumber("auto_startPos", 5)); //table->GetNumber("startPos", 5);
	float timeTo10 = SmartDashboard::GetNumber("auto_seconds", 2.5); //table->GetNumber("timeTo10", 2.5);
	float over9000 = SmartDashboard::GetNumber("auto_speed", 0.75); //table->GetNumber("powerLevel", 0.75);
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
		float speed = SmartDashboard::GetNumber("adjustSpeed", 1.0);
	driveTrain.TankDrive(-over9000*speed, over9000);
	Wait(timeTo10);
	driveTrain.TankDrive(0.0, 0.0);
	//turn 180 unless it's at the ends then turn  145
	logfile << "Twist'n, baby!"<< std::endl;
	float fraction = 1;
	if (startPos == 1 || startPos == 5)
	{
		if(startPos == 1)
			fraction = 215;
		else fraction = -145;
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
	/*if (position.GetAngleDegrees() < fraction) {
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
	}*/
	driveTrain.TurnToAngle(fraction);
	driveTrain.TankDrive(0.0, 0.0);

	float angleToTower = aimer.GetAngleToTower();
	driveTrain.TurnToRelativeAngle(angleToTower);

	driveTrain.ArcadeDrive(0.0, 0.0, false);
	//aim at tower
	int age = aimer.GetAge();
	if/* (false)*/(age < 3)
	{
		float angleToShoot = aimer.GetAngleToShoot();
		shooter.SetAngle(angleToShoot);
		shooter.SetSpeed(1.0);
		logfile << "Angle to shoot: " << angleToShoot << std::endl;
		Wait(1);
		shooter.Shoot();
	}
	for (int i=0; i<20; i++)
		std::cout << "position: " << position.GetAngleDegrees() << std::endl;
	//shooter.SetAngle(34);
	shooter.Disable();
	driveTrain.Disable();
	logfile << "Auto ended" << std::endl;
	logfile.close();
}
START_ROBOT_CLASS(Robot);
