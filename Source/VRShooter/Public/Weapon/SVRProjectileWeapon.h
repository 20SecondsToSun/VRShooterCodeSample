// The Family

#pragma once

#include "CoreMinimal.h"
#include "SVRBaseWeapon.h"
#include "SVRProjectileWeapon.generated.h"

class ASVRBasicProjectile;

/**
 * 
 */
UCLASS()
class VRSHOOTER_API ASVRProjectileWeapon : public ASVRBaseWeapon
{
	GENERATED_BODY()

public:
	ASVRProjectileWeapon();

protected:
	UFUNCTION(Reliable, Server, WithValidation, Category = "Weapon | Default")
		virtual void StartFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Projectile")
		TSubclassOf<ASVRBasicProjectile> Projectile;
};
