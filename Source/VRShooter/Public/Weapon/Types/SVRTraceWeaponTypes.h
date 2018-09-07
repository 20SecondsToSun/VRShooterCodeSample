// The Family

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "SVRTraceWeaponTypes.generated.h"

USTRUCT()
struct FHitScanTraceData
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FVector_NetQuantize TraceEnd;

	UPROPERTY()
		FVector_NetQuantize TraceStart;

	UPROPERTY()
		FVector_NetQuantize ShotDirection;

	UPROPERTY()
		bool Valid = false;

};

// Contains information of a single hitscan weapon linetrace
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
		FVector_NetQuantize TraceTo;
};
