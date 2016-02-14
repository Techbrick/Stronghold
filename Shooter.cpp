/*
 * Shooter.cpp
 *
 *  Created on: Feb 13, 2016
 *      Author: Noah
 */

#include "Shooter.h"

Shooter::Shooter(uint32_t leftTalon, uint32_t rightTalon, uint32_t angleTalon) :
	left(leftTalon),
	right(rightTalon),
	aim(angleTalon),
	ballSensor(Constants::shooterIRPin),
	servo(Constants::servoPin),
	pot(Constants::potPin, 90, 0)
{
	left.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	right.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	aim.SetControlMode(CANTalon::ControlMode::kPosition);
}

void Shooter::Enable()
{
	left.Enable();
	right.Enable();
	aim.Enable();
}

void Shooter::Disable()
{
	left.Disable();
	right.Disable();
	aim.Disable();
}

void Shooter::SetSpeed(float leftSpeed, float rightSpeed) //speeds are a percentage of the maximum possible speed
{
	left.Set(leftSpeed);
	right.Set(rightSpeed);
}

void Shooter::SetAngle(float angle) { //degrees
	int value;
	while (pot.Get() < angle) {
		aim.Set(value);
		value++;
	}
}

void Shooter::PrepareShooter() {
	SetSpeed(.75, -.75);
	SetAngle(45);
}

void Shooter::PrepareShooter(float angle, float speed) {
	SetAngle(angle);
	SetSpeed(speed, -speed);
}

void Shooter::LoadBall() {
	while (HasBall() == false) {
		SetSpeed(-.35, .35);
	}
	SetSpeed(0, 0);
}

void Shooter::Shoot() {
	servo.SetAngle(servo.GetMaxAngle()); //angle will need to be adjusted with testing
	Wait(.5);
	servo.SetAngle(servo.GetMinAngle()); //angle will need to be adjusted with testing
}

bool Shooter::HasBall() {
	if (ballSensor.Get() == 0) { //may be 1. The website says 0 is found barrier but then the output of the example code according to their instructions says otherwise
		return true;
	} else {
		return false;
	}
}

float Shooter::WheelSpeed() {
	if (left.Get() != right.Get()) {
		return -1;
	} else {
		return left.Get();
	}
}

float Shooter::Angle() {
	return pot.Get();
}
