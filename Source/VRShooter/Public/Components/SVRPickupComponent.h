// The Family

#pragma once

#include "CoreMinimal.h"
#include "SVRPlayerBaseComponent.h"
#include "SVRPickupComponent.generated.h"

class ASVRFlag;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRSHOOTER_API USVRPickupComponent : public USVRPlayerBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USVRPickupComponent();

public:	
	void EquipFlag(ASVRFlag* Flag);

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerEquipFlag(ASVRFlag* NewWeapon);

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName FlagAttachSocketName = "FlagSocket";

	void DropPickup();

	virtual void OwnerDead() override;
	virtual void OwnerAlive() override;

private:
	ASVRFlag* CurrentFlag;
	void SetCurrentFlag(ASVRFlag* Flag);	
};
