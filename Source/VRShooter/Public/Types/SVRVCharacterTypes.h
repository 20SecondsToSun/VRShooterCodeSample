// The Family

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SVRVCharacterTypes.generated.h"

UENUM(BlueprintType)
enum class EPlayerHandType : uint8
{
	Left,
	Right
};

UENUM(BlueprintType)
enum class EClientLaunchType : uint8
{
	Desktop UMETA(DisplayName = "Desktop"),
	Oculus UMETA(DisplayName = "Oculus"),
	HTC UMETA(DisplayName = "HTC"),
	MixedReality UMETA(DisplayName = "MixedReality")
};

UENUM(BlueprintType)
enum class EClientControllerType : uint8
{
	Keyboard UMETA(DisplayName = "Keyboard"),
	OculusTouch UMETA(DisplayName = "OculusTouch"),
	HTCTouch UMETA(DisplayName = "HTCTouch"),
	MixedRealityTouch UMETA(DisplayName = "MixedRealityTouch")
};
