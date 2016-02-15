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

float Shooter::AngleToShoot() {
	float theta = Constants::minShootAngle; //degrees
	float velocity = Constants::shooter100Velocity; //meters/second
	float length = position.DistanceToTower();
	float equation1 = pow(velocity, 2) * length * sin(2 * theta);
	float equation2 = 2 + 2 * cos(2 * theta) + 9.8 * pow(length, 2);
	while (abs(equation1 - equation2) < .1) { //.1 is arbitrary. We need to determine what is a good margin of error
		theta = theta + .1;
		equation1 = pow(velocity, 2) * length * sin(2 * theta);
		equation2 = 2 + 2 * cos(2 * theta) + 9.8 * pow(length, 2);
		if (theta > 90 && velocity == Constants::shooter100Velocity) {
			velocity = Constants::shooter75Velocity;
			theta = Constants::minShootAngle;
		} else if (theta > 90 && velocity  == Constants::shooter75Velocity) {
			velocity = Constants::shooter50Velocity;
			theta = Constants::minShootAngle;
		}
	} //may need to find a more efficient algorithm for this loop. Depends on how long it takes to loop
	if (velocity == Constants::shooter75Velocity) {
		SetSpeed(.75);
	} else if (velocity == Constants::shooter50Velocity) {
		SetSpeed(.5);
	}
	return theta;
}
