#include "WPILib.h"
#include <memory>
#include "NetworkTables/NetworkTable.h"

#ifndef SRC_AIMER_H
#define SRC_AIMER_H


class Aimer
{
public:
	Aimer();

	std::shared_ptr<NetworkTable> table;

	float GetAngleToTower();
	float GetAngleToShoot();
	float GetSpeedToShoot();
	float GetDistanceToTower();
};

#endif
