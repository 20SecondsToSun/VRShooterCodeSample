// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "SVRFlag.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UMeshComponent;

UCLASS()
class VRSHOOTER_API ASVRFlag : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASVRFlag();

	virtual void BeginPlay() override;


protected:
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
		UCapsuleComponent* PhysCapsule;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Interactable")
		UCapsuleComponent* InteractCapsule;

	UPROPERTY(BlueprintReadOnly, Category = "Interactable", ReplicatedUsing = OnRep_PhysicsEnabled)
		bool PhysicsEnabled;

	/** current weapon rep handler */
	UFUNCTION()
		void OnRep_PhysicsEnabled(bool bPhysicsEnabled);

public:		
	void OnEquip(USkeletalMeshComponent* Mesh, const FName& AttachPoint);
	virtual void OnUnEquip();
	bool IsEquipped() const;

private:
	bool bIsEquipped = false;

	void AttachMeshToPawn(USkeletalMeshComponent* Mesh, const FName& AttachPoint);
	void DetachMeshFromPawn();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		void StartInteract(AActor* IInstigator);
	virtual void StartInteract_Implementation(AActor* IInstigator) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		void StopInteract(AActor* IInstigator);
	virtual void StopInteract_Implementation(AActor* IInstigator) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
		void ChangePhycisBehaviour(bool Enabled);
	void ChangePhycisBehaviour_Implementation(bool Enabled);

private:
	void EnablePhysics(bool Enabled);
	
};
