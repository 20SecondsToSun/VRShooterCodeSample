// The Family

#include "SVRBasicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASVRBasicProjectile::ASVRBasicProjectile()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.5f;
	ProjectileMovementComponent->Velocity = FVector(1000.0f, 0.0f, 0.0f);
	ProjectileMovementComponent->bShouldBounce = false;
}

