/*
 * Position.cpp
 *
 *  Created on: Jan 25, 2016
 *      Author: Noah Zbozny
 */
//origin is nearest left corner
#include "WPILib.h"
#include "Constants.h"
#include <pthread.h>
#include <math.h>
#include "Position.h"

#define PI 3.14159265

	Position::Position(CANTalon &leftFrontTalon_, CANTalon &leftRearTalon_, CANTalon &rightFrontTalon_, CANTalon &rightRearTalon_):
	mxp(I2C::Port::kMXP), //assuming we're on this port
	leftFrontTalon(leftFrontTalon_),
	leftRearTalon(leftRearTalon_),
	rightFrontTalon(rightFrontTalon_),
	rightRearTalon(rightRearTalon_)
	{
		xAcceleration = 0;
		yAcceleration = 0;
		xVelocity = 0;
		yVelocity = 0;
		xDistance = 0;
		yDistance = 0;
		xTime = 0;
		yTime = 0;
		xPosAccel = Constants::xStartPos;
		yPosAccel = Constants::yStartPos;
		xPosTalon = Constants::xStartPos;
		yPosTalon = Constants::yStartPos;
		leftFrontTalon.SetPID(Constants::CANTalonP, Constants::CANTalonI, Constants::CANTalonD, Constants::CANTalonF);
		leftFrontTalon.EnableControl();
		leftFrontTalon.SetControlMode(CANSpeedController::kSpeed);
		leftRearTalon.SetPID(Constants::CANTalonP, Constants::CANTalonI, Constants::CANTalonD, Constants::CANTalonF);
		leftRearTalon.EnableControl();
		leftRearTalon.SetControlMode(CANSpeedController::kSpeed);
		rightFrontTalon.SetPID(Constants::CANTalonP, Constants::CANTalonI, Constants::CANTalonD, Constants::CANTalonF);
		rightFrontTalon.EnableControl();
		rightFrontTalon.SetControlMode(CANSpeedController::kSpeed);
		rightRearTalon.SetPID(Constants::CANTalonP, Constants::CANTalonI, Constants::CANTalonD, Constants::CANTalonF);
		rightRearTalon.EnableControl();
		rightRearTalon.SetControlMode(CANSpeedController::kSpeed);
		xAccelTimer.Start();
		xAccelTimer.Reset();
		yAccelTimer.Start();
		yAccelTimer.Reset();
		xTalonTimer.Start();
		xTalonTimer.Reset();
		yTalonTimer.Start();
		yTalonTimer.Reset();
		mxp.Reset();
	}
	
	void Position::AccelerometerTrackX() {
		xTime = xAccelTimer.Get();
		xAcceleration = mxp.GetRawAccelX();
		xVelocity = xVelocity + xAcceleration * xTime;
		xDistance = xDistance + xVelocity * xTime;
		xPosAccel = xPosAccel + xDistance;
		xVelocity = 0;
		xDistance = 0;
		xAccelTimer.Reset();
	}

	void Position::AccelerometerTrackY() {
		yAcceleration = mxp.GetRawAccelY();
		yTime = yAccelTimer.Get();
		yVelocity = yVelocity + yAcceleration * yTime;
		yDistance = yDistance + yVelocity * yTime;
		yPosAccel = yPosAccel +yDistance;
		yVelocity = 0;
		yDistance = 0;
		yAccelTimer.Reset();
	}

/*	void Position::TalonTrackX() {
		xVelocity = ((leftFrontTalon.GetSpeed() + leftRearTalon.GetSpeed() + rightFrontTalon.GetSpeed() + rightRearTalon.GetSpeed()) / 4.0) * cos(mxp.GetAngle());
		xPosTalon = xVelocity * xTalonTimer.Get();
		xTalonTimer.Reset();
	}

	void Position::TalonTrackY() {
		yVelocity = ((leftFrontTalon.GetSpeed() + leftRearTalon.GetSpeed() + rightFrontTalon.GetSpeed() + rightRearTalon.GetSpeed()) / 4.0) * sin(mxp.GetAngle());
		yPosTalon = yVelocity * yTalonTimer.Get();
		yTalonTimer.Reset();
	}
*/
	void Position::Update() {
		AccelerometerTrackX();
		AccelerometerTrackY();
		xPos = xPosAccel;
		yPos = yPosAccel;
		SmartDashboard::PutNumber("xPos", xPos);
		SmartDashboard::PutNumber("yPos", yPos);
		SmartDashboard::PutNumber("xVel", xVelocity);
		SmartDashboard::PutNumber("yVel", yVelocity);
		SmartDashboard::PutNumber("xAccel", xAcceleration);
		SmartDashboard::PutNumber("yAccel", yAcceleration);
	/*	TalonTrackX();
		TalonTrackY();
		xPos = (xPosAccel + xPosTalon) / 2;
		yPos = (yPosAccel + yPosTalon) / 2;*/
	}

	float Position::GetX() {
		return xPos;
	}

	float Position::GetY() {
		return yPos;
	}

	float Position::AngleToTower() {
		float theta = mxp.GetAngle();
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

	float Position::DistanceToTower() {
		float xPart;
		float yPart;
		float distance;

		xPart = Constants::towerX - xPos;
		yPart = Constants::towerY - yPos;
		distance = sqrt(pow(xPart, 2) + pow(yPart, 2));
		return distance;
	}
