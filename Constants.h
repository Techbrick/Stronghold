#ifndef SRC_CONSTANTS_H
#define SRC_CONSTANTS_H

namespace Constants {
	//DriveTrain
	static constexpr int driveLeftTalonID  = 1;
	static constexpr int driveRightTalonID = 0;
	
	//Joystick axis and buttons
	static constexpr int driveJoystickChannel = 0;
	static constexpr int driveLeftStickX = 0;
	static constexpr int driveLeftStickY = 1;
	static constexpr int driveL2         = 3;
	static constexpr int driveRightStickY = 5;
	static constexpr int calibrateButton = 99;
	static constexpr int shootButton = 99;
	
	//Shooter
	static constexpr int shooterLeftTalonID = 7;
	static constexpr int shooterRightTalonID = 8;
	static constexpr int shooterAimTalonID = 99;
	static constexpr int shooterIRPin = 99;
	static constexpr int servoPin = 99;
	static constexpr int potPin = 99;
	static constexpr float minShootAngle = 0; //needs to be found
	static constexpr float shooter100Velocity = 12.33; //via physics major
	static constexpr float shooter75Velocity = 9.2475; //may need to be changed. Currently assuming percent voltage to velocity is linear
	static constexpr float shooter50Velocity = 6.165; //may need to be changed. Currently assuming percent voltage to velocity is linear



	//Arm
	static constexpr int armTalonPin  = 99;
	static constexpr int armBottomPin = 99;
	static constexpr int armTopPin    = 99;

	//Position
	static constexpr float towerX = 0;
	static constexpr float towerY = 0;
	static constexpr float xStartPos = 0;
	static constexpr float yStartPos = 0;
	static constexpr float wheelRadius = 3; //INCHES
	static constexpr float gyroOffset = 90;
	static constexpr int ticksPerRotation = 2048;
};

#endif
