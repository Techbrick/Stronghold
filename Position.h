
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
	Timer xAccelTimer;
	Timer yAccelTimer;
	Timer runOnceTimer;
	float xAcceleration;
	float yAcceleration;
	float xVelocity;
	float yVelocity;
	float xDistance;
	float yDistance;
	float xPosAccel;
	float yPosAccel;
	float xTime;
	float yTime;
	float xPos = Constants::xStartPos;
	float yPos = Constants::yStartPos;
	int counter = 0;
	//static float xPos = Constants::xStartPos; //doesn't like static without constexpr
	//static float yPos = Constants::yStartPos;

public :
	Position();
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
