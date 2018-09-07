// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SVRTriggerBase.h"
#include "InteractableInterface.h"
#include "SVRTriggerMaster.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHighlightInteractableChanged, bool, Highlight);

class UMeshComponent;

UCLASS()
class VRSHOOTER_API ASVRTriggerMaster : public ASVRTriggerBase, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	ASVRTriggerMaster();
	virtual void SetState(ETriggerState State) override;

protected:
	virtual void BeginPlay() override;
	
public:	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		void StartInteract(AActor* IInstigator);
		virtual void StartInteract_Implementation(AActor* IInstigator) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		void StopInteract(AActor* IInstigator);
		virtual void StopInteract_Implementation(AActor* IInstigator) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void OnHighlightInteractableChanged(bool Value);
		virtual void OnHighlightInteractableChanged_Implementation(bool Value) override;

	UPROPERTY(BlueprintAssignable, Category = "Interactable")
		FOnHighlightInteractableChanged HighlightStateChanged;
};
