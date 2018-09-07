// The Family

#include "SVRShootgunWeapon.h"
#include "../Damage/SVRShootgunDamageType.h"

ASVRShootgunWeapon::ASVRShootgunWeapon()
{
	DamageType = USVRShootgunDamageType::StaticClass();
}

void ASVRShootgunWeapon::StartFire_Implementation()
{
	for (int i = 0; i < BulletsNum; ++i)
	{
		CalculateTraceData();
		MakeSingleTrace(TraceData);
	}

	PlayMuzzleEffect();
	PlaySleeveEffect();
}

bool ASVRShootgunWeapon::StartFire_Validate()
{
	return true;
}
