#include "DriveTrain.h"

DriveTrain::DriveTrain(uint32_t leftMasterDeviceID, uint32_t leftSlaveDeviceID, uint32_t rightMasterDeviceID, uint32_t rightSlaveDeviceID, Position *position_):
	RobotDrive(leftMaster, rightMaster),
	leftMaster(leftMasterDeviceID),
	leftSlave(leftSlaveDeviceID),
	rightMaster(rightMasterDeviceID),
	rightSlave(rightSlaveDeviceID),
	position(position_)
{
	leftMaster.SetClosedLoopOutputDirection(true);
	leftMaster.SetControlMode(CANTalon::ControlMode::kSpeed);
	leftMaster.SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	leftMaster.ConfigEncoderCodesPerRev(2048);
	leftMaster.SetSensorDirection(true);
	leftMaster.SelectProfileSlot(0);
	leftMaster.SetF(0.124);
	leftMaster.SetP(0.12);
	leftMaster.SetI(0.00000);
	leftMaster.SetD(0.0);
	leftMaster.SetAllowableClosedLoopErr(10);
	leftMaster.SetInverted(false);

	leftSlave.SetModeSelect(CanTalonSRX::kMode_SlaveFollower);
	leftSlave.SetDemand(leftMasterDeviceID);
	leftSlave.SetRevMotDuringCloseLoopEn(1);

	rightMaster.SetControlMode(CANTalon::ControlMode::kSpeed);
	rightMaster.SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	rightMaster.ConfigEncoderCodesPerRev(2048);
	rightMaster.SetSensorDirection(true);
	rightMaster.SelectProfileSlot(0);
	rightMaster.SetF(0.124);
	rightMaster.SetP(0.12);
	rightMaster.SetI(0.0000);
	rightMaster.SetD(0.0);
	rightMaster.SetAllowableClosedLoopErr(10);
	rightMaster.SetInverted(true);
	
	//rightSlave.SetControlMode(CANTalon::ControlMode::kFollower);
	rightSlave.SetModeSelect(CanTalonSRX::kMode_SlaveFollower);
	rightSlave.Set(Constants::driveRightMasterID);
	rightSlave.SetDemand(rightMasterDeviceID);
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

void DriveTrain::TurnToAngle(float absAngle)
{
	SmartDashboard::PutString("status", "TurnToAngle turn started");

	float k_P = 0.008;

	// Convert target to +/- 180 degrees to match gyro
	float targetAngle = (absAngle <= 180.0) ? absAngle : absAngle-360;
	float currentAngle = position->GetAngleDegrees();
	float error = targetAngle-currentAngle;

	// Take the shortest path to targetAngle
	error = (error > 180) ? error - 360 : error;
	error = (error < -180) ? error + 360 : error;

	auto scaleOutput = [](float output)
	{
		bool tooLowAndNonZero = abs(output) > Constants::drivePIDMinSpeed
			&& abs(output) < Constants::drivePIDFinishTurn;
		bool tooHigh = abs(output) > Constants::drivePIDMaxSpeed;

		if (tooLowAndNonZero)
			output = copysign(Constants::drivePIDFinishTurn, output);
		if (tooHigh)
			output = Constants::drivePIDMaxSpeed;

		return output;
	};

	SmartDashboard::PutNumber("targetAngle", targetAngle);
	SmartDashboard::PutNumber("errorAngle", error);

	unsigned int failsafe = 0;
	float delta_t = 0.02;
	unsigned int failsafeMax = static_cast<unsigned int>(3.0 / delta_t); // Two seconds timeout

	while(abs(error) > Constants::drivePIDepsilon && failsafe < failsafeMax)
	{
		float motorOutput = scaleOutput(k_P * error);
		SmartDashboard::PutNumber("TurnPower", motorOutput);
		TankDriveSpeed(motorOutput, -motorOutput);

		Wait(delta_t);
		failsafe++;

		currentAngle = position->GetAngleDegrees();
		error = targetAngle - currentAngle;
		error = (abs(error) > 180) ? error - copysign(360.0, error) : error;

		SmartDashboard::PutNumber("ErrorAngle", abs(error));
	}

	if (failsafe == failsafeMax)
	{
		SmartDashboard::PutString("status", "driveTrain.TurnToAngle() failsafe hit");
	}
	else
	{
		SmartDashboard::PutString("status", "driveTrain.TurnToAngle() completed");
	}

	TankDriveSpeed(0, 0);
}

void DriveTrain::TurnToRelativeAngle(float angle)
{
	TurnToAngle(angle + position->GetAngleDegrees());
}

void DriveTrain::TankDriveStraight(float speed, float fieldAngle)
{
	// Convert target to +/- 180 degrees to match gyro
	float targetAngle = (fieldAngle <= 180.0) ? fieldAngle : fieldAngle-360;
	float currentAngle = position->GetAngleDegrees();
	float error = targetAngle-currentAngle;

	// Take the shortest path to targetAngle
	error = (error > 180) ? error - 360 : error;
	error = (error < -180) ? error + 360 : error;

	float pidAdjustment = Constants::driveK_P * error;

	bool tooLowAndNonZero = abs(pidAdjustment) > Constants::drivePIDMinSpeed
		&& abs(pidAdjustment) < Constants::drivePIDFinishTurn;
	bool tooHigh = abs(pidAdjustment) > Constants::drivePIDMaxSpeed;

	if (tooLowAndNonZero)
		pidAdjustment = copysign(Constants::drivePIDFinishTurn, pidAdjustment);
	if (tooHigh)
		pidAdjustment = Constants::drivePIDMaxSpeed;

	float motorOutput;
	if (abs(error) > 20.0)
		TankDriveSpeed(pidAdjustment, -pidAdjustment);
	else
		TankDriveSpeed(speed + pidAdjustment, speed - pidAdjustment);
}

void DriveTrain::TankDriveSpeed(float leftspeed, float rightspeed)
{
	leftspeed = (std::abs(leftspeed) <= Constants::drivePIDMinSpeed) ? 0.0 : leftspeed;
	rightspeed = (std::abs(rightspeed) <= Constants::drivePIDMinSpeed) ? 0.0 : rightspeed;
	leftMaster.SetControlMode(CANTalon::ControlMode::kSpeed);
	rightMaster.SetControlMode(CANTalon::ControlMode::kSpeed);
	leftMaster.Set(leftspeed * Constants::driveMaxRPM);
	rightMaster.Set(rightspeed * Constants::driveMaxRPM);
	SmartDashboard::PutNumber("LeftError", leftMaster.GetClosedLoopError());
	SmartDashboard::PutNumber("RightError", rightMaster.GetClosedLoopError());
}
