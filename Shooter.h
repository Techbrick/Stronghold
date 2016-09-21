/*
 * Shooter.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Noah
 */

#include "WPILib.h"
#include "Constants.h"
#include "Position.h"
#include "ADXL345_I2C.h"

#ifndef SRC_SHOOTER_H
#define SRC_SHOOTER_H

class Shooter {
	CANTalon left;
	CANTalon right;
	CANTalon aim;
	DigitalInput ballSensor;
	Servo servo;
	CANTalon kicker;
	Position *position;
	ADXL345_I2C accel;

public:
	Shooter(uint32_t leftTalon, uint32_t rightTalon, uint32_t angleTalon, uint32_t kickerTalon, Position *position_);
	void Enable();
	void Disable();
	void SetSpeed(float leftSpeed, float rightSpeed);
	void SetSpeed(float speed);
	void SetAngle(float angle);
	void Move(float speed);
	void PrepareShooter(float angle = 45, float speed = 1.0);
	void LoadBall();
	void Shoot();
	void SetPotValue(int potValue);
	float ReadPot();
	bool HasBall();
	float WheelSpeed();
	float Angle();
	float Roll();
	float Pitch();
};

#endif
