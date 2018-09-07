// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "./SVRGameInstance.h"
#include "./Assets/SVRConfigAsset.h"

#include "SVRGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

/**
 * 
 */
class ASVRPlayerStart;
class APlayerController;
class ASVRBaseCharacter;

UCLASS(Config = Game)
class VRSHOOTER_API ASVRGameMode : public AGameMode
{
	GENERATED_BODY()

	/** select best spawn point for player */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UFUNCTION(BlueprintCallable, Category = "Game")
		virtual void StartMatch() override;

public:	
	ASVRGameMode(const FObjectInitializer& ObjectInitializer);	

public:
	virtual void PreInitializeComponents() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	ASVRPlayerStart* GetSpawnPointByTeamID(AActor* KilledActor, ETeamID TeamID);
	ASVRPlayerStart* GetSpawnPointByTeamID(ETeamID TeamID);

public:
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual bool ReadyToStartMatch_Implementation() override;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
		FOnActorKilled OnActorKilled;

	USVRConfigAsset* ConfigAsset;	

	void ActorKilled(AActor* VictimActor, AActor* KillerActor, AController* KillerController);

	void FlagPossessed(AActor* OwnerActor);

	void FlagLost(AActor* OwnerActor);

	void ChangeLevel();
	void ForceRespawnHandler(AActor* RespawnActor);

	void ResetClientByType(APlayerController* NewPlayer);

	void CanBeRespawn(AActor* OwnerActor, APlayerStart* PlayerStart);

private:
	int32 RoundTime = 60;
	int32 FragCost = 1;
	int32 PlayerIndex = 0;
	bool AutoRespawn = false;
	ASVRFlagSpawner* FlagSpawner;

	FTimerHandle TimerHandle_GameRoundTimer;
	void GameRoundTimerTickHandler();
	int32 RoundCountDown;
	
	void MakePlayerStartsFree();
	void PlacePlayersInGame();
	void DetermineTheWinner();

	void SpawnFlagSpawner();

	class ASVRBaseCharacter* GetPalyerCharacter(APlayerController* NewPlayer);
	class ASVRPlayerState* GetPlayerState(ASVRBaseCharacter* NewPlayer);
	class ASVRPlayerState* GetPlayerState(AActor* NewPlayer);
	class ASVRPlayerState* GetPlayerState(AController* NewPlayer);
};
