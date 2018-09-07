// The Family

#include "SVRGameInstance.h"
#include "Types/SVRPreprocessor.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
//#pragma optimize("", off)

FTeamData USVRGameInstance::GetTeamDataByID(ETeamID ID)
{
	return ConfigAsset->GetTeamDataByID(ID);
}

FString USVRGameInstance::ChangeToNextMap()
{	
	return ConfigAsset->ChangeToNextMap();
}

FString USVRGameInstance::GetCurrentMapName()
{
	return ConfigAsset->GetCurrentMapName();
}

void USVRGameInstance::Init()
{
	Super::Init();

	const bool IsServer = IsRunningDedicatedServer();
	if (ConfigAsset)
	{
		if (IsServer)
		{
			ExecuteStartupCommands(ConfigAsset->ServerConsoleCommandsOnStartup);
		}
		else
		{
			ExecuteStartupCommands(ConfigAsset->ClientConsoleCommandsOnStartup);
		}
	}


	//FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &USVRGameInstance::OnPreLoadMap);
	//FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &USVRGameInstance::OnPostLoadMap);

	/*mHubConnector = NewObject<USFHubConnector>();
	check(mHubConnector);

	mHubConnector->GetOnFailedToConnect().BindUObject(this, &USFGameInstance::OnFailedToConnect);
	mHubConnector->GetOnRecievedCommand().BindUObject(this, &USFGameInstance::OnHubCommandReceived);

	mHubConnector->SetClientState(HubClientState::AwaitingGameSession);
	mHubConnector->ConnectToHub(mHubAddress, mHubPort);

	{
	}

	mTickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &USFGameInstance::Tick), ConnectionRetryDelay);*/

	GEngine->OnTravelFailure().AddUObject(this, &USVRGameInstance::OnTravelFailure);
	GEngine->OnNetworkFailure().AddUObject(this, &USVRGameInstance::OnNetworkFailure);
}

void USVRGameInstance::ExecuteStartupCommands(const TArray<FString>& ConsoleCommands)
{
	auto World = GetWorld();
	if (World)
	{
		for (int i = 0; i < ConsoleCommands.Num(); ++i)
		{
			World->Exec(GetWorld(), *ConsoleCommands[i]);
		}
	}	
}

void USVRGameInstance::OnPreLoadMap(const FString& MapName)
{
	UE_LOG(VRLog, Log, TEXT("OnPreLoadMap "));
}

void USVRGameInstance::OnPostLoadMap()
{
	UE_LOG(VRLog, Log, TEXT("OnPostLoadMap "));
	
	/*if (mHubConnector && IsRunningDedicatedServer() && mAwaitingServerReady) 
	{
		const UWorld* World = GetWorld();
		check(World);

		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		check(SocketSubsystem);

		UIpNetDriver* IPNetDriver = Cast<UIpNetDriver>(World->GetNetDriver());
		check(IPNetDriver);
		check(IPNetDriver->Socket);

		const int32 Port = IPNetDriver->Socket->GetPortNo();

		mHubConnector->SendServerReady(Port);
		mAwaitingServerReady = false;

		UE_LOG(SFLog, Log, TEXT("SERVER READY SENT!"));
	}*/
}

void USVRGameInstance::OnStart()
{
	Super::OnStart();

	UE_LOG(VRLog, Log, TEXT("UVRGameInstance: OnStart "));

	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USVRGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &USVRGameInstance::OnStartOnlineGameComplete);

	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USVRGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USVRGameInstance::OnJoinSessionComplete);
	//StartSession();
}

void USVRGameInstance::StartSession()
{
	// Creating a local player where we can get the UserID from
	ULocalPlayer* const Player = GetFirstGamePlayer();

	// Call our custom HostSession function. GameSessionName is a GameInstance variable
	HostSession(Player->GetPreferredUniqueNetId(),
		ConfigAsset->NetworkConfig.SessionName,
		ConfigAsset->NetworkConfig.IsLANSession,
		ConfigAsset->NetworkConfig.UsesPresence,
		ConfigAsset->NetworkConfig.MaxPublicConnections);
}

bool USVRGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, const FName& lSessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	// Get the Online Subsystem to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
			Fill in all the Session Settings that we want to use.

			There are more with SessionSettings.Set(...);
			For example the Map or the GameMode/Type.
			*/
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, GetCurrentMapName(), EOnlineDataAdvertisementType::ViaOnlineService);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Our delegate should get called when this is complete (doesn't need to be successful!)
			return Sessions->CreateSession(*UserId, lSessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void USVRGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);
			}
		}
	}
}

void USVRGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), ConfigAsset->NetworkConfig.StartupMapName, ConfigAsset->NetworkConfig.IsLevelOpenAbsolute,	ConfigAsset->NetworkConfig.Option);
	}
}

void USVRGameInstance::Shutdown()
{
	Super::Shutdown();
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->DestroySession(ConfigAsset->NetworkConfig.SessionName);
		}
	}
}

void USVRGameInstance::OnTravelFailure(UWorld *World, ETravelFailure::Type FailureType, const FString& ReasonString)
{
	UE_LOG(VRLog, Log, TEXT("OnTravelFailure "));
}

void USVRGameInstance::OnNetworkFailure(UWorld * World, UNetDriver *NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(VRLog, Log, TEXT("OnNetworkFailure "));	
}

//////////////// Find and Join session ////////////////
///////////////////////////////////////////////////////

void USVRGameInstance::FindOnlineGames()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	SVRFindSessions(Player->GetPreferredUniqueNetId(), true, true);
}

void USVRGameInstance::SVRFindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
			Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
			*/
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = ConfigAsset->NetworkConfig.IsLANSession;
			SessionSearch->MaxSearchResults = ConfigAsset->NetworkConfig.MaxSearchSessionResult;
			SessionSearch->PingBucketSize = ConfigAsset->NetworkConfig.PingBucketSize;

			// We only want to set this Query Setting if "bIsPresence" is true
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Set the Delegate to the Delegate Handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}
}

void USVRGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface of the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// Just debugging the Number of Search results. Can be displayed in UMG or something later on
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			// If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
			if (SessionSearch->SearchResults.Num() > 0)
			{
				// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
				// This can be customized later on with your own classes to add more information that can be set and displayed
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
					// This is something you can't do in Blueprint for example!
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
				}
			}
		}
	}

	if (bWasSuccessful)
	{
		JoinOnlineGame();
	}
}

void USVRGameInstance::JoinOnlineGame()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	// Just a SearchResult where we can save the one we want to use, for the case we find more than one!
	FOnlineSessionSearchResult SearchResult;

	// If the Array is not empty, we can go through it
	if (SessionSearch->SearchResults.Num() > 0)
	{
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			// To avoid something crazy, we filter sessions from ourself
			if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId())
			{
				SearchResult = SessionSearch->SearchResults[i];

				// Once we found sounce a Session that is not ours, just join it. Instead of using a for loop, you could
				// use a widget where you click on and have a reference for the GameSession it represents which you can use
				// here
				SVRJoinSession(Player->GetPreferredUniqueNetId(), ConfigAsset->NetworkConfig.SessionName, SearchResult);
				break;
			}
		}
	}
}


bool USVRGameInstance::SVRJoinSession(TSharedPtr<const FUniqueNetId> UserId, const FName& SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	// Return bool
	bool bSuccessful = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}

	return bSuccessful;
}

void USVRGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
			// This is something the Blueprint Node "Join Session" does automatically!
			APlayerController * const PlayerController = GetFirstLocalPlayerController();

			// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
			// String for us by giving him the SessionName and an empty String. We want to do this, because
			// Every OnlineSubsystem uses different TravelURLs
			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				// Finally call the ClienTravel. If you want, you could print the TravelURL to see
				// how it really looks like
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}
