// The Family

#pragma once

#include "CoreMinimal.h"
#include "SVRPlayerBaseComponent.h"
#include "Types/SVRVFXTypes.h"
#include "SVRVFXComponent.generated.h"

class UParticleSystem;
class UDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRSHOOTER_API USVRVFXComponent : public USVRPlayerBaseComponent
{
	GENERATED_BODY()

public:	
	USVRVFXComponent();

protected:
	UFUNCTION()
		void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
		void HandlePointDamage(AActor* damagedActor,
			float Damage,
			class AController* instigatedBy,
			FVector hitLocation,
			class UPrimitiveComponent* fHitComponent,
			FName boneName,
			FVector ShotFromDirection,
			const class UDamageType* DamageType,
			AActor* damageCauser);

	UFUNCTION(NetMulticast, Reliable)
		void PlayClientImpactEffects(UParticleSystem* Effect, const FVector& Hit, const FVector& ShotDirection);
	void PlayClientImpactEffects_Implementation(UParticleSystem* Effect, const FVector& Hit, const FVector& ShotDirection);

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		UDataTable* ImpactVFXTable;
	
	void PlayImpactEffects(TSubclassOf<UDamageType> DamageType, const FHitResult& Hit, const FVector& ShotDirection);
};
