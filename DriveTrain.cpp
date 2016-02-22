#include "DriveTrain.h"
#define PI 3.14159265

DriveTrain::DriveTrain(uint32_t leftDeviceID, uint32_t rightDeviceID, Position *position_):
	left(leftDeviceID),
	right(rightDeviceID),
	position(position_),
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

void DriveTrain::TurnToAngle(float angle) { //give angle in degrees
	float currentAngle = 0;
	float p = .75 / 180;
	float offset;
	while (abs(currentAngle - angle) > 1.0)
	{
		offset = angle - currentAngle;
		left.Set(offset * p);
		right.Set(-offset * p);
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
		left.Set(speed);
		right.Set(speed);
	}
}
