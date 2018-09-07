// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "./Types/SVROnlineTypes.h"
#include "SVRPlayerState.generated.h"

UCLASS()
class VRSHOOTER_API ASVRPlayerState : public APlayerState
{
	GENERATED_BODY()

private:	
	UPROPERTY(Transient, Replicated)
		int32 FragsKilled = 0;

	UPROPERTY(Transient, Replicated)
		bool IsReady = false;//@todo		

	UPROPERTY(Transient, Replicated)
		bool HasFlag = false;	
	
public:
	UPROPERTY(Transient, BlueprintReadOnly, Replicated)
		FPlayerDataStruct TeamData;

	UFUNCTION(BlueprintCallable, Category = "State")
		bool IsPlayerReady() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
		ETeamID GetTeamID() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
		int32 GetFragsKilled() const;

	UFUNCTION(BlueprintCallable, Category = "State")
		FString GetPlayerName() const;

	UFUNCTION(BlueprintCallable, Category = "State")
		bool GetHasFlag() const;	

	void SetReady(bool IsReady);
	void SetTeamID(ETeamID TeamID);
	void AddFrag(int32 FragCost = 1);

	void SetFlagLost();
	void SetFlagPossessed();
	bool IsFragPossessed() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FPlayerDataStruct GetPlayerData();
};
