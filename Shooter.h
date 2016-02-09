#include "WPILib.h"

#ifndef SRC_SHOOTER_H
#define SRC_SHOOTER_H

class Shooter {
	TalonSRX left;
	TalonSRX right;

public:
	Shooter(uint32_t leftTalon, uint32_t rightTalon);

	void setSpeed(float speed);
};

#endif