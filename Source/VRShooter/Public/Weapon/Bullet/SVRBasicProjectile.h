// The Family

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Bullet/SVRBasicBullet.h"
#include "SVRBasicProjectile.generated.h"

class UProjectileMovementComponent;
/**
 * 
 */
UCLASS()
class VRSHOOTER_API ASVRBasicProjectile : public ASVRBasicBullet
{
	GENERATED_BODY()

public:
	ASVRBasicProjectile();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Bullet | Projectile")
		UProjectileMovementComponent* ProjectileMovementComponent;
};
