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
	leftMaster.SetClosedLoopOutputDirection(true);
	leftSlave.SetModeSelect(CanTalonSRX::kMode_SlaveFollower);
	leftSlave.SetDemand(leftMasterDeviceID);
	leftSlave.SetRevMotDuringCloseLoopEn(1);

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


//TODO: Take a sensitivity
void DriveTrain::TurnToAngle(float angle) { //give angle in degrees
	float currentAngle = position->GetAngle();
	float k_p = 0.75;
	float k_i = 0.5;
	float p   = 0;
	float i   = 0;
	float delta_t = 0.01;
	
	float error = 0;
	float output = 0;

	unsigned int failsafe = 0;

	while (abs(currentAngle - angle) > 10.0 && failsafe < 500)
	{
		std::cout << "PI Loop: " << std::endl;
		std::cout << " P: " << p << std::endl;
		std::cout << " I: " << i << std::endl;
		std::cout << " Output: " << output << std::endl;

		i = (i + k_i * error) * delta_t;

		error = abs(angle - currentAngle);
		if(error > 180.0)
			error = error * -1;
		error /= 180.0;

		p = k_p * error;
		output = p + i;
		leftMaster.Set(output);   //TODO: Noah, try this, if it spins, then try replacing the 0.5 with 'output'
		rightMaster.Set(-output);
		currentAngle = position->GetAngleDegrees();
		failsafe++;
		Wait(delta_t);
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
