#include "WPILib.h"
#include "Position.h"
#include <math.h>

#ifndef SRC_DRIVETRAIN_H
#define SRC_DRIVETRAIN_H
#define PI 3.14159265

class DriveTrain : public RobotDrive
{
	CANTalon leftMaster;
	CANTalon rightMaster;
	CANTalon leftSlave;
	CANTalon rightSlave;

	Position *position;
public:
	DriveTrain(uint32_t leftMasterDeviceID, uint32_t leftSlaveDeviceID, uint32_t rightMasterDeviceID, uint32_t rightSlaveDeviceID, Position *position_);
	void Enable();
	void Disable();
	void TankDrive(float leftSpeed, float rightSpeed);
	void TurnToAngle(float angle);
	void TurnToRelativeAngle(float angle);
	void MoveDistance(float distance, float speed);

};

#endif
