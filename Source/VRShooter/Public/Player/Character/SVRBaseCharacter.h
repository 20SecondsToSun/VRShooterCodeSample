// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "../../Types/SVRVCharacterTypes.h"
#include "SVRBaseCharacter.generated.h"

class ASVRBaseWeapon;
class ASVRFlag;
class UCameraComponent;
class USpringArmComponent;
class USkeletalMesh;
class UMaterialInstanceDynamic;
class UMaterialInstance;
class UTextRenderComponent;
class USVRHealthComponent;
class USVRWeaponComponent;
class USVRPickupComponent;
class USVRInteractableComponent;
class USVRVFXComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpectatorModeChanged, bool, SpectatorMode);

UCLASS()
class VRSHOOTER_API ASVRBaseCharacter : public ACharacter 
{
	GENERATED_BODY()

public:
	ASVRBaseCharacter();	

protected:
	virtual void PostInitializeComponents() override;

private:	
	virtual void PossessedBy(class AController* C) override;		
	virtual void OnRep_PlayerState() override;	

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UTextRenderComponent* TextRenderer;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
		USkeletalMeshComponent* WeaponDebugMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		UMaterialInstance* FirstPlayerMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		UMaterialInstance* ThirdPlayerMaterial;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName FlagAttachSocketName = "FlagSocket";

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName MeshColorParam = "BodyColor";

	UPROPERTY(Transient, BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_MeshColor)
		FLinearColor MeshColor;

	UFUNCTION()
		void OnRep_MeshColor();

public:	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	USkeletalMeshComponent* GetPawnMesh() const;
	
	virtual void MoveForward(float Value);

	virtual void MoveRight(float Value);

private:
	UPROPERTY(EditDefaultsOnly)
		UMaterialInstanceDynamic* FirstPlayerDynMaterial;

	UPROPERTY(EditDefaultsOnly)
		UMaterialInstanceDynamic* ThirdPlayerDynMaterial;	

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		bool ShootFromEyes = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		EPlayerHandType PlayerHandType = EPlayerHandType::Right;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		USVRWeaponComponent* WeaponComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX")
		USVRVFXComponent* VFXComp;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow")
		UArrowComponent* Arrow;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void StartFire();

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Weapon")
		void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void ChangeWeapon();

	virtual FVector GetPawnViewLocation() const override;

	FVector GetForwardVector() const;
	FVector GetRightVector() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
		USVRHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickupComp")
		USVRPickupComponent* PickupComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractableComp")
		USVRInteractableComponent* InteractableComp;

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
		void OnHealthChangedEvent(float Health);
	void OnHealthChangedEvent_Implementation(float Health);

	UFUNCTION()
		void OnHealthChanged(USVRHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health")
		bool IsAlive() const;	

public:
	void Heal();	
	virtual void UpdateTeamData();	

private:
	TArray<class USVRPlayerBaseComponent*> PlayerComponents;

	void UpdateTeamColorsAllMIDs();
	void UpdateTeamColors(UMaterialInstanceDynamic* UseMID);

protected:
	UPROPERTY(Transient)
		TArray<UMaterialInstanceDynamic*> MeshMIDs;

public:	
	UPROPERTY(Transient, Replicated)
		bool SpectatorMode = false;	

	UPROPERTY(BlueprintAssignable)
		FOnSpectatorModeChanged OnSpectatorModeChanged;

	UFUNCTION(Reliable, Server, WithValidation)
		void ForceRespawnHandler();

	UFUNCTION(Reliable, Server, WithValidation)
		void DropPickup();

	virtual void ChangeSpectatorMode(bool SpectatorMode);
	void MakeDead();	
};
