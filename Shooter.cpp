/*
 * Shooter.cpp
 *
 *  Created on: Feb 13, 2016
 *      Author: Noah
 */

#include "Shooter.h"

Shooter::Shooter(uint32_t leftTalon, uint32_t rightTalon, uint32_t angleTalon, Position *position_) :
	left(leftTalon),
	right(rightTalon),
	aim(angleTalon),
	ballSensor(Constants::shooterIRPin),
	servo(Constants::servoPin),
	position(position_)
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

void Shooter::SetSpeed(float speed) {
	left.Set(-speed);
	right.Set(speed);
}

void Shooter::SetAngle(float angle) { //degrees
	//TODO: Don't set angle more than Constants::maximumAngle or less than Constants::minimumAngle
	if (angle < 0 || angle > 68.2) //TODO: Put these in Constants
	{
		SmartDashboard::PutString("Set Angle Failed", "Angle was out of bounds. Bounds are [0, 68.2]");
		return;
	}
	aim.SetControlMode(CANTalon::ControlMode::kPosition);
	int position = aim.Get();
	int potValue = (int)(754 - (angle * Constants::aimDegreesToPotFactor));
	if (potValue < 488) {
		SmartDashboard::PutString("Set Angle Failed for a different reason", "Angle was out of bounds. Will not clear the bar");
	}
	if (aim.GetAnalogInRaw() < potValue) {
		while (aim.GetAnalogInRaw() < potValue) {
			aim.Set(position);
			position--; //may need to change increment value
		}
	} else {
		while (aim.GetAnalogInRaw() > potValue) {
			aim.Set(position);
			position++; //depending on how the talon is wired this may need to be -= and the other one may need to be +=
		}
	}
	aim.SetControlMode(CANTalon::ControlMode::kPercentVbus);
}

void Shooter::Move(float speed) {
	aim.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	aim.Set(speed);
}

void Shooter::PrepareShooter(float angle, float speed) {
	SetAngle(angle);
	SetSpeed(speed);
}

void Shooter::LoadBall() {
	//TODO: should this set an angle too?
	SetSpeed(-.35);
}

void Shooter::Shoot() {
	servo.Set(Constants::servoMaxPosition); //angle will need to be adjusted with testing
	Wait(.5);
	servo.Set(Constants::servoMinPosition); //angle will need to be adjusted with testing
}

bool Shooter::HasBall() {
	return ballSensor.Get();
}

float Shooter::WheelSpeed() {
	return left.Get();
}

float Shooter::Angle() {
	return aim.Get();
}

void Shooter::ReadPot() {
	SmartDashboard::PutNumber("Pot Raw", aim.GetAnalogInRaw());
	SmartDashboard::PutNumber("Pot reg", aim.GetAnalogIn());
}
