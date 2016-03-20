#include "Arm.h"

Arm::Arm(uint32_t beltMoverID) :
beltMover(beltMoverID)
{
	beltMover.SetControlMode(CANTalon::ControlMode::kPercentVbus);
}

void Arm::Enable()
{
	/* if (!ArmMover.IsEnabled())
		ArmMover.Enable();
	 */

	if (!beltMover.IsEnabled())
		beltMover.Enable();
}

void Arm::Disable()
{
	/* if (ArmMover.IsEnabled())
		ArmMover.Disable();
	*/

	if (beltMover.IsEnabled())
		beltMover.Disable();
}
void Arm::ManualMoveBelt(float speed) {
	beltMover.Set(speed);
}
void Arm::MoveBelt (float speed)
{
	if(beltMover.IsFwdLimitSwitchClosed() && speed > 0) /*upper limit switch hit && speed > 0*/
	{
	beltMover.Set(0);
	}
	else if(beltMover.IsRevLimitSwitchClosed() && speed < 0)/*lower limit switch hit && speed < 0*/
	{
	beltMover.Set(0);
	}
	else
	{
		beltMover.Set(speed);
	}
}

void Arm::PositionBelt (float position)
{

}

/* void Arm::SetAngle (float angle)
{

}

void Arm::OpenSallyPort ()
{

}

void Arm::OpenDrawbridge ()
{

} */

void Arm::OpenPortcullis ()
{

}

void Arm::MoveSeesaw ()
{

}
