// The Family

#include "SVRProjectileWeapon.h"
#include "Engine.h"
#include "Weapon/Bullet/SVRBasicProjectile.h"
#include "../Damage/SVRProjectileDamageType.h"

ASVRProjectileWeapon::ASVRProjectileWeapon()
{
	DamageType = USVRProjectileDamageType::StaticClass();
}

void ASVRProjectileWeapon::StartFire_Implementation()
{
	USkeletalMeshComponent* WeaponMesh = GetWeaponMesh();
	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);

	//FireProjectileOnServer(MuzzleLocation, MuzzleRotation);

	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<ASVRBasicProjectile>(Projectile, MuzzleLocation, MuzzleRotation, SpawnParams);
	PlayMuzzleEffect();
	PlaySleeveEffect();
}

bool ASVRProjectileWeapon::StartFire_Validate()
{
	return true;
}
