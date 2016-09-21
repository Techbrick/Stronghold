/*
 * Shooter.cpp
 *
 *  Created on: Feb 13, 2016
 *      Author: Noah
 */

#include "Shooter.h"
#include "math.h"
#define PI 3.14159265

Shooter::Shooter(uint32_t leftTalon, uint32_t rightTalon, uint32_t angleTalon, uint32_t kickerTalon, Position *position_) :
	left(leftTalon),
	right(rightTalon),
	aim(angleTalon),
	ballSensor(Constants::shooterIRPin),
	servo(Constants::servoPin),
	kicker(kickerTalon),
	position(position_),
	accel(I2C::Port::kOnboard)
{
	left.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	right.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	aim.SetControlMode(CANTalon::ControlMode::kPosition);
	aim.SetFeedbackDevice(CANTalon::FeedbackDevice::AnalogPot);
	aim.SetClosedLoopOutputDirection(true);
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

/*void Shooter::SetAngle(float angle) { //degrees
	if (angle < Constants::shooterMinAngle || angle > Constants::shooterMaxAngle) {
		return;
	}
	aim.SetControlMode(CANTalon::ControlMode::kPosition);
	int potValue = (int)(Constants::potMinValue - (angle * Constants::aimDegreesToPotFactor));
	aim.Set(potValue);
	/*if (angle < Constants::shooterMinAngle || angle > Constants::shooterMaxAngle) {
		return;
	}
	int potValue = (int)(Constants::potMinValue - (angle * Constants::aimDegreesToPotFactor));
	aim.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	float failsafe = 0.0;
	float delta_t = 0.05;
	while (abs(aim.GetAnalogInRaw() - potValue) > 2.0 && failsafe < 2.0/delta_t)
	{
		aim.Set()
	}
}*/

void Shooter::SetPotValue(int potValue) {
	aim.SetControlMode(CANTalon::ControlMode::kPosition);
	aim.Set(potValue);
}
void Shooter::SetAngle(float angle) { //degrees
	if (angle < 32 || angle > 44) {
		return;
	}
	aim.SetControlMode(CANTalon::ControlMode::kPosition);
	int position = aim.GetAnalogInRaw();
   int failsafe = 0;
	int potValue = (int)(Constants::potMinValue - (angle * Constants::aimDegreesToPotFactor));
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
	aim.Set(speed);
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
	Wait(0.45);
	kicker.Set(.5);
	Wait(0.45);
	kicker.Set(0);
}

bool Shooter::HasBall() {
	return !ballSensor.Get();
}

float Shooter::WheelSpeed() {
	return left.Get();
}

float Shooter::Angle() {
	//return (Constants::potMinValue - aim.GetAnalogInRaw()) / Constants::aimDegreesToPotFactor; //wired backwards
	//return (Constants::potMaxValue - aim.GetAnalogInRaw()) / Constants::aimDegreesToPotFactor; //wired forwards

	return Roll();

}

float Shooter::Roll() {
	return -(atan2(accel.GetX(),sqrt(accel.GetY()*accel.GetY()+accel.GetZ()*accel.GetZ()))  * 180.0) / PI;
}

float Shooter::Pitch(){
	return (atan2(accel.GetY(),sqrt(accel.GetX()*accel.GetX()+accel.GetZ()*accel.GetZ()))  * 180.0) / PI;
}

float Shooter::ReadPot() {
	return aim.GetAnalogInRaw();
}
