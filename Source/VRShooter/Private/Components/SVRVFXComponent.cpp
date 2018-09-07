// The Family

#include "SVRVFXComponent.h"
#include "VRShooter.h"
#include "Net/UnrealNetwork.h"
#include "VRShooter.h"
#include "Engine/DataTable.h"
#include "../Types/SVRVFXTypes.h"

USVRVFXComponent::USVRVFXComponent()
{	
	bReplicates = true;
	SetIsReplicated(true);
	PrimaryComponentTick.bCanEverTick = false;
}

void USVRVFXComponent::PlayImpactEffects(TSubclassOf<UDamageType> DamageType, const FHitResult& Hit, const FVector& ShotDirection)
{
	UParticleSystem* SelectedEffect = nullptr;

	if (ImpactVFXTable)
	{
		TArray<FVFXImpactStruct*> Rows;
		FString Context;
		ImpactVFXTable->GetAllRows<FVFXImpactStruct>(Context, Rows);

		for (auto& Row : Rows)
		{
			if (Row->DamageType == DamageType)
			{
				SelectedEffect = Row->Effect;
				EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
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
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, ShotDirection.Rotation(), true);
		PlayClientImpactEffects(SelectedEffect, Hit.ImpactPoint, ShotDirection);
	}
}

void USVRVFXComponent::PlayClientImpactEffects_Implementation(UParticleSystem* Effect, const FVector& Hit, const FVector& ShotDirection)
{
	if (Effect)
	{	
		//if(GetOwner()->Role < ROLE_Authority)
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Client impact"));	
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, Hit, ShotDirection.Rotation(), true);
	}
}

void USVRVFXComponent::HandlePointDamage(AActor* damagedActor,
	float Damage, 
	class AController* InstigatedBy,
	FVector HitLocation, 
	class UPrimitiveComponent* HitComponent, 
	FName BoneName,
	FVector ShotFromDirection, 
	const class UDamageType* DamageType, 
	AActor* DamageCauser) 
{

}

void USVRVFXComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy,
	AActor* DamageCauser)
{
	
}

