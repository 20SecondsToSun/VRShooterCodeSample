// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SVRTriggerBase.h"
#include "SVRTriggerSlave.generated.h"

UCLASS()
class VRSHOOTER_API ASVRTriggerSlave : public ASVRTriggerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASVRTriggerSlave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
