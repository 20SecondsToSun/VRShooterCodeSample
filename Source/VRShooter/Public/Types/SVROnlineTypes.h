// The Family

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SVROnlineTypes.generated.h"

UENUM(BlueprintType)
enum class ETeamID : uint8
{
	None,
	First,
	Second,
	Third,
	Fourth
};

USTRUCT(BlueprintType)
struct FTeamData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ETeamID ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor Color;

	FTeamData(ETeamID ID = ETeamID::None, const FString& Name = "None", const FLinearColor& Color = FLinearColor::Black)
		:ID(ID), Name(Name), Color(Color)
	{

	}
};

USTRUCT(BlueprintType)
struct FTeamStat
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
		int32 Frags;

	UPROPERTY(BlueprintReadOnly)
		int32 Deaths;

	UPROPERTY(BlueprintReadOnly)
		int32 Wins;

	UPROPERTY(BlueprintReadOnly)
		ETeamID ID;

	UPROPERTY(BlueprintReadOnly)
		bool HasFlag;

	UPROPERTY(BlueprintReadOnly)
		FString Name;

	UPROPERTY(BlueprintReadOnly)
		FLinearColor Color;

	FTeamStat(ETeamID ID = ETeamID::None,
		const FString& Name = "None",
		const FLinearColor& Color = FLinearColor::Black,
		int32 Frags = 0,
		int32 Deaths = 0,
		int32 Wins = 0,
		bool HasFlag = false) :
		Frags(Frags),
		Deaths(Deaths),
		Wins(Wins),
		HasFlag(HasFlag),
		ID(ID),
		Name(Name),
		Color(Color)
	{
	}
};


USTRUCT(BlueprintType)
struct FPlayerDataStruct
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadOnly)
		ETeamID ID;

	UPROPERTY(BlueprintReadOnly)
		FLinearColor Color;

	UPROPERTY(BlueprintReadOnly)
		FString TeamName;

	FPlayerDataStruct(ETeamID ID = ETeamID::None, const FLinearColor& Color = FLinearColor::Red, const FString& Name = "None")
		:ID(ID), Color(Color), TeamName(Name)
	{

	}
};

USTRUCT(BlueprintType)
struct FNetworkConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
		FName SessionName = "TestName";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
		FName StartupMapName = "TrainLevel";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
		int32 MaxPublicConnections = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
		int32 MaxSearchSessionResult = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
		int32 PingBucketSize = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
		bool IsLANSession = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
		FString Option = TEXT("listen");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
		bool IsLevelOpenAbsolute = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
		bool UsesPresence = true;
	
	FNetworkConfig()
	{

	}
};


