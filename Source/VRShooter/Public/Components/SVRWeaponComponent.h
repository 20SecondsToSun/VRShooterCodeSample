// The Family

#pragma once

#include "CoreMinimal.h"
#include "SVRPlayerBaseComponent.h"
#include "../Types/SVRVCharacterTypes.h"
#include "SVRWeaponComponent.generated.h"

class ASVRBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRSHOOTER_API USVRWeaponComponent : public USVRPlayerBaseComponent
{
	GENERATED_BODY()

public:	
	USVRWeaponComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Transient, ReplicatedUsing = OnRep_CurrentWeapon, Category = "Weapon")
		ASVRBaseWeapon* CurrentWeapon;

public:		
	UPROPERTY(Transient, Replicated)
		TArray<ASVRBaseWeapon*> Inventory;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<ASVRBaseWeapon> > DefaultInventoryClasses;

	UFUNCTION()
		void OnRep_CurrentWeapon(ASVRBaseWeapon* LastWeapon);

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerEquipWeapon(ASVRBaseWeapon* NewWeapon);

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName LeftWeaponAttachSocketName = "GripPoint";

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName RightWeaponAttachSocketName = "GripPoint";

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void StartFire();
	void StopFire();
	bool CanFire();
	void ChangeWeapon();

	void AddWeapon(ASVRBaseWeapon* Weapon);
	void RemoveWeapon(ASVRBaseWeapon* Weapon);
	class ASVRBaseWeapon* FindWeapon(TSubclassOf<ASVRBaseWeapon> WeaponClass);

	void EquipWeapon(ASVRBaseWeapon* Weapon);
	void DestroyInventory();

	void SetCurrentWeapon(ASVRBaseWeapon* NewWeapon, ASVRBaseWeapon* LastWeapon = nullptr);
	void SpawnDefaultInventory();	

	virtual void OwnerDead() override;
	virtual void OwnerAlive() override;

	FName GetWeaponAttachSocketName() const;

	void SetShootFromEyes(bool Value);
	void SetPlayerHandType(EPlayerHandType HandType);

private:
	int32 CurrentWeaponIndex = 0;
	bool ShootFromEyes = false;
	EPlayerHandType PlayerHandType;
	FTimerHandle WaitTimer;
};
