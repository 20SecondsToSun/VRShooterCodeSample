// The Family

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "SVRVFXWeaponTypes.generated.h"

struct FTableRowBase;
class UDataTable;
class UParticleSystem;

USTRUCT(BlueprintType)
struct FVFXDefaultImpactWeaponStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
		TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
		UParticleSystem* Effect;
};