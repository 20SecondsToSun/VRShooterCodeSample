// The Family

#pragma once

#include "CoreMinimal.h"
#include "SVRPlayerBaseComponent.h"
#include "SVRInteractableComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRSHOOTER_API USVRInteractableComponent : public USVRPlayerBaseComponent
{
	GENERATED_BODY()

public:	
	USVRInteractableComponent();

public:	
	UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation, Category = "Interactable")
		void TryInteractableBySphereTrace(const FVector& StartLocation, const FVector& EndLocation, float SphereRadius = 100.0f, bool DebugDraw = false);
	
	UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation, Category = "Interactable")
		void SearchInteractableBySphereTrace(const FVector& StartLocation, const FVector& EndLocation, float SphereRadius, bool DebugDraw);
	
	virtual void OwnerDead() override;
	virtual void OwnerAlive() override;

private:
	bool ScanSpaceBySphere(TArray<FHitResult>& HitResults, const FVector& StartLocation, const FVector& EndLocation, float SphereRadius, bool DebugDraw);
	AActor* LastInteractableActor = nullptr;

	UFUNCTION(Client, Reliable, Category = "Interactable")
	void HighLightMeshes(AActor* InteractableActor);

	UFUNCTION(Client, Reliable, Category = "Interactable")
	void SnuffOutMeshes(AActor* InteractableActor);

	void SnuffOutLast();
};
