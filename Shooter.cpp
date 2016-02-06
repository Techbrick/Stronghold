#include "Shooter.h"

Shooter::Shooter(uint32_t leftTalon, uint32_t rightTalon) :
		left(leftTalon),
		right(rightTalon) {}

void Shooter::setSpeed(float speed)
{
	left.Set(speed);
	right.Set(speed);
}