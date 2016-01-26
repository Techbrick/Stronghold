/*
 * Constants.cpp
 *
 *  Created on: Jan 25, 2016
 *      Author: Noah Zbozny
 */

#pragma once //necessary until headers are made
class Constants {
public:

	//Drive
	static constexpr int leftDriveChannel = 0;
	static constexpr int rightDriveChannel = 0;
	static constexpr int driveFrontRightPin = 0;
	static constexpr int driveFrontLeftPin = 0;
	static constexpr int driveRearRightPin = 0;
	static constexpr int driveRearLeftPin = 0;


	//Operator
	static constexpr int operatorStickChannel = 0;

	//Shooter
	static constexpr int shooterBottomPin = 0;
	static constexpr int shooterTopPin = 0;


	//Arm
	static constexpr int armTalonPin = 0;
	static constexpr int armBottomPin = 0;
	static constexpr int armTopPin = 0;


	//Position
	static constexpr float accelerometerSensitivity = .018;
	static constexpr float accelerometerZero = 2.5;
	static constexpr float xStartPos = 0;
	static constexpr float yStartPos = 0;
	static constexpr int gyroPin = 0;




	Constants() {}


};




