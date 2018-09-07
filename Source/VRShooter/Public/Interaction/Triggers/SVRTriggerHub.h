// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SVRTriggerHub.generated.h"

class UChildActorComponent;
class ASVRTriggerSlave;
class ASVRTriggerMaster;

UCLASS()
class VRSHOOTER_API ASVRTriggerHub : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASVRTriggerHub();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Transient, Category = "TriggerHub")
		float InteractionTime = 3.0f;

	UPROPERTY(VisibleDefaultsOnly, Category = Scene)
		USceneComponent* SceneComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "TriggerHub")
		UChildActorComponent* TriggerMasterComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "TriggerHub")
		UChildActorComponent* TriggerSlaveComponent;

	UFUNCTION(BlueprintNativeEvent, Category = "TriggerHub")
		void InteractionPercentUpdate(float Percent);
	void InteractionPercentUpdate_Implementation(float Percent);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnMasterStateChanged(ETriggerState State);

	UFUNCTION()
	void OnSlaveStateChanged(ETriggerState State);

	UFUNCTION()
	void OnProgressChanged(float Percent);

	float InteractionTimeStart = 0.0f;

	void OnInteractionTimerUpdate();

	float InteractionPercent = 0.0f;

	FTimerHandle InteractionTimer;

	ASVRTriggerSlave* TriggerSlave;
	ASVRTriggerMaster* TriggerMaster;
	
};
