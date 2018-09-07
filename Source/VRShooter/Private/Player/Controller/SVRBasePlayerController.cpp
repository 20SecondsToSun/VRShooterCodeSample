// The Family

#include "SVRBasePlayerController.h"
#include "SVRBaseCharacter.h"
#include "Engine.h"
#include "./Online/SVRGameMode.h"
#include "VRShooter.h"
#include "./././Online/SVRPlayerState.h"

ASVRBasePlayerController::ASVRBasePlayerController()
{
	SetReplicates(true);
}

void ASVRBasePlayerController::LoadLocalData_Implementation(ETeamID ID)
{
	auto SVRGameInstance = Cast<USVRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FTeamData ConfigTeamData = SVRGameInstance->GetTeamDataByID(ID);
	check(SVRGameInstance);

	SetPlayerData(ConfigTeamData, SVRGameInstance->ConfigAsset->PlayerName, SVRGameInstance->ConfigAsset->ClientLaunchType);
}

void ASVRBasePlayerController::SetPlayerData_Implementation(const FTeamData& PlayerData, const FString& PName, EClientLaunchType LType)
{
	TeamColor = PlayerData.Color;
	ClientLaunchType = LType;

	ASVRBaseCharacter* const lCharacter = Cast<ASVRBaseCharacter>(GetCharacter());
	if (lCharacter)
	{
		lCharacter->MeshColor = TeamColor;
	}

	ASVRPlayerState* const PlState = Cast<ASVRPlayerState>(PlayerState);
	if (PlState)
	{
		PlState->TeamData.TeamName = PlayerData.Name;
		PlState->TeamData.Color = PlayerData.Color;
		PlState->PlayerName = PName;
	}

	ASVRGameMode* SVRGameMode = GetWorld()->GetAuthGameMode<ASVRGameMode>();
	if (SVRGameMode)
	{
		SVRGameMode->ResetClientByType(this);
	}
}

bool ASVRBasePlayerController::SetPlayerData_Validate(const FTeamData& PlayerData, const FString& PName, EClientLaunchType LType)
{
	return true;
}

bool ASVRBasePlayerController::IsOculusClientType() const
{
	return ClientLaunchType == EClientLaunchType::Oculus;
}

EClientLaunchType ASVRBasePlayerController::GetClientLaunchType() const
{
	return ClientLaunchType;
}

FLinearColor ASVRBasePlayerController::GetTeamColor() const
{
	return TeamColor;
}

void ASVRBasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	DefineBindControls();
}

void ASVRBasePlayerController::DefineBindControls()
{
	InputComponent->BindAxis("MoveForward", this, &ASVRBasePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASVRBasePlayerController::MoveRight);

	InputComponent->BindAxis("LookUp", this, &ASVRBasePlayerController::LookUpHandler);
	InputComponent->BindAxis("Turn", this, &ASVRBasePlayerController::TurnHandler);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ASVRBasePlayerController::PlayerStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ASVRBasePlayerController::PlayerStopFire);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ASVRBasePlayerController::Jump);

	InputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &ASVRBasePlayerController::ChangeWeapon);
	InputComponent->BindAction("DropFlag", IE_Pressed, this, &ASVRBasePlayerController::FlagDropHandler);
	InputComponent->BindAction("ChangeLevel", IE_Pressed, this, &ASVRBasePlayerController::ChangeLevelHandler);
	InputComponent->BindAction("Respawn", IE_Pressed, this, &ASVRBasePlayerController::ForceRespawnHandler);

	InputComponent->BindAction("MakeDead", IE_Pressed, this, &ASVRBasePlayerController::MakeDeadHandler);
}

void ASVRBasePlayerController::MakeDeadHandler()
{
	ASVRBaseCharacter* LocalPlayer = Cast<ASVRBaseCharacter>(GetPawn());
	if (LocalPlayer)
	{
		LocalPlayer->MakeDead();
	}
}

void ASVRBasePlayerController::MoveForward(float Value)
{
	if (Value != 0)
	{
		ASVRBaseCharacter* LocalPlayer = Cast<ASVRBaseCharacter>(GetPawn());
		if (LocalPlayer)
		{
			LocalPlayer->MoveForward(Value);
		}
	}	
}

void ASVRBasePlayerController::MoveRight(float Value)
{
	ASVRBaseCharacter* LocalPlayer = Cast<ASVRBaseCharacter>(GetPawn());
	if (LocalPlayer)
	{
		LocalPlayer->MoveRight(Value);
	}
}

void ASVRBasePlayerController::LookUpHandler(float Value)
{
	APawn* LocalPlayer = GetPawn();
	if (LocalPlayer)
	{
		LocalPlayer->AddControllerPitchInput(Value);
	}
}

void ASVRBasePlayerController::TurnHandler(float Value)
{
	APawn* LocalPlayer = GetPawn();
	if (LocalPlayer)
	{
		LocalPlayer->AddControllerYawInput(Value);
	}
}

void ASVRBasePlayerController::PlayerStartFire()
{
	ASVRBaseCharacter* LocalPlayer = Cast<ASVRBaseCharacter>(GetPawn());
	if (LocalPlayer)
	{
		LocalPlayer->StartFire();
	}
}

void ASVRBasePlayerController::PlayerStopFire()
{
	ASVRBaseCharacter* LocalPlayer = Cast<ASVRBaseCharacter>(GetPawn());
	if (LocalPlayer)
	{
		LocalPlayer->StopFire();
	}
}

void ASVRBasePlayerController::ChangeWeapon()
{
	ASVRBaseCharacter* LocalPlayer = Cast<ASVRBaseCharacter>(GetPawn());
	if (LocalPlayer)
	{
		LocalPlayer->ChangeWeapon();
	}
}

void ASVRBasePlayerController::FlagDropHandler()
{
	ASVRBaseCharacter* LocalPlayer = Cast<ASVRBaseCharacter>(GetPawn());
	if (LocalPlayer)
	{
		LocalPlayer->DropPickup();
	}
}

void ASVRBasePlayerController::ChangeLevelHandler()
{
	ASVRGameMode* SVRGameMode = GetWorld()->GetAuthGameMode<ASVRGameMode>();
	if (SVRGameMode)
	{
		SVRGameMode->ChangeLevel();
	}
}

// only for listening server
void ASVRBasePlayerController::ForceRespawnHandler()
{
	ASVRBaseCharacter* LocalPlayer = Cast<ASVRBaseCharacter>(GetPawn());
	if (LocalPlayer)
	{
		LocalPlayer->ForceRespawnHandler();
	}
}

void ASVRBasePlayerController::Jump()
{
	ASVRBaseCharacter* LocalPlayer = Cast<ASVRBaseCharacter>(GetPawn());
	if (LocalPlayer)
	{
		LocalPlayer->Jump();
	}
}

void ASVRBasePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASVRBasePlayerController, TeamColor);
	DOREPLIFETIME(ASVRBasePlayerController, ClientLaunchType);
}

