#include "DriveTrain.h"

DriveTrain::DriveTrain(uint32_t leftDeviceID, uint32_t rightDeviceID):
	left(leftDeviceID),
	right(rightDeviceID),
	RobotDrive(left, right)
{
	left.SetControlMode(CANTalon::ControlMode::kPercentVbus);
	right.SetControlMode(CANTalon::ControlMode::kPercentVbus);
}

void DriveTrain::Enable()
{
	left.Enable();
	right.Enable();
}

void DriveTrain::Disable()
{
	left.Disable();
	right.Disable();
}

void DriveTrain::TankDrive(float leftSpeed, float rightSpeed) {
	left.Set(leftSpeed);
	right.Set(rightSpeed);
}

void DriveTrain::TurnToAngle(float angle) {
	angle = angle * PI / 180;
	if (position.GetAngle() < angle) {
		while (position.GetAngle() < angle) {
			left.Set(-.5);
			right.Set(.5);
		}
	} else if (position.GetAngle() > angle) {
		while (position.GetAngle() > angle) {
			left.Set(.5);
			right.Set(-.5);
		}
	}
}

void DriveTrain::MoveDistance(float distance, float speed) {
	float xOffset = position.GetX();
	float yOffset = position.GetY();
	while (sqrt(pow(position.GetX() - xOffset, 2) + pow(position.GetY() - yOffset, 2)) < distance) {
		left.Set(speed);
		right.Set(speed);
	}
}
