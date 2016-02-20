/*
 * Shooter.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Noah
 */

#include "WPILib.h"
#include "Constants.h"
#include "Position.h"

#ifndef SRC_SHOOTER_H
#define SRC_SHOOTER_H

class Shooter {
	CANTalon left;
	CANTalon right;
	CANTalon aim;
	DigitalInput ballSensor;
	Servo servo;
	AnalogPotentiometer pot;
	Position *position;

public:
	Shooter(uint32_t leftTalon, uint32_t rightTalon, uint32_t angleTalon, Position *position_);
	void Enable();
	void Disable();
	void SetSpeed(float leftSpeed, float rightSpeed);
	void SetSpeed(float speed);
	void SetAngle(float angle);
	void PrepareShooter(float angle = 45, float speed = 0.75);
	void LoadBall();
	void Shoot();
	bool HasBall();
	float WheelSpeed();
	float Angle();
	float AngleToShoot();
};

#endif
