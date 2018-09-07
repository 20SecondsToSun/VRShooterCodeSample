// The Family

#include "SVRSMGWeapon.h"
#include "TimerManager.h"
#include "Engine.h"
#include "../Damage/SVRSMGDamageType.h"
#include "Net/UnrealNetwork.h"

ASVRSMGWeapon::ASVRSMGWeapon()
{
	RateOfFire = 600;
	BurstCounter = 0;
	DamageType = USVRSMGDamageType::StaticClass();
}

void ASVRSMGWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 60 / RateOfFire;
}

void ASVRSMGWeapon::OnRep_BurstCounter()
{
	if (BurstCounter > 0)
	{
		//PlayMuzzleEffect();
		CalculateTraceData();
	}
}

void ASVRSMGWeapon::StartFire_Implementation()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASVRSMGWeapon::BurstOneShot, TimeBetweenShots, true, FirstDelay);
}

bool ASVRSMGWeapon::StartFire_Validate()
{
	return true;
}

void ASVRSMGWeapon::BurstOneShot()
{
	BurstCounter++;
	CalculateTraceData();
	MakeSingleTrace(TraceData);
	PlayMuzzleEffect();
	PlaySleeveEffect();
	LastFireTime = GetWorld()->TimeSeconds;
}

void ASVRSMGWeapon::StopFire_Implementation()
{
	BurstCounter = 0;
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

bool ASVRSMGWeapon::StopFire_Validate()
{
	return true;
}

void ASVRSMGWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASVRSMGWeapon, BurstCounter);
}
