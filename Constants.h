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
	
	//Shooter
	static constexpr int shooterLeftTalonID = 7;
	static constexpr int shooterRightTalonID = 8;

	//Arm
	static constexpr int armTalonPin  = 0;
	static constexpr int armBottomPin = 0;
	static constexpr int armTopPin    = 0;

	//Position
	static constexpr float accelerometerSensitivity = .018;
	static constexpr float accelerometerZero = 2.5;
	static constexpr float xStartPos = 0;
	static constexpr float yStartPos = 0;
	static constexpr int gyroPin = 0;

	//Field Coordinates
	static constexpr float towerX = 0;
	static constexpr float towerY = 0;

}

#endif
