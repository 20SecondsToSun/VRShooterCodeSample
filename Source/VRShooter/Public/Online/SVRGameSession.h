// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "SVRGameSession.generated.h"

/**
 * 
 */
UCLASS()
class VRSHOOTER_API ASVRGameSession : public AGameSession
{
	GENERATED_BODY()

	const int32 MaxPlayersDefault = 2;	
	virtual void InitOptions(const FString& Options) override;
};
