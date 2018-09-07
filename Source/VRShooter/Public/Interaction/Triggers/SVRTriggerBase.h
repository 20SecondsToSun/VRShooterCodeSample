// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SVRTriggerBase.generated.h"

UENUM(BlueprintType)
enum class ETriggerState : uint8
{	
	Deactive 	UMETA(DisplayName = "Deactive"),
	Active 		UMETA(DisplayName = "Active")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, ETriggerState, TriggerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProgressChanged, float, Percent);

UCLASS()
class VRSHOOTER_API ASVRTriggerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASVRTriggerBase();

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnStateChanged StateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnProgressChanged ProgressChanged;

	UFUNCTION(NetMulticast, Reliable)
		virtual void SetState(ETriggerState State);
	virtual void SetState_Implementation(ETriggerState State);

	UFUNCTION(BlueprintPure, Category = "Trigger")
	bool IsActive() const;

	UFUNCTION(BlueprintCallable, Category = "Trigger")
		void SetProgress(float Percent);

	UFUNCTION(BlueprintPure, Category = "Trigger")
		float GetProgress() const;

protected:	
	UPROPERTY(Transient, Replicated)
		ETriggerState State = ETriggerState::Deactive;

	float Percent;
};
