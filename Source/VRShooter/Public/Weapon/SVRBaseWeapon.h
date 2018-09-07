// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "SVRBaseWeapon.generated.h"

class ASVRBaseCharacter;
class UArrowComponent;
class USoundCue;
class UAudioComponent;
class UCapsuleComponent;
class USkeletalMeshComponent;
class UMeshComponent;
class USVRVFXComponent;
class UDataTable;

UCLASS()
class VRSHOOTER_API ASVRBaseWeapon : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

	virtual void PostInitializeComponents() override;

	void AttachMeshToPawn(USkeletalMeshComponent* Mesh, const FName& AttachPoint);
	void DetachMeshFromPawn();
	bool IsAttachedToPawn() const;

protected:
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditAnywhere, Category = Arrow)
		UArrowComponent* Arrow1P;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
		UCapsuleComponent* InteractCapsule;

	USkeletalMeshComponent* GetWeaponMesh() const;
	UArrowComponent* GetForwardArrow() const;

public:
	ASVRBaseWeapon();

	void OnEquip(USkeletalMeshComponent* Mesh, const FName& AttachPoint);
	virtual void OnUnEquip();
	bool IsEquipped() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
		void ChangePhycisBehaviour(bool Enabled);
	void ChangePhycisBehaviour_Implementation(bool Enabled);

protected:
	/** is equip animation playing? */
	uint32 bPendingEquip : 1;

	/** is weapon currently equipped? */
	uint32 bIsEquipped : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Default")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Default")
		float BaseDamage;

	//void MakeDamage(const FHitResult& Hit, EPhysicalSurface SurfaceType, const FVector& ShotDirection);
	void MakeDamage(const FHitResult& Hit, const FVector& ShotDirection);

public:
	UFUNCTION(Reliable, Server, WithValidation, Category = "Weapon | Default")
		virtual void StartFire();

	UFUNCTION(Reliable, Server, WithValidation, Category = "Weapon | Default")
		virtual void StopFire();

	virtual void SetVisibility(bool value);

protected:
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Default")
		UDataTable* ImpactVFXTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Default")
		UParticleSystem* DefaultImpactEffect;

	UFUNCTION(NetMulticast, Reliable)
		void PlayClientImpactEffects(UParticleSystem* Effect, const FVector& Hit, const FVector& ShotDirection);
	void PlayClientImpactEffects_Implementation(UParticleSystem* Effect, const FVector& Hit, const FVector& ShotDirection);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Default")
		UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Default")
		FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Default")
		UParticleSystem* SleeveEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Default")
		FName SleeveSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Default")
		USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interactable", ReplicatedUsing = OnRep_PhysicsEnabled)
		bool PhysicsEnabled;

	/** current weapon rep handler */
	UFUNCTION()
		void OnRep_PhysicsEnabled(bool bPhysicsEnabled);


	UFUNCTION(NetMulticast, Reliable)
		virtual void PlayMuzzleEffect();
	virtual void PlayMuzzleEffect_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		virtual void PlaySleeveEffect();
	virtual void PlaySleeveEffect_Implementation();

	virtual void PlayImpactEffects(EPhysicalSurface SurfaceType, const FVector& ImpactPoint);

	UAudioComponent* PlayWeaponSound(USoundCue* Sound);

	USVRVFXComponent* TryGetVFXComponent(AActor* HitActor);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		void StartInteract(AActor* IInstigator);
	virtual void StartInteract_Implementation(AActor* IInstigator) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		void StopInteract(AActor* IInstigator);
	virtual void StopInteract_Implementation(AActor* IInstigator) override;

private:
	void EnablePhysics(bool Enabled);

};
