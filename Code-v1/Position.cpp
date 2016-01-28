
/*
 * Position.h
 *
 *  Created on: Jan 26, 2016
 *      Author: Owner
 */

#ifndef SRC_POSITION_H_
#define SRC_POSITION_H_

#include "WPILib.h"
#include "pthread.h"
#include "AHRS.h"

class Position {
	AHRS gyro; //navx mxp
	BuiltInAccelerometer accel;
	Timer xTimer;
	Timer yTimer;
	float xAcceleration;
	float yAcceleration;
	float xDistance;
	float yDistance;
	//static float xPos = Constants::xStartPos; //doesn't like static without constexpr
	//static float yPos = Constants::yStartPos;
	float xPos = Constants::xStartPos;
	float yPos = Constants::yStartPos;

public :
	Position();
	void Setup();
	float trackX();
	float trackY();
	float angleToTower();
};


#endif /* SRC_POSITION_H_ */
