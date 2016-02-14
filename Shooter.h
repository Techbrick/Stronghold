#include "WPILib.h"
#include "Constants.h"

#ifndef SRC_SHOOTER_H
#define SRC_SHOOTER_H

class Shooter {
	CANTalon left;
	CANTalon right;
	CANTalon aim;
	DigitalInput ballSensor;
	Servo servo;

public:
	Shooter(uint32_t leftTalon, uint32_t rightTalon, uint32_t angleTalon);

	void SetSpeed(float leftSpeed, float rightSpeed);
	void SetAngle(float angle);
	void Enable();
	void Disable();
	void PrepareShooter();
	void PrepareShooter(float angle, float speed);
	void LoadBall();
	void Shoot();
	bool HasBall();
	float WheelSpeed();
	float Angle();
};

#endif
