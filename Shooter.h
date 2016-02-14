/*
 * Shooter.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Noah
 */

#include "WPILib.h"
#include "Constants.h"

#ifndef SRC_SHOOTER_H
#define SRC_SHOOTER_H

class Shooter {
	CANTalon left;
	CANTalon right;
	CANTalon aim;
	DigitalInput ballSensor;
	Servo servo;
	AnalogPotentiometer pot;

public:
	Shooter(uint32_t leftTalon, uint32_t rightTalon, uint32_t angleTalon);
	void Enable();
	void Disable();
	void SetSpeed(float leftSpeed, float rightSpeed);
	void SetAngle(float angle);
	void PrepareShooter();
	void PrepareShooter(float angle, float speed);
	void LoadBall();
	void Shoot();
	bool HasBall();
	float WheelSpeed();
	float Angle();
};

#endif
