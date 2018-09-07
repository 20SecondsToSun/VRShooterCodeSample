// The Family

#pragma once

#include "CoreMinimal.h"
#include "Weapon/SVRBaseWeapon.h"
#include "Types/SVRTraceWeaponTypes.h"
#include "SVRTraceWeapon.generated.h"

/**
 * 
 */
UCLASS()
class VRSHOOTER_API ASVRTraceWeapon : public ASVRBaseWeapon
{
	GENERATED_BODY()

public:
	ASVRTraceWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Trace", meta = (ClampMin = 0.0f))
		float BulletSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Trace")
		float TraceMaxDistance = 10000;

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
		FHitScanTrace HitScanTrace;

	UFUNCTION()
		void OnRep_HitScanTrace();

	FHitScanTraceData TraceData;

	virtual void CalculateTraceData();

	UFUNCTION(Reliable, Server, WithValidation, Category = "Weapon | Trace")
		void MakeSingleTrace(const FHitScanTraceData& TraceData);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Trace")
		UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Trace")
		FName TracerTargetName;

	UFUNCTION(NetMulticast, Reliable)
		void PlayTracerEffectMulti(const FVector& ImpactPoint);
	void PlayTracerEffectMulti_Implementation(const FVector& ImpactPoint);

	virtual void PlayTracerEffect(const FVector& ImpactPoint);
};
