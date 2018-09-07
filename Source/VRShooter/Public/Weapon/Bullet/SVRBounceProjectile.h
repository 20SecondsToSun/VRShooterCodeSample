// The Family

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Bullet/SVRBasicProjectile.h"
#include "SVRBounceProjectile.generated.h"

/**
 * 
 */
UCLASS()
class VRSHOOTER_API ASVRBounceProjectile : public ASVRBasicProjectile
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Bullet | BounceProjectile")
		int32 MaxBounces = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet | BounceProjectile")
		int32 BouncesCount = 0;
	
};
