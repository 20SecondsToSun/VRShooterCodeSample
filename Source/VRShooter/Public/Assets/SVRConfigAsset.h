// The Family

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Gameplay/SVRFlagSpawner.h"
#include "./Player/Character/SVRMixRealityCharacter.h"
#include "./Player/Character/SVRDesktopCharacter.h"
#include "./Types/SVROnlineTypes.h"
#include "../Types/SVRVCharacterTypes.h"
#include "SVRConfigAsset.generated.h"

/**
*
*/
class ASVRFlagSpawner;

template <typename EnumType>
static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!Enum)
	{
		return EnumType(0);
	}
	return (EnumType)Enum->GetValueByName(FName(*String));
}

template<typename TEnum>
static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetNameByValue((int64)Value).ToString();
}

template<typename TEnum>
static FORCEINLINE FText GetEnumMetaText(const FString& Name, const FName& Key)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr)
	{
		return TEXT("Invalid");
	}
	return enumPtr->GetMetaDataText(Key);
}

template<typename TEnum>
static FORCEINLINE FString GetEnumDisplayName(const FString& Name, TEnum Value)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetDisplayNameTextByIndex((int64)Value).ToString();
}

UCLASS(BlueprintType)
class VRSHOOTER_API USVRConfigAsset : public UObject
{
	GENERATED_BODY()

public:
	USVRConfigAsset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool ResetSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool SaveSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paths")
		FString RelativeConfigPath = "Content/Config.json";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelData")
		TSubclassOf<ASVRFlagSpawner> FlagSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelData")
		TSubclassOf<ASVRFlag> FlagClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		TSubclassOf<ASVRDesktopCharacter> DesktopCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		TSubclassOf<ASVRMixRealityCharacter> OculusCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		bool AutoRespawn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		EClientLaunchType ClientLaunchType = EClientLaunchType::Desktop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		EClientControllerType ClientControllerType = EClientControllerType::Keyboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		float PlayerMaxSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		ETeamID TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (ClampMin = "30", ClampMax = "300", UIMin = "30", UIMax = "300"))
		int32 RoundTime = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (ClampMin = "1", ClampMax = "10", UIMin = "1", UIMax = "10"))
		int32 FragCost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
		int32 MaxPlayers = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		FString StartupMapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TArray<FString> MapsNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TMap<ETeamID, FTeamData> TeamData;

	/////////////////////////////////////////////////////////////////////////
	// Network

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
		FNetworkConfig NetworkConfig;

	UFUNCTION(BlueprintCallable, Category = "Network")
		FString ChangeToNextMap();

	UFUNCTION(BlueprintCallable, Category = "Network")
		FString GetCurrentMapName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
		TArray<FString> ServerConsoleCommandsOnStartup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
		TArray<FString> ClientConsoleCommandsOnStartup;

	FTeamData GetTeamDataByID(ETeamID ID);

	virtual void LoadConfig();
	virtual void SaveConfig();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
#endif	

private:		
	int32 CurrentMapIndex = 0;
};
