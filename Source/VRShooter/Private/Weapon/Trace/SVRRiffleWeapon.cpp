// The Family

#include "SVRRiffleWeapon.h"
#include "TimerManager.h"
#include "VRShooter.h"
#include "Engine.h"
#include "../Damage/SVRRiffleDamageType.h"
#include "Net/UnrealNetwork.h"

ASVRRiffleWeapon::ASVRRiffleWeapon()
{
	RateOfFire = 600;
	BurstCounter = 0;
	DamageType = USVRRiffleDamageType::StaticClass();
}

void ASVRRiffleWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 60 / RateOfFire;
}

void ASVRRiffleWeapon::OnRep_BurstCounter()
{
	if (BurstCounter > 0)
	{
		//PlayMuzzleEffect();
		CalculateTraceData();
	}
}

void ASVRRiffleWeapon::StartFire_Implementation()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASVRRiffleWeapon::BurstOneShot, TimeBetweenShots, true, FirstDelay);
}

bool ASVRRiffleWeapon::StartFire_Validate()
{
	return true;
}

void ASVRRiffleWeapon::BurstOneShot()
{
	BurstCounter++;
	CalculateTraceData();
	MakeSingleTrace(TraceData);
	PlayMuzzleEffect();
	LastFireTime = GetWorld()->TimeSeconds;
}

void ASVRRiffleWeapon::StopFire_Implementation()
{
	BurstCounter = 0;
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

bool ASVRRiffleWeapon::StopFire_Validate()
{
	return true;
}

void ASVRRiffleWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASVRRiffleWeapon, BurstCounter);
}
