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
	float currentAngle = position->GetAngle();
	float k_p = 0.75;
	float k_i = 0.5;
	float p   = 0;
	float i   = 0;

	float output = 0;
	while (abs(currentAngle - angle) > 1.0)
	{
		std::cout << "PI Loop: " << std::endl;
		std::cout << " P: " << p << std::endl;
		std::cout << " I: " << i << std::endl;
		std::cout << " Output: " << output << std::endl;
		p = k_p * (angle - currentAngle);
		i = i + k_i * output;
		output = i + p;
		leftMaster.Set(0.5);
		rightMaster.Set(-0.5);
		currentAngle = position->GetAngle();
	}

	leftMaster.Set(0);
	rightMaster.Set(0);
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
