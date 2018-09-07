// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "./Types/SVROnlineTypes.h"
#include "SVRGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeamStatChanged);

/**
 * 
 */
UCLASS()
class VRSHOOTER_API ASVRGameState : public AGameState
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GameState", ReplicatedUsing = OnRep_TeamStat)
	TArray<FTeamStat> TeamStat;

	/** current weapon rep handler */
	UFUNCTION()
		void OnRep_TeamStat(TArray<FTeamStat> _TeamStat);

	int32 GetIndexByID(ETeamID ID);

public:
	UPROPERTY(BlueprintReadOnly, Transient, Replicated, Category = "GameState")
		int32 NumPlayers; //replicated from GameMode

	UPROPERTY(BlueprintReadOnly, Transient, Replicated, Category = "GameState")
		int32 MaxPlayers; //replicated from GameMode

	UPROPERTY(BlueprintReadOnly, Transient, Replicated, Category = "GameState")
		int32 TimeForGameRoundEnd; 

	UPROPERTY(BlueprintReadOnly, Transient, Replicated, Category = "GameState")
		int32 CurrentRound;

	UPROPERTY(BlueprintAssignable)
		FOnTeamStatChanged OnTeamStatChanged;

	UFUNCTION(BlueprintCallable, Category = "State")
		int32 GetReadyCount() const;

	UFUNCTION(BlueprintCallable, Category = "TeamStat")
		bool HasTeamID(ETeamID ID) const;

	UFUNCTION(BlueprintCallable, Category = "TeamStat")
		FTeamStat GetTeamStatByID(ETeamID ID) const;

	/** Match state has changed */
	UFUNCTION()
		virtual void OnRep_MatchState() override;
	
	void ClearTeams();
	void AddTeamByID(ETeamID ID);

	void AddTeamFragByID(ETeamID ID, int32 FragCost);
	void AddTeamDeathByID(ETeamID ID);

	void SetFlagPossessedTeamID(ETeamID ID);
	void SetFlagLostTeamID(ETeamID ID);

	void SetWinTeam(ETeamID ID);
};
