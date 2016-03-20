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

		float manualMoveBeltInput = -operatorStick.GetRawAxis(5);
		manualMoveBeltInput = abs(manualMoveBeltInput) > 0.005 ? manualMoveBeltInput : 0.0;
		arm.ManualMoveBelt(manualMoveBeltInput);

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
		if (operatorStick.GetRawButton(Constants::ejectButton))
		{
			shooter.SetSpeed(.35);
			Wait(.05);
			shooter.Shoot();
		}
		if (operatorStick.GetRawButton(5))
		{
			shooter.SetAngle(20);
		}
		if (operatorStick.GetRawButton(6))
		{
			shooter.SetAngle(50);
		}
		SmartDashboard::PutNumber("getPOV", operatorStick.GetPOV());
		SmartDashboard::PutNumber("xPos", position.GetX());
		SmartDashboard::PutNumber("yPos", position.GetY());
		SmartDashboard::PutString("Version", "0.99");
		shooter.ReadPot();
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
	//bool updateThreadRun = true;
	//std::thread updateThread(autoUpdateThreadFunction, &updateThreadRun, &autoPosition);
	std::fstream logfile; logfile.open("logfile.txt", std::fstream::out);
	//Timer timer;
	//timer.Start();
	//timer.Reset();
	std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("datatable");
	int startPos = table->GetNumber("startPos", 1);
	float timeTo10 = table->GetNumber("timeTo10", 3);
	float over9000 = table->GetNumber("powerLevel", 0.75);
	logfile << startPos << " " << timeTo10 << " " << over9000 << std::endl;
	//  .net rules!!!!!!!!!!!!
	int failSafe = 0;
	if (startPos == 0 || timeTo10 == 0 || over9000 == 0)
	{
		logfile << "ERROR.  RETURNING WITHOUT REGRETS BECAUSE ONE OF THE VALUES WAS SET TO 0!!!" << std::endl;
		return;
	}
	//drive over defense
	logfile << "Over the mountain" << std::endl;
	if (timeTo10 < 8) //TODO: TJ, should this really be less-than???
		timeTo10 = 8;
	driveTrain.ArcadeDrive(over9000, 0, false);
	Wait(timeTo10);
	driveTrain.ArcadeDrive(0.0, 0.0, false);
	//turn 180 unless it's at the ends then turn  145
	logfile << "Twist'n, baby!"<< std::endl;
	float fraction = 1;
	if (startPos == 1 || startPos == 5)
	{
		if (startPos == 1)
			fraction = 145;
		else fraction = 215;
	}
	else fraction = 180;
	//timer.Reset();
	/*
	driveTrain.ArcadeDrive(0.0, (over9000 / 2)*fraction, false);
	while (timer.Get() <= timeToTurn && failSafe < 200)
	{
	        Wait(0.01);
	        failSafe++;
	        logfile << "loop 1: " << failSafe;
	}
	*/
	float output1;
	double angleToTurn = fraction; //tj
	double startAngle = position.GetAngle();
	double currentAngle = position.GetAngle() - startAngle;
	while (abs((position.GetAngle() - startAngle) - angleToTurn) > 2 && failSafe <= 500)
	{
		if (position.GetAngle() >= 10)
		{
			output1 = 0.8;
		}
		else if (currentAngle < -10)
		{
			output1 = -0.8;
		}
		else if (currentAngle > 0 && currentAngle < 10)
		{
			output1 = currentAngle*0.06+0.2;
		}
		else if (currentAngle < 0 && currentAngle > -10)
		{
			output1 = -(currentAngle*0.06+0.2);
		}
		currentAngle = position.GetAngle();
		logfile << "Calculated Output 2:" << output1 << std::endl;
		logfile << "Azimuth 2:" << currentAngle << std::endl;
		driveTrain.TankDrive(output1, -output1);
		Wait(0.01);
		failSafe++;
		currentAngle = position.GetAngle();
		logfile << "loop 3:" << failSafe << std::endl;
		//turn
	}
	if (failSafe >=500)
	{
		driveTrain.Disable();
		shooter.Disable();

		logfile << "FAIL SAFE ACTIVATED!! WEEOO WEEOO!" << std::endl;
		return;
	}
	failSafe = 0;
	driveTrain.ArcadeDrive(0.0, 0.0, false);
	//turn to tower
	//logfile << "turning to tower..."<< std::endl;
	std::cout << "VALUES TO OLD!!!" << std::endl;
	float azimuth = aimer.GetAngleToTower();
	float output = 0;
	//  this would be so much easier in c#
	//driveTrain.TurnToRelativeAngle(azimuth);
	//  if the image is fresh then turn slow so azimuth between -1 and 1
	while (abs(azimuth) > 2.0 && failSafe < 500)
	{
		if (azimuth >= 10)
		{
			output = 0.8;
		}
		else if (azimuth < -10)
		{
			output = -0.8;
		}
		else if (azimuth > 0 && azimuth < 10)
		{
			output = azimuth*0.06+0.2;
		}
		else if (azimuth < 0 && azimuth > -10)
		{
			output = -(azimuth*0.06+0.2);
		}
		logfile << "Calculated Output 3:" << output << std::endl;
		logfile << "Azimuth 3:" << azimuth << std::endl;
		driveTrain.TankDrive(output, -output);
		Wait(0.01);
		failSafe++;
		azimuth = aimer.GetAngleToTower();
		logfile << "loop 3:" << failSafe << std::endl;
		//  start the shooter motor and raise the shooter arm
		//  if azimuth < 0 and arm in position then fire shooter
		//  then turn 180 to point back to the home side
		//  console.println("pc > xbox");

		//  else sleep for .01 second sleep(1000) then increment failsafe
	}
	if (failSafe >=500)
	{
		driveTrain.Disable();
		shooter.Disable();

		logfile << "FAIL SAFE ACTIVATED!! WEEOO WEEOO!" << std::endl;
		return;
	}
	failSafe = 0;
	driveTrain.ArcadeDrive(0.0, 0.0, false);
	//aim at tower
	shooter.SetAngle(aimer.GetAngleToShoot());
	shooter.SetSpeed(1.0);
	shooter.Shoot();

	shooter.Disable();
	driveTrain.Disable();
	logfile << "Auto ended" << std::endl;
	logfile.close();

}
START_ROBOT_CLASS(Robot);
