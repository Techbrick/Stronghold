/*
 * Position.cpp
 *
 *  Created on: Jan 25, 2016
 *      Author: Noah Zbozny
 */

#include "WPILib.h"
#include "Constants.cpp"
#include "pthread.h"
#include "math.h"

#define PI 3.14159265

class Position {
	Timer xTimer;
	Timer yTimer;
	float xAcceleration;
	float yAcceleration;
	float xDistance;
	float yDistance;
	//static float xPos = Constants::xStartPos;
	//static float yPos = Constants::yStartPos;
	float xPos = Constants::xStartPos;
	float yPos = Constants::yStartPos;


public:

	//TODO: Add in absolute locations of obstacles so that the robot can move to them && add in vision capabilities
	//TODO: Set in Robot.cpp to update every ms - Kyle

	void Setup(AnalogGyro gyro, BuiltInAccelerometer accel) {
		xTimer.Start();
		xTimer.Reset();
		yTimer.Start();
		yTimer.Reset();
		gyro.Reset();
	}
	float trackX(AnalogGyro gyro, BuiltInAccelerometer accel) {
		xAcceleration = accel.GetX() * cos((gyro.GetAngle() - 90) * PI / 180); //angle + 90? Will have to test
		xDistance = .5 * xAcceleration * xTimer.Get() * xTimer.Get();
		xPos = xPos + xDistance;
		return xPos;
		xTimer.Reset();
	}

	float trackY(AnalogGyro gyro, BuiltInAccelerometer accel) {
		yAcceleration = accel.GetY() * sin((gyro.GetAngle() - 90) * PI / 180); //again, we'll have to play with it
		yDistance = .5 * yAcceleration * yTimer.Get() * yTimer.Get();
		yPos = yPos + yDistance;
		return yPos;
		yTimer.Reset();
	}

	Position() {}


};
