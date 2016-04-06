/*
 * Shooter.cpp
 *
 *  Created on: Feb 13, 2016
 *      Author: Noah
 */

#include "Shooter.h"

Shooter::Shooter(uint32_t leftTalon, uint32_t rightTalon, uint32_t angleTalon, uint32_t kickerTalon, Position *position_) :
	left(leftTalon),
	right(rightTalon),
	aim(angleTalon),
	ballSensor(Constants::shooterIRPin),
	servo(Constants::servoPin),
	kicker(kickerTalon),
	position(position_)
{
	left.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	right.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	aim.SetControlMode(CANTalon::ControlMode::kPosition);
	aim.SetFeedbackDevice(CANTalon::FeedbackDevice::AnalogPot);
	kicker.SetControlMode(CANTalon::ControlMode::kPercentVbus);
}

void Shooter::Enable()
{
	left.Enable();
	right.Enable();
	aim.Enable();
	kicker.Enable();
}

void Shooter::Disable()
{
	left.Disable();
	right.Disable();
	aim.Disable();
	kicker.Disable();
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
	if (angle < 32 || angle > 48) {
		SmartDashboard::PutString("Set Angle Failed", "Angle was out of bounds. Bounds are [0, 68.2]");
		return;
	}
	aim.SetControlMode(CANTalon::ControlMode::kPosition);
	int position = aim.GetAnalogInRaw();
	int failsafe = 0;
	int potValue = (int)(882 - (angle * Constants::aimDegreesToPotFactor));
	//if (potValue < 488) {
	//	SmartDashboard::PutString("Set Angle Failed for a different reason", "Angle was out of bounds. Will not clear the bar");
	//}
	if (aim.GetAnalogInRaw() < potValue) {
		while (aim.GetAnalogInRaw() < potValue && failsafe < 200) {
			aim.Set(position);
			position--; //may need to change increment value
			failsafe++;
			Wait(.01);
		}
	} else {
		while (aim.GetAnalogInRaw() > potValue && failsafe < 200) {
			aim.Set(position);
			failsafe++;
			position++; //depending on how the talon is wired this may need to be -= and the other one may need to be +=
			Wait(.01);
		}
	}
	//aim.SetAnalogPosition(potValue);
	aim.SetControlMode(CANTalon::ControlMode::kPercentVbus);
}

void Shooter::Move(float speed) {
	aim.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	//if (aim.GetAnalogInRaw() < 754 && speed > 0) {
	//	aim.Set(speed);
	//} else if (aim.GetAnalogInRaw() > 209 && speed < 0) {
		aim.Set(speed);
	//} else {
	//	aim.Set(0);
	//	return;
	//}
	SmartDashboard::PutBoolean("Shooter Top Limit Switch Hit", aim.GetReverseLimitOK());
	SmartDashboard::PutBoolean("Shooter Bottom Limit Switch Hit", aim.GetForwardLimitOK());
}

void Shooter::PrepareShooter(float angle, float speed) {
	SetAngle(angle);
	SetSpeed(speed);
}

void Shooter::LoadBall() {
	SetSpeed(-.6);
}

void Shooter::Shoot() {
	kicker.Set(-.5);
	Wait(0.3);
	kicker.Set(.5);
	Wait(.3);
	kicker.Set(0);
}

bool Shooter::HasBall() {
	return !ballSensor.Get();
}

float Shooter::WheelSpeed() {
	return left.Get();
}

float Shooter::Angle() {
	return (882 - aim.GetAnalogInRaw()) / Constants::aimDegreesToPotFactor - 6.0;
}

float Shooter::ReadPot() {
	return aim.GetAnalogInRaw();
	SmartDashboard::PutNumber("Pot reg", aim.GetAnalogIn());
}
