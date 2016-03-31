#include "DriveTrain.h"

DriveTrain::DriveTrain(uint32_t leftMasterDeviceID, uint32_t rightMasterDeviceID, float P, float I, float D, float F, Position *position_) :
	RobotDrive(leftMaster, rightMaster),
	leftMaster(leftMasterDeviceID),
	rightMaster(rightMasterDeviceID),
	pidOutput(0),
	pidController(P, I, D, F, position->PtrToMXP(), this, 0.02),
	position(position_)
{
	pidController.SetInputRange(-180.0, 180.0);
	pidController.SetContinuous(true);

	leftMaster.SetControlMode(CANTalon::ControlMode::kSpeed);
	leftMaster.SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Absolute);
	leftMaster.SetSensorDirection(false);
	leftMaster.SetReversed(true);
	leftMaster.SelectProfileSlot(0);
	leftMaster.SetF(0.1);
	leftMaster.SetP(0);
	leftMaster.SetI(0);
	leftMaster.SetD(0);
	
	rightMaster.SetControlMode(CANTalon::ControlMode::kSpeed);
	rightMaster.SetFeedbackDevice(CANTalon::FeedbackDevice::CtreMagEncoder_Absolute);
	rightMaster.SetSensorDirection(false);
	rightMaster.SelectProfileSlot(0);
	rightMaster.SetF(0.1);
	rightMaster.SetP(0);
	rightMaster.SetI(0);
	rightMaster.SetD(0);
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
	pidController.SetPID(SmartDashboard::GetNumber("kF", 0.0), SmartDashboard::GetNumber("kP", 0.0), SmartDashboard::GetNumber("kI", 0.0), SmartDashboard::GetNumber("kD", 0.0));
	pidController.Enable();
	auto failsafe = 0;
	auto delta_t = 0.02;

	while (abs(pidController.GetAvgError()) > 2 && failsafe < 3.0/delta_t)
	{
		leftMaster.Set(pidOutput);
		rightMaster.Set(-pidOutput);
		Wait(delta_t);
	}

	pidController.Disable();
	leftMaster.Set(0);
	rightMaster.Set(0);
}

void DriveTrain::TurnToRelativeAngle(float angle) {
	TurnToAngle(angle + position->GetAngle());
}

void DriveTrain::DriveStraight(float speed, float fieldAngle, float timeInSeconds)
{
}

void DriveTrain::TankDriveSpeed(float leftspeed, float rightspeed)
{
	leftMaster.SetControlMode(CANTalon::ControlMode::kSpeed);
	rightMaster.SetControlMode(CANTalon::ControlMode::kSpeed);
	leftMaster.Set(leftspeed);
	rightMaster.Set(rightspeed);
}

void DriveTrain::PIDWrite(float output)
{
	pidOutput = output;
}
