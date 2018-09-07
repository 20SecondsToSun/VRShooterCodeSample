// The Family

#pragma once

#include "CoreMinimal.h"

// Logs
DECLARE_LOG_CATEGORY_EXTERN(VRLog, Log, All);
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 4.5, FColor::White,text)

#define COLLISION_WEAPON			ECC_GameTraceChannel1
#define COLLISION_INTERACTABLE      ECC_GameTraceChannel8
