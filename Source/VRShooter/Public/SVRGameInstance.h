// The Family

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "Online.h"
#include "Types/SVROnlineTypes.h"
#include "Assets/SVRConfigAsset.h"
#include "SVRGameInstance.generated.h"

class UNetDriver;

UCLASS()
class VRSHOOTER_API USVRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		USVRConfigAsset* ConfigAsset;

	UFUNCTION(BlueprintCallable, Category = "Network")
		void StartSession();

	UFUNCTION(BlueprintCallable, Category = "Network")
		void JoinOnlineGame();

	UFUNCTION(BlueprintCallable, Category = "Network")
		void FindOnlineGames();

	UFUNCTION(BlueprintCallable, Category = "Network")
		FString ChangeToNextMap();

	UFUNCTION(BlueprintCallable, Category = "Network")
		FString GetCurrentMapName();

	FTeamData GetTeamDataByID(ETeamID ID);

	virtual void Init() override;
	virtual void Shutdown() override;
		
private:
	void OnPreLoadMap(const FString& MapName);
	void OnPostLoadMap();	

	virtual void OnStart() override;

	void OnTravelFailure(UWorld *World, ETravelFailure::Type FailureType, const FString& ReasonString);
	void OnNetworkFailure(UWorld *World, UNetDriver *NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	void ExecuteStartupCommands(const TArray<FString>& ConsoleCommands);
public:
	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	/**
	*	Function to host a game!
	*
	*	@Param		UserID			User that started the request
	*	@Param		SessionName		Name of the Session
	*	@Param		bIsLAN			Is this is LAN Game?
	*	@Param		bIsPresence		"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	        Number of Maximum allowed players on this "Session" (Server)
	*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, const FName& lSessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	/**
	*	Function fired when a session create request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Function fired when a session start request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);


	void SVRFindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);
	/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/**
	*	Delegate fired when a session search query has completed
	*
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);

	bool SVRJoinSession(TSharedPtr<const FUniqueNetId> UserId, const FName& lSessionName, const FOnlineSessionSearchResult& SearchResult);

	/** Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/**
	*	Delegate fired when a session join request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
