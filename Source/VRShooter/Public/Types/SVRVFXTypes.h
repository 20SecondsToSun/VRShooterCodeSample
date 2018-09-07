// The Family

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/DamageType.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "SVRVFXTypes.generated.h"

struct FTableRowBase;
class UDataTable;
class UDamageType;
class UParticleSystem;

USTRUCT(BlueprintType)
struct FVFXImpactStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
		TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
		UParticleSystem* Effect;
};