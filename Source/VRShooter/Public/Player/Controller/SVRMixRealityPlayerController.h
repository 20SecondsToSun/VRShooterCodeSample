// The Family

#pragma once

#include "CoreMinimal.h"
#include "Player/Controller/SVRBasePlayerController.h"
#include "SVRMixRealityPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VRSHOOTER_API ASVRMixRealityPlayerController : public ASVRBasePlayerController
{
	GENERATED_BODY()	
	
public:
	virtual void DefineBindControls() override;
};
