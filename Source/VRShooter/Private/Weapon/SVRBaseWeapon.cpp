// The Family

#include "SVRBaseWeapon.h"
#include "Engine.h"
#include "../Types/SVRPreprocessor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "../Components/SVRWeaponComponent.h"
#include "../Components/SVRVFXComponent.h"
#include "Net/UnrealNetwork.h"
#include "Types/SVRVFXWeaponTypes.h"

// Sets default values
ASVRBaseWeapon::ASVRBaseWeapon()
	:MuzzleSocketName("Muzzle"),
	SleeveSocketName("Sleeve")
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	//SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bNetUseOwnerRelevancy = false;
	bIsEquipped = false;
	bReplicateMovement = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh1P->bReceivesDecals = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetIsReplicated(true);
	Mesh1P->SetupAttachment(RootComponent);
	Mesh1P->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh1P->SetCollisionResponseToAllChannels(ECR_Block);

	Arrow1P = CreateDefaultSubobject<UArrowComponent>(TEXT("Shoot Direction Arrow"));
	Arrow1P->SetupAttachment(Mesh1P, MuzzleSocketName);

	InteractCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCapsule"));
	InteractCapsule->SetCapsuleHalfHeight(60.0);
	InteractCapsule->SetCapsuleRadius(22.0f);
	InteractCapsule->SetupAttachment(Mesh1P);
	InteractCapsule->SetCollisionObjectType(COLLISION_INTERACTABLE);
	InteractCapsule->SetCollisionProfileName("Interactable");
	InteractCapsule->SetRelativeLocation(FVector(0.0f, 20.0f, 5.0f));
	InteractCapsule->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	InteractCapsule->ShapeColor = FColor(0.0, 0.0, 255.0);

	//AddHighlightMesh(Mesh1P);
}

void ASVRBaseWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

/////////////////////////////////////////////////////////////////////////
// Input
void ASVRBaseWeapon::StartFire_Implementation()
{

}

void ASVRBaseWeapon::StopFire_Implementation()
{

}

bool ASVRBaseWeapon::StartFire_Validate()
{
	return true;
}

bool ASVRBaseWeapon::StopFire_Validate()
{
	return true;
}

USVRVFXComponent* ASVRBaseWeapon::TryGetVFXComponent(AActor* HitActor)
{
	if (HitActor)
	{
		TArray<USVRVFXComponent*> VFXComps;
		HitActor->GetComponents<USVRVFXComponent>(VFXComps);
		if (VFXComps.Num() > 0)
		{
			return VFXComps[0];
		}
	}
	return nullptr;
}

void ASVRBaseWeapon::MakeDamage(const FHitResult& Hit, const FVector& ShotDirection)
{
	AActor* HitActor = Hit.GetActor();
	AActor* MyOwner = GetOwner();
	float ActualDamage = BaseDamage;
	USVRVFXComponent* VFXComp = TryGetVFXComponent(HitActor);

	if (HitActor != nullptr && VFXComp != nullptr)
	{
		VFXComp->PlayImpactEffects(DamageType, Hit, ShotDirection);
	}
	else if (HitActor != nullptr)
	{
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		PlayImpactEffects(SurfaceType, Hit.ImpactPoint);
	}

	if (HitActor != nullptr)
	{
		UGameplayStatics::ApplyPointDamage(HitActor,
			ActualDamage,
			ShotDirection,
			Hit,
			MyOwner->GetInstigatorController(),
			MyOwner, DamageType);
	}
}

//////////////////////////////////////////////////////////////////////////
// Effects
void ASVRBaseWeapon::PlayMuzzleEffect_Implementation()
{
	if (MuzzleFlashEffect)
	{
		USkeletalMeshComponent* WeaponMesh = GetWeaponMesh();
		FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffect, MuzzleLocation, MuzzleRotation, true);
	//	UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, WeaponMesh, MuzzleSocketName, MuzzleLocation, MuzzleRotation, EAttachLocation::SnapToTarget, true);
	}
}

void ASVRBaseWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, const FVector& ImpactPoint)
{
	UParticleSystem* SelectedEffect = DefaultImpactEffect;

	if (ImpactVFXTable)
	{
		TArray<FVFXDefaultImpactWeaponStruct*> Rows;
		FString Context;
		ImpactVFXTable->GetAllRows<FVFXDefaultImpactWeaponStruct>(Context, Rows);

		for (auto& Row : Rows)
		{
			if (Row->SurfaceType == SurfaceType)
			{
				SelectedEffect = Row->Effect;

				if (Row->SurfaceType == SurfaceType)
				{
					SelectedEffect = Row->Effect;
					break;
				}
			}
		}
	}

	if (SelectedEffect)
	{
		USkeletalMeshComponent* WeaponMesh = GetWeaponMesh();
		FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation(), true);
		PlayClientImpactEffects(SelectedEffect, ImpactPoint, ShotDirection);
	}
}

void ASVRBaseWeapon::PlayClientImpactEffects_Implementation(UParticleSystem* Effect, const FVector& Hit, const FVector& ShotDirection)
{
	if (Effect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, Hit, ShotDirection.Rotation(), true);
	}
}

void ASVRBaseWeapon::PlaySleeveEffect_Implementation()
{
	if (SleeveEffect)
	{
		USkeletalMeshComponent* WeaponMesh = GetWeaponMesh();
		FVector SleeveLocation = WeaponMesh->GetSocketLocation(SleeveSocketName);
		FRotator SleeveRotation = WeaponMesh->GetSocketRotation(SleeveSocketName);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SleeveEffect, SleeveLocation, SleeveRotation, true);
	}
}

UAudioComponent* ASVRBaseWeapon::PlayWeaponSound(USoundCue* Sound)
{
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Inventory
void ASVRBaseWeapon::OnEquip(USkeletalMeshComponent* Mesh, const FName& AttachPoint)
{
	if (Role == ROLE_Authority)
	{
		EnablePhysics(false);
		AttachMeshToPawn(Mesh, AttachPoint);
		bIsEquipped = true;
	}
}

void ASVRBaseWeapon::OnUnEquip()
{
	if (Role == ROLE_Authority)
	{
		StopFire();
		EnablePhysics(true);
		DetachMeshFromPawn();
		bIsEquipped = false;
	}
}

void ASVRBaseWeapon::AttachMeshToPawn(USkeletalMeshComponent* Mesh, const FName& AttachPoint)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);

	AttachToComponent(Mesh, AttachmentRules, AttachPoint);
}

void ASVRBaseWeapon::DetachMeshFromPawn()
{
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

//////////////////////////////////////////////////////////////////////////
// Interactable interface
void ASVRBaseWeapon::StartInteract_Implementation(AActor* IInstigator)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Weapon Start Interact"));
	if (IInstigator)
	{
		TArray<USVRWeaponComponent*> WeaponComps;
		IInstigator->GetComponents<USVRWeaponComponent>(WeaponComps);
		if (WeaponComps.Num() > 0)
		{
			WeaponComps[0]->EquipWeapon(this);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Has weapon components, can attach"));
		}
	}
}

void ASVRBaseWeapon::StopInteract_Implementation(AActor* IInstigator)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Weapon Stop Interact"));
}

//////////////////////////////////////////////////////////////////////////
// Misc
void ASVRBaseWeapon::EnablePhysics(bool Enabled)
{
	if (Role == ROLE_Authority)
	{
		PhysicsEnabled = Enabled;
		ChangePhycisBehaviour(PhysicsEnabled);
	}
}

void ASVRBaseWeapon::ChangePhycisBehaviour_Implementation(bool Enabled)
{
	if (!Enabled)
	{
		InteractCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Mesh1P->SetCollisionObjectType(ECC_WorldDynamic);
		//Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh1P->SetSimulatePhysics(false);
	}
	else
	{
		InteractCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		//Mesh1P->SetCollisionObjectType(ECC_WorldDynamic);
		//Mesh1P->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//Mesh1P->SetCollisionResponseToAllChannels(ECR_Block);
		Mesh1P->SetSimulatePhysics(true);
	}
}

void ASVRBaseWeapon::SetVisibility(bool value)
{
	RootComponent->SetVisibility(value, true);
	Mesh1P->SetVisibility(value, true);
}

USkeletalMeshComponent* ASVRBaseWeapon::GetWeaponMesh() const
{
	return Mesh1P;
}

UArrowComponent* ASVRBaseWeapon::GetForwardArrow() const
{
	return Arrow1P;
}

bool ASVRBaseWeapon::IsEquipped() const
{
	return bIsEquipped;
}

bool ASVRBaseWeapon::IsAttachedToPawn() const
{
	return bIsEquipped || bPendingEquip;
}

//////////////////////////////////////////////////////////////////////////
// Replication
void ASVRBaseWeapon::OnRep_PhysicsEnabled(bool bPhysicsEnabled)
{
	ChangePhycisBehaviour(PhysicsEnabled);
}

void ASVRBaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASVRBaseWeapon, PhysicsEnabled);
}
