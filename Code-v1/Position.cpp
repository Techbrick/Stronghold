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
	ADXL345_I2C accel;
	Gyro gyro;
	Timer xTimer;
	Timer yTimer;
	Timer zTimer;
	float xAcceleration;
	float yAcceleration;
	float xDistance;
	float yDistance;
	static float xPos = Constants::xStartPos;
	static float yPos = Constants::yStartPos;

public:

	//TODO: Add in absolute locations of obstacles so that the robot can move to them && add in vision capabilities

	void Setup() {
		xTimer.Start();
		yTimer.Start();
		zTimer.Start();
		gyro(Constants::gyroPin);
		accel(Constants::accelerometerPin);
		gyro.Reset();
	}
	float trackX() {
		xTimer.Reset();
		xAcceleration = accel.GetX() * cos((gyro.GetAngle() - 90) * PI / 180); //angle + 90? Will have to test
		xDistance = .5 * xAcceleration * xTimer.Get() * xTimer.Get();
		xPos = xPos + xDistance;
		return xPos;
	}

	float trackY() {
		yTimer.Reset();
		yAcceleration = accel.GetY() * sin((gyro.GetAngle() - 90) * PI / 180); //again, we'll have to play with it
		yDistance = .5 * yAcceleration * yTimer.Get() * yTimer.Get();
		yPos = yPos + yDistance;
		return yPos;
	}

};
