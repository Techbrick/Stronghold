#include "Aimer.h"

Aimer::Aimer()
{
	table = NetworkTable::GetTable("datatable");
}

float Aimer::GetAngleToTower()
{
	return 0;
}

float Aimer::GetDistanceToTower()
{
	return 0;
}

float Aimer::GetSpeedToShoot()
{
	return 0;
}

float Aimer::GetAngleToShoot()
{
	return 0;
}
