/*
 * Position.cpp
 *
 *  Created on: Jan 25, 2016
 *      Author: Noah Zbozny
 */
//origin is nearest left corner
#include "WPILib.h"
#include "Constants.cpp"
#include "pthread.h"
#include "math.h"
#include "Position.h"

#define PI 3.14159265

	//TODO: Set to update every ms - Kyle

	Position::Position():
	gyro(I2C::Port::kMXP), //assuming we're on this port
	accel()
	{
		xAcceleration = 0;
		yAcceleration = 0;
		xDistance = 0;
		yDistance = 0;
	}

	void Position::Setup() {
		xTimer.Start();
		xTimer.Reset();
		yTimer.Start();
		yTimer.Reset();
		gyro.Reset();
	}
	
	float Position::trackX() {
		xAcceleration = accel.GetX() * cos((gyro.GetAngle() - 90) * PI / 180); //angle + 90? Will have to test
		xDistance = .5 * xAcceleration * xTimer.Get() * xTimer.Get();
		xPos = xPos + xDistance;
		xTimer.Reset();
		return xPos;
	}

	float Position::trackY() {
		yAcceleration = accel.GetY() * sin((gyro.GetAngle() - 90) * PI / 180); //again, we'll have to play with it
		yDistance = .5 * yAcceleration * yTimer.Get() * yTimer.Get();
		yPos = yPos + yDistance;
		yTimer.Reset();
		return yPos;
	}

	float Position::angleToTower() {
		float theta = gyro.GetAngle();
		float xToTower = Constants::towerX - xPos;
		float yToTower = Constants::towerY - yPos;
		float dotProduct;
		float uLength;
		float vLength;
		float angleToTower;

		dotProduct = (-1 * xPos) * (xToTower) + (-1 * xPos * tan(90 - theta)) * (yToTower);
		uLength = sqrt(pow(-1 * xPos, 2) + pow(-1 * xPos * tan(90 - theta), 2));
		vLength = sqrt(pow(xToTower, 2) + pow(yToTower, 2));
		angleToTower = acos(dotProduct/(uLength * vLength)); //linear algebra
		return angleToTower;
	}



