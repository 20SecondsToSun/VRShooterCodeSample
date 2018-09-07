// The Family

#include "SVRGameMode.h"
#include "../Types/SVRPreprocessor.h"
#include "SVRGameSession.h"
#include "SVRGameState.h"
#include "SVRPlayerState.h"
#include "SVRPlayerStart.h"
#include "./UI/SVRHUD.h"
#include "./Player/Controller/SVRBasePlayerController.h"
#include "./Player/Controller/SVRDesktopPlayerController.h"
#include "./Player/Character/SVRBaseCharacter.h"
#include "./Gameplay/SVRFlagSpawner.h"
#include "./SVRGameInstance.h"
#include "Kismet/GameplayStatics.h"

ASVRGameMode::ASVRGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	
	DefaultPawnClass = ASVRBaseCharacter::StaticClass();
	PlayerControllerClass = ASVRBasePlayerController::StaticClass();
	PlayerStateClass = ASVRPlayerState::StaticClass();
	GameStateClass = ASVRGameState::StaticClass();
	HUDClass = ASVRHUD::StaticClass();
	GameSessionClass = ASVRGameSession::StaticClass();
	SpectatorClass = ASpectatorPawn::StaticClass();
	ReplaySpectatorPlayerControllerClass = APlayerController::StaticClass();

	bStartPlayersAsSpectators = false;	
}

void ASVRGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto World = GetWorld();
	check(World);

	auto SVRGameInstance = Cast<USVRGameInstance>(UGameplayStatics::GetGameInstance(World));
	check(SVRGameInstance);

	if (SVRGameInstance)
	{
		ConfigAsset = SVRGameInstance->ConfigAsset;
	}
}

void ASVRGameMode::PreInitializeComponents() 
{
	Super::PreInitializeComponents();
	ASVRGameState* const SVRGameState = GetGameState<ASVRGameState>();

	PlayerIndex = 0;

	if (SVRGameState && GameSession)
	{
		auto World = GetWorld();
		check(World);

		auto SVRGameInstance = Cast<USVRGameInstance>(UGameplayStatics::GetGameInstance(World));
		check(SVRGameInstance);

		if (SVRGameInstance)
		{
			SVRGameState->MaxPlayers = GameSession->MaxPlayers = ConfigAsset->MaxPlayers;
		}
		else
		{
			SVRGameState->MaxPlayers = GameSession->MaxPlayers;
		}		
	}	
}

void ASVRGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void ASVRGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	ASVRGameState* const AVRGameState = GetGameState<ASVRGameState>();
	check(AVRGameState);

	AVRGameState->NumPlayers = GetNumPlayers();
	
	ASVRBasePlayerController* CharacterController = Cast<ASVRBasePlayerController>(NewPlayer);
	ASVRPlayerState* PlayerState = Cast<ASVRPlayerState>(NewPlayer->PlayerState);
	CharacterController->LoadLocalData(PlayerState->GetTeamID());
}

void ASVRGameMode::ResetClientByType(APlayerController* NewPlayer)
{
	ASVRBaseCharacter* PlayerCharacter = GetPalyerCharacter(NewPlayer);
	ASVRBasePlayerController* CharacterController = Cast<ASVRBasePlayerController>(NewPlayer);

	if (PlayerCharacter && CharacterController->IsOculusClientType())
	{
		FVector Location = PlayerCharacter->GetActorLocation();
		FRotator Rotation = PlayerCharacter->GetActorRotation();

		CharacterController->UnPossess();
		PlayerCharacter->Destroy();
		FTransform Transform;
		Transform.SetLocation(Location);

		//spawn oculus character
		FActorSpawnParameters SpawnParams;
		ASVRBaseCharacter* NewActor = GetWorld()->SpawnActor<ASVRBaseCharacter>(ConfigAsset->OculusCharacter, Transform, SpawnParams);
		CharacterController->Possess(NewActor);
	}
}

void ASVRGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ASVRGameState* const VRGameState = GetGameState<ASVRGameState>();
	check(VRGameState);

	VRGameState->NumPlayers = GetNumPlayers();
}

void ASVRGameMode::StartPlay()
{
	Super::StartPlay();
	MakePlayerStartsFree();
	SpawnFlagSpawner();

	RoundTime = ConfigAsset->RoundTime;
	FragCost = ConfigAsset->FragCost;
	AutoRespawn = ConfigAsset->AutoRespawn;

	UE_LOG(VRLog, Warning, TEXT("ASVRGameMode::StartPlay()"))
}

void ASVRGameMode::MakePlayerStartsFree()
{
	for (TActorIterator<ASVRPlayerStart> It(GetWorld()); It; ++It)
	{
		It->SetBusy(false);
	}
}

void ASVRGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
}

void ASVRGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	UE_LOG(VRLog, Warning, TEXT("ASVRGameMode::HandleMatchHasStarted()"));

	ASVRGameState* const VRGameState = GetGameState<ASVRGameState>();
	check(VRGameState);
	VRGameState->ClearTeams();

	TArray<AActor*> Characters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASVRBaseCharacter::StaticClass(), Characters);

	for (int i = 0; i < Characters.Num(); ++i)
	{
		ASVRBaseCharacter* PlayerCharacter = Cast<ASVRBaseCharacter>(Characters[i]);
		ASVRBasePlayerController* CharacterController = Cast<ASVRBasePlayerController>(PlayerCharacter->GetController());
		check(CharacterController);
		
		ASVRPlayerState* const SFPS = Cast<ASVRPlayerState>(CharacterController->PlayerState);
		check(SFPS);

		const FPlayerDataStruct PlayerData = SFPS->GetPlayerData();
		ASVRPlayerStart* PlayerStart = GetSpawnPointByTeamID(PlayerData.ID);
		PlayerCharacter->SetActorLocation(PlayerStart->GetActorLocation());
		PlayerCharacter->SetActorRotation(PlayerStart->GetActorRotation());
		VRGameState->AddTeamByID(PlayerData.ID);

		ResetClientByType(CharacterController);
	}

	RoundCountDown = RoundTime;
	GetWorldTimerManager().SetTimer(TimerHandle_GameRoundTimer, this, &ASVRGameMode::GameRoundTimerTickHandler, 1.0f, true);
	MakePlayerStartsFree();

	VRGameState->CurrentRound = 1;
}

void ASVRGameMode::GameRoundTimerTickHandler()
{
	RoundCountDown--;

	ASVRGameState* const VRGameState = GetGameState<ASVRGameState>();
	check(VRGameState);

	if (RoundCountDown != -1)
	{
		VRGameState->TimeForGameRoundEnd = RoundCountDown;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_GameRoundTimer);

		DetermineTheWinner();
		MakePlayerStartsFree();
		PlacePlayersInGame();	

		RoundCountDown = RoundTime;
		GetWorldTimerManager().SetTimer(TimerHandle_GameRoundTimer, this, &ASVRGameMode::GameRoundTimerTickHandler, 1.0f, true);
		VRGameState->CurrentRound++;
		MakePlayerStartsFree();
		
		FlagSpawner->Spawn();		
	}
}

void ASVRGameMode::DetermineTheWinner()
{
	for (TActorIterator<ASVRBaseCharacter> PlayerCharacterItr(GetWorld()); PlayerCharacterItr; ++PlayerCharacterItr)
	{
		ASVRPlayerState* const SFPS = GetPlayerState(*PlayerCharacterItr);

		if (SFPS->IsFragPossessed())
		{
			auto TeamID = SFPS->GetTeamID();
			ASVRGameState* const VRGameState = GetGameState<ASVRGameState>();
			check(VRGameState);
			VRGameState->SetWinTeam(TeamID);
		}
	}
}

void ASVRGameMode::PlacePlayersInGame()
{
	for (TActorIterator<ASVRBaseCharacter> PlayerCharacterItr(GetWorld()); PlayerCharacterItr; ++PlayerCharacterItr)
	{
		ASVRBaseCharacter* const PlayerCharacter = *PlayerCharacterItr;
		ASVRPlayerState* const SFPS = GetPlayerState(*PlayerCharacterItr);

		const ETeamID TeamID = SFPS->GetTeamID();
		ASVRPlayerStart* PlayerStart = GetSpawnPointByTeamID(TeamID);
		//TODO!
		//PlayerCharacter->FlagDropHandler();			
	}
}

bool ASVRGameMode::ReadyToStartMatch_Implementation() 
{
	ASVRGameState* const VRGameState = GetGameState<ASVRGameState>();
	check(VRGameState);
	check(GameSession);

	UE_LOG(VRLog, Warning, TEXT("ReadyToStartMatch_Implementation::Players num: [%i]  Ready num: [%i] GameSession.MaxPlayers: [%i]"), GetNumPlayers(), VRGameState->GetReadyCount(), GameSession->MaxPlayers);
	
	bool AllReadyToPlay = GetNumPlayers() == GameSession->MaxPlayers && GetNumPlayers() == VRGameState->GetReadyCount();
	return AllReadyToPlay;
}

void ASVRGameMode::StartMatch()
{
	Super::StartMatch();
	UE_LOG(VRLog, Warning, TEXT("StartMatch"));
}

AActor* ASVRGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(VRLog, Warning, TEXT("--------------------ChoosePlayerStart_Implementation--------------------"));
	
	const int32 TEAM_COUNT = 2;
	const int32 TeamID = PlayerIndex % TEAM_COUNT + 1;
	UE_LOG(VRLog, Warning, TEXT("Set TeamID [%i]"), TeamID);

	ASVRBasePlayerController* const CharacterController = Cast<ASVRBasePlayerController>(Player);
	Cast<ASVRPlayerState>(CharacterController->PlayerState)->SetTeamID(ETeamID(TeamID));
	PlayerIndex++;

	return Super::ChoosePlayerStart_Implementation(Player);
}

void ASVRGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);	
}

ASVRPlayerStart* ASVRGameMode::GetSpawnPointByTeamID(ETeamID TeamID)
{
	for (TActorIterator<ASVRPlayerStart> It(GetWorld()); It; ++It)
	{
		if (!It->IsBusy() && It->ID == TeamID)
		{
			It->SetBusy(true);
			return *It;
		}
	}

	return nullptr;
}

ASVRPlayerStart* ASVRGameMode::GetSpawnPointByTeamID(AActor* KilledActor, ETeamID TeamID)
{
	ASVRPlayerStart* PlayerStart = nullptr;
	float Distance = -1;

	for (TActorIterator<ASVRPlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->ID == TeamID)
		{
			if (!It->IsBusy())
			{
				ASVRPlayerStart* CandidateToStart = *It;

				FVector CandidateLocation = CandidateToStart->GetActorLocation();
				FVector2D CandidateLocation2D = FVector2D(CandidateLocation.X, CandidateLocation.Y);

				FVector KilledActorLocation = KilledActor->GetActorLocation();
				FVector2D KilledActorLocation2D = FVector2D(KilledActorLocation.X, KilledActorLocation.Y);

				float DistanceNew = FVector2D::Distance(CandidateLocation2D, KilledActorLocation2D);
				
				if (DistanceNew > Distance)
				{
					Distance = DistanceNew;
					PlayerStart = *It;
					CandidateToStart->SetBusy(true);
				}
			}			
		}	
	}

	if (PlayerStart)
	{
		//@todo: not controlled when two die simultaneously
		//PlayerStart->SetBusy(true);
	}
	
	return PlayerStart;
}

void ASVRGameMode::ActorKilled(AActor* VictimActor, AActor* KillerActor, AController* KillerController)
{
	ASVRGameState* const VRGameState = GetGameState<ASVRGameState>();
	check(VRGameState);

	ASVRBaseCharacter* KilledCharacter = Cast<ASVRBaseCharacter>(VictimActor);
	
	if (KilledCharacter)
	{	
		ASVRPlayerState* PlayerState = GetPlayerState(KilledCharacter);		
		if (PlayerState)
		{
			UE_LOG(VRLog, Warning, TEXT(":::::::::::::::::::: KILLED IN TEAM ::::::::::::::::::::::[%i]"), (int32)PlayerState->GetTeamID());
			ETeamID TeamID = PlayerState->GetTeamID();
				
			VRGameState->AddTeamDeathByID(TeamID);

			ASVRPlayerStart* PlayerStart = GetSpawnPointByTeamID(KilledCharacter, TeamID);
				
			if (AutoRespawn)
			{
				KilledCharacter->SetActorLocation(PlayerStart->GetActorLocation());
			}
			else
			{
				KilledCharacter->ChangeSpectatorMode(true);
				PlayerStart->LitItAndWait(KilledCharacter);
			}

			KilledCharacter->Heal();					
		}
	}

	if (KillerController)
	{
		ASVRPlayerState* PlayerState = GetPlayerState(KillerController);

		if (PlayerState)
		{
			VRGameState->AddTeamFragByID(PlayerState->GetTeamID(), FragCost);
			PlayerState->AddFrag(FragCost);
		}
	}	
	
	MakePlayerStartsFree();
}

/////////////////////////////////////////////////////////////////////////
// Flag

void ASVRGameMode::SpawnFlagSpawner()
{
	FActorSpawnParameters SpawnInfo;
	FTransform Transform;
	FlagSpawner = GetWorld()->SpawnActorDeferred<ASVRFlagSpawner>(ConfigAsset->FlagSpawner, Transform);
	FlagSpawner->FlagClass = ConfigAsset->FlagClass;
	UGameplayStatics::FinishSpawningActor(FlagSpawner, Transform);
}

void ASVRGameMode::FlagPossessed(AActor* OwnerActor)
{
	ASVRGameState* const VRGameState = GetGameState<ASVRGameState>();	
	ASVRPlayerState* PlayerState = GetPlayerState(OwnerActor);

	if (PlayerState && VRGameState)
	{
		PlayerState->SetFlagPossessed();
		VRGameState->SetFlagPossessedTeamID(PlayerState->GetTeamID());
	}
}

void ASVRGameMode::FlagLost(AActor* OwnerActor)
{
	ASVRGameState* const VRGameState = GetGameState<ASVRGameState>();
	ASVRPlayerState* PlayerState = GetPlayerState(OwnerActor);

	if (PlayerState && VRGameState)
	{
		PlayerState->SetFlagLost();
		VRGameState->SetFlagLostTeamID(PlayerState->GetTeamID());
	}
}

void ASVRGameMode::CanBeRespawn(AActor* OwnerActor, APlayerStart* PlayerStart)
{
	ASVRBaseCharacter* RespawnCharacter = Cast<ASVRBaseCharacter>(OwnerActor);

	if (RespawnCharacter)
	{
		ASVRBasePlayerController* const CharacterController = Cast<ASVRBasePlayerController>(RespawnCharacter->GetController());

		if (CharacterController)
		{
			ASVRPlayerState* PlayerState = Cast<ASVRPlayerState>(CharacterController->PlayerState);
			if (PlayerState)
			{
				ETeamID TeamID = PlayerState->GetTeamID();	
				RespawnCharacter->ChangeSpectatorMode(false);					
				Cast<ASVRPlayerStart>(PlayerStart)->SetBusy(true);
				
				//RespawnCharacter->SetActorLocation(PlayerStart->GetActorLocation());				
				RespawnCharacter->Heal();
			}
		}
	}
}

void ASVRGameMode::ChangeLevel()
{
	auto SVRGameInstance = Cast<USVRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (SVRGameInstance)
	{
		auto MapName = SVRGameInstance->ChangeToNextMap();
		GetWorld()->ServerTravel(MapName);
	}
}

void ASVRGameMode::ForceRespawnHandler(AActor* RespawnActor)
{
	ASVRBaseCharacter* RespawnCharacter = Cast<ASVRBaseCharacter>(RespawnActor);
	ASVRPlayerState* PlayerState = GetPlayerState(RespawnCharacter);

	if (PlayerState)
	{
		ETeamID TeamID = PlayerState->GetTeamID();
		ASVRPlayerStart* PlayerStart = GetSpawnPointByTeamID(RespawnCharacter, TeamID);
		RespawnActor->SetActorLocation(PlayerStart->GetActorLocation());
		PlayerStart->SetBusy(false);
	}
}

ASVRBaseCharacter* ASVRGameMode::GetPalyerCharacter(APlayerController* NewPlayer)
{
	ASVRBasePlayerController* CharacterController = Cast<ASVRBasePlayerController>(NewPlayer);
	check(CharacterController);

	ASVRBaseCharacter* PlayerCharacter = Cast<ASVRBaseCharacter>(CharacterController->GetCharacter());
	//check(PlayerCharacter);

	return PlayerCharacter;
}

ASVRPlayerState* ASVRGameMode::GetPlayerState(ASVRBaseCharacter* NewPlayer)
{
	ASVRBasePlayerController* const CharacterController = Cast<ASVRBasePlayerController>(NewPlayer->GetController());
	check(CharacterController);

	ASVRPlayerState* const SFPS = Cast<ASVRPlayerState>(CharacterController->PlayerState);
	check(SFPS);

	return SFPS;
}

ASVRPlayerState* ASVRGameMode::GetPlayerState(AActor* NewPlayer)
{
	ASVRBaseCharacter* const Character = Cast<ASVRBaseCharacter>(NewPlayer);
	//check(Character);

	return GetPlayerState(Character);
}

ASVRPlayerState* ASVRGameMode::GetPlayerState(AController* NewPlayer)
{
	ASVRBasePlayerController* const CharacterController = Cast<ASVRBasePlayerController>(NewPlayer);
	check(CharacterController);

	ASVRPlayerState* const SFPS = Cast<ASVRPlayerState>(CharacterController->PlayerState);
	check(SFPS);

	return SFPS;
}
