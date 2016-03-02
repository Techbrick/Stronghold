#include "DriveTrain.h"
#define PI 3.14159265

DriveTrain::DriveTrain(uint32_t leftMasterDeviceID, uint32_t leftSlaveDeviceID, uint32_t rightMasterDeviceID, uint32_t rightSlaveDeviceID, Position *position_):
	RobotDrive(leftMaster, rightMaster),
	leftMaster(leftMasterDeviceID),
	leftSlave(leftSlaveDeviceID),
	rightMaster(rightMasterDeviceID),
	rightSlave(rightSlaveDeviceID),
	position(position_)
{
	leftMaster.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	leftSlave.SetControlMode(CANTalon::ControlMode::kFollower);
	leftSlave.Set(Constants::driveLeftMasterID);

	rightMaster.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	rightSlave.SetControlMode(CANTalon::ControlMode::kFollower);
	rightSlave.Set(Constants::driveRightMasterID);
}

void DriveTrain::Enable()
{
	leftMaster.Enable();
	rightMaster.Enable();
}

void DriveTrain::Disable()
{
	leftMaster.Disable();
	rightMaster.Disable();
}

void DriveTrain::TankDrive(float leftSpeed, float rightSpeed) {
	leftMaster.Set(leftSpeed);
	rightMaster.Set(rightSpeed);
}

//TODO: Take a sensitivity
void DriveTrain::TurnToAngle(float angle) { //give angle in degrees
	float currentAngle = 0;
	float p = .75 / 180;
	float offset;
	while (abs(currentAngle - angle) > 1.0)
	{
		offset = angle - currentAngle;
		leftMaster.Set(offset * p);
		rightMaster.Set(-offset * p);
		currentAngle = position->GetAngle();
	}
}

void DriveTrain::TurnToRelativeAngle(float angle) {
	TurnToAngle(angle + position->GetAngle());
}

void DriveTrain::MoveDistance(float distance, float speed) {
	float xOffset = position->GetX();
	float yOffset = position->GetY();
	while (sqrt(pow(position->GetX() - xOffset, 2) + pow(position->GetY() - yOffset, 2)) < distance) {
		leftMaster.Set(speed);
		rightMaster.Set(speed);
	}
}
