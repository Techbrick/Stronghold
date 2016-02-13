#include "Shooter.h"

Shooter::Shooter(uint32_t leftTalon, uint32_t rightTalon) :
	left(leftTalon),
	right(rightTalon)
{
	left.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	right.SetControlMode(CANTalon::ControlMode::kPercentVbus);
}

void Shooter::setSpeed(float leftSpeed, float rightSpeed)
{
	left.Set(leftSpeed);
	right.Set(rightSpeed);
}

void Shooter::Enable()
{
	left.Enable();
	right.Enable();
}

void Shooter::Disable()
{
	left.Disable();
	right.Disable();
}