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
void DriveTrain::TurnToAngle(float angle) //give angle in degrees
{
	//angle *= -1;
	angle = fmod(angle, 360.0);
	angle = angle > 180 ? angle-360 : angle;

	float currentAngle = position->GetAngleDegrees();
	float k_p = SmartDashboard::GetNumber("PID_k_P", 0.75);
	float k_i = SmartDashboard::GetNumber("PID_k_I", 1.00);
	float p   = 0;
	float i   = 0;
	float delta_t = 0.1;
	
	float errorDegrees = 0;
	float errorScaled = 0;
	float output = 0;

	unsigned int failsafe = 0;

	std::cout << "Running Loop";
	while (abs(currentAngle - angle) > Constants::drivePIDepsilon && failsafe < 30)
	{
		std::cout << "PI Loop: " << std::endl;
		std::cout << " P: " << p << std::endl;
		std::cout << " I: " << i << std::endl;
		std::cout << " Output: " << output << std::endl;
		std::cout << " ErrorDegrees: " << errorDegrees << std::endl;
		std::cout << " ErrorScaled: " << errorScaled << std::endl;
		std::cout << " Target angle: " << angle << std::endl;
		std::cout << " Current angle: " << currentAngle << std::endl;
		std::cout << " Failsafe: " << failsafe << std::endl;

		i = (i + k_i * errorScaled) * delta_t;

		errorDegrees = currentAngle - angle;
		if (abs(errorDegrees) > 180.0)
		{
			errorDegrees = errorDegrees - copysign(360.0, errorDegrees);
		}
		errorScaled = errorDegrees/180.0;

		p = k_p * errorScaled;
		output = p + i;
		leftMaster.Set(output);
		rightMaster.Set(-output);
		currentAngle = position->GetAngleDegrees();
		failsafe++;
		Wait(delta_t);
	}

	leftMaster.Set(0);
	rightMaster.Set(0);
}

void DriveTrain::TurnToRelativeAngle(float angle) {
	TurnToAngle(angle + position->GetAngleDegrees());
}

void DriveTrain::DriveStraight(float speed, float fieldAngle, float timeInSeconds)
{
	float currentAngle = position->GetAngle();
	float k_p = Constants::driveK_P;
	float k_i = Constants::driveK_I;
	float p   = 0;
	float i   = 0;
	float delta_t = 0.01;
	
	float error = 0;
	float pidOutput = 0;

	unsigned int failsafe = 0;
	unsigned int failsafeMax = timeInSeconds / delta_t;
	while (abs(currentAngle-fieldAngle) > Constants::drivePIDepsilon && failsafe < failsafeMax)
	{
		i = (i + k_i * error) * delta_t;

		error = abs(fieldAngle - currentAngle);
		if(error > 180.0)
			error = error * -1;
		error /= 180.0;

		p = k_p * error;
		pidOutput = p + i;
		float combinedOutputLeft = tanh(speed + pidOutput);
		float combinedOutputRight = tanh(speed - pidOutput);
		combinedOutputLeft = combinedOutputLeft*0.9 + copysign(0.1, combinedOutputLeft);
		combinedOutputRight = combinedOutputRight*0.9 + copysign(0.1, combinedOutputRight);

		leftMaster.Set(combinedOutputLeft);
		rightMaster.Set(combinedOutputRight);
		currentAngle = position->GetAngleDegrees();
		failsafe++;
		Wait(delta_t);
	}

	leftMaster.Set(0);
	rightMaster.Set(0);
}

void DriveTrain::MoveDistance(float distance, float speed) {
	float xOffset = position->GetX();
	float yOffset = position->GetY();
	while (sqrt(pow(position->GetX() - xOffset, 2) + pow(position->GetY() - yOffset, 2)) < distance) {
		leftMaster.Set(speed);
		rightMaster.Set(speed);
	}
}
