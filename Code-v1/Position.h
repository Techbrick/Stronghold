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

class Position {
	AnalogGyro &gyro;
	BuiltInAccelerometer &accel;
public :
	Position(AnalogGyro &gyroPtr, BuiltInAccelerometer &accelPtr);
	void Setup();
	float trackX();
	float trackY();
};


#endif /* SRC_POSITION_H_ */
