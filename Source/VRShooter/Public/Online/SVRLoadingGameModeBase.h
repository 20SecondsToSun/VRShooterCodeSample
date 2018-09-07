// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SVRLoadingGameModeBase.generated.h"

UCLASS()
class VRSHOOTER_API ASVRLoadingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Game)
	virtual void StartPlay() override;

private:
	FTimerHandle LoadTimerHandle;
	void LoadTrainLevel();
};
