// The Family

#pragma once

#include "CoreMinimal.h"
#include "Weapon/SVRTraceWeapon.h"
#include "SVRShootgunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class VRSHOOTER_API ASVRShootgunWeapon : public ASVRTraceWeapon
{
	GENERATED_BODY()

public:
	ASVRShootgunWeapon();

protected:
	UFUNCTION(Reliable, Server, WithValidation, Category = "Weapon | Default")
		virtual void StartFire() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Shootgun")
		int BulletsNum = 5;
};
