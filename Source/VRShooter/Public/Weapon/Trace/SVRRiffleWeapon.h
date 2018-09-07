// The Family

#pragma once

#include "CoreMinimal.h"
#include "Weapon/SVRTraceWeapon.h"
#include "SVRRiffleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class VRSHOOTER_API ASVRRiffleWeapon : public ASVRTraceWeapon
{
	GENERATED_BODY()

public:
	ASVRRiffleWeapon();

	UFUNCTION(Reliable, Server, WithValidation, Category = "Weapon | Default")
		virtual void StartFire() override;

	UFUNCTION(Reliable, Server, WithValidation, Category = "Weapon | Default")
		virtual void StopFire() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Riffle")
		float RateOfFire;

	UPROPERTY(ReplicatedUsing = OnRep_BurstCounter)
		int32 BurstCounter;

	UFUNCTION()
		void OnRep_BurstCounter();

	virtual void BurstOneShot();

	float TimeBetweenShots;
	float LastFireTime;
	FTimerHandle TimerHandle_TimeBetweenShots;
};
