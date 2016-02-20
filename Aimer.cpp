#include "Aimer.h"

Aimer::Aimer()
{
	table = NetworkTable::GetTable("datatable");
}

float Aimer::GetAngleToTower()
{
	return (float)table->GetNumber("averageAzimuthOut", 42);
}

float Aimer::GetDistanceToTower()
{
	return (float)table->GetNumber("averageDistanceOut", 42);
}

float Aimer::GetSpeedToShoot()
{
	return 42;
}

float Aimer::GetAngleToShoot()
{
	return (float)table->GetNumber("averageShootyAngleOut", 42);
}
