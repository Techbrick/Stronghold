
/*
 * Position.h
 *
 *  Created on: Jan 26, 2016
 *      Author: Noah Zbozny
 */

#ifndef SRC_POSITION_H_
#define SRC_POSITION_H_

#include "WPILib.h"
#include "pthread.h"
#include "AHRS.h"

class Position {
	AHRS mxp; //navx mxp
	CANTalon &leftFrontTalon;
	CANTalon &leftRearTalon;
	CANTalon &rightFrontTalon;
	CANTalon &rightRearTalon;
	Timer xAccelTimer;
	Timer yAccelTimer;
	Timer xTalonTimer;
	Timer yTalonTimer;
	float xAcceleration;
	float yAcceleration;
	float xVelocity;
	float yVelocity;
	float xDistance;
	float yDistance;
	float xPosAccel;
	float yPosAccel;
	float xPosTalon;
	float yPosTalon;
	float xTime;
	float yTime;
	float xPos = Constants::xStartPos;
	float yPos = Constants::yStartPos;
	//static float xPos = Constants::xStartPos; //doesn't like static without constexpr
	//static float yPos = Constants::yStartPos;

public :
	Position(CANTalon &leftFrontTalon_, CANTalon &leftRearTalon_, CANTalon &rightFrontTalon_, CANTalon &rightRearTalon_);
	void Setup();
	void AccelerometerTrackX();
	void AccelerometerTrackY();
	void TalonTrackX();
	void TalonTrackY();
	void Update();
	float GetX();
	float GetY();
	float AngleToTower();
	float DistanceToTower();
};


#endif /* SRC_POSITION_H_ */
