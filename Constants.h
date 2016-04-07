#ifndef SRC_CONSTANTS_H
#define SRC_CONSTANTS_H

namespace Constants {
	//DriveTrain
	static constexpr int driveLeftMasterID  = 12;
	static constexpr int driveRightMasterID = 14;
	static constexpr int driveLeftSlaveID = 13;
	static constexpr int driveRightSlaveID = 15;
	static constexpr float driveK_P = 0.008;
	static constexpr float drivePIDepsilon = 4.0;
	static constexpr float drivePIDMaxSpeed = 0.9;
	static constexpr float drivePIDMinSpeed = 0.03;
	static constexpr float drivePIDFinishTurn = 0.1;
	static constexpr int driveMaxRPM = 590;

	//Joystick axis and buttons
	static constexpr int driveJoystickChannel = 0;
	static constexpr int driveLeftStickX = 0;
	static constexpr int driveLeftStickY = 1;
	static constexpr int driveL2 = 3;
	static constexpr int driveR2 = 4;
	static constexpr int driveRightStickY = 5;
	static constexpr int calibrateButton = 12;
	static constexpr int shootButton = 1;
	static constexpr int prepareToShootButton = 3;
	static constexpr int ejectButton = 14;
	static constexpr int stopShooterWheels = 13;
	static constexpr int xButton = 2;

	//Shooter
	static constexpr int shooterLeftTalonID = 8;
	static constexpr int shooterRightTalonID = 9;
	static constexpr int shooterAimTalonID = 10;
	static constexpr int shooterKickerTalonID = 11;
	static constexpr int servoPin = 0;
	static constexpr int shooterIRPin = 6;
	static constexpr float shooter100Velocity = 12.33; //via physics major
	static constexpr float shooter75Velocity = 9.2475; //may need to be changed. Currently assuming percent voltage to velocity is linear
	static constexpr float shooter50Velocity = 6.165; //may need to be changed. Currently assuming percent voltage to velocity is linear
	static constexpr float shooterMinAngle = 32;	//Actual value: needs more accuracy
	static constexpr float shooterMaxAngle = 46; //TODO: temporary value
	static constexpr float servoMaxPosition = 0.9;
	static constexpr float servoMinPosition = 0.3;
	static constexpr int potMinValue = 469;
	static constexpr int potMaxValue = 45;
	static constexpr float aimDegreesToPotFactor = ((potMinValue - potMaxValue) / (double)shooterMaxAngle);//(potMaxValue / 48.0); / (103.289 / 44.0);

	//Arm
	static constexpr int armTalonPin  = 11;

	//Position
	static constexpr float towerX = 0;
	static constexpr float towerY = 0;
	static constexpr float towerHeight = 0;
	static constexpr float xStartPos = 0;
	static constexpr float yStartPos = 0;
	static constexpr float gyroOffset = 90;
	static constexpr int ticksPerRotation = 2048;
	static constexpr float quadratureEncoderFactor = .25;
	static constexpr float wheelRadius = 3 * .0254; //meters

};

#endif

