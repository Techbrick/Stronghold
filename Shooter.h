#include "WPILib.h"

#ifndef SRC_SHOOTER_H
#define SRC_SHOOTER_H

class Shooter {
	CANTalon left;
	CANTalon right;

public:
	Shooter(uint32_t leftTalon, uint32_t rightTalon);
	
	void Enable();
	void Disable();

	void setSpeed(float leftSpeed, float rightSpeed);
};

#endif