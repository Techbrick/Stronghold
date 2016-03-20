#include <WPILib.h>

#ifndef ARM_H_
#define ARM_H_

class Arm {

	CANTalon beltMover;

public:
	Arm(uint32_t beltMover);

	void Enable();

	void Disable();

	void ManualMoveBelt(float speed);

	void MoveBelt(float speed);

	void PositionBelt (float position);

	void OpenPortcullis();

	void MoveSeesaw();

	/* void SetAngle(float angle);

	void OpenSallyPort();

	void OpenDrawbridge(); */

};

#endif /* ARM_H_ */
