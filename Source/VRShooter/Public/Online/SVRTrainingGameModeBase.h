// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SVRTrainingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VRSHOOTER_API ASVRTrainingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Game)
		virtual void StartPlay() override;	
};
