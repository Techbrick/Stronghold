#include "WPILib.h"
#include "Position.h"

#ifndef SRC_DRIVETRAIN_H
#define SRC_DRIVETRAIN_H

class DriveTrain : public RobotDrive
{
	CANTalon left;
	CANTalon right;
	Position position;
public:
	DriveTrain(uint32_t leftDeviceID, uint32_t rightDeviceID);
	void Enable();
	void Disable();
	void TankDrive(float leftSpeed, float rightSpeed);
	void TurnToAngle(float angle);
	void MoveDistance(float distance, float speed);

};

#endif
