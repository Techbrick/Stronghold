/*
 * Constants.cpp
 *
 *  Created on: Jan 25, 2016
 *      Author: Noah Zbozny
 */

#pragma once //makes sure the constants file is only included once during compile - error caused by including constants in a file, then including another file that also includes constants
class Constants {
public:

	//Drive
	static constexpr int leftDriveChannel = 0;
	static constexpr int rightDriveChannel = 0;
	static constexpr int driveRightPin = 0;
	static constexpr int driveLeftPin = 0;


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
	static constexpr float xStartPos = 0; //relative to origin
	static constexpr float yStartPos = 0; //relative to origin
	static constexpr float towerX = 0; //relative to origin
	static constexpr float towerY = 0; //relative to origin




	Constants() {}


};




