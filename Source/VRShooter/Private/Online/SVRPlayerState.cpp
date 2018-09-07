// The Family

#include "SVRPlayerState.h"
#include "VRShooter.h"
#include "SVRGameState.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine.h"
#include "./SVRGameInstance.h"

bool ASVRPlayerState::IsPlayerReady() const
{
	return true;
}

void ASVRPlayerState::SetReady(bool Value)
{
	IsReady = Value;
}

int32 ASVRPlayerState::GetFragsKilled() const
{
	return FragsKilled;
}

void ASVRPlayerState::AddFrag(int32 FragCost)
{
	FragsKilled += FragCost;
}

FString ASVRPlayerState::GetPlayerName() const
{
	return PlayerName;
}

bool ASVRPlayerState::GetHasFlag() const
{
	return HasFlag;
}

void ASVRPlayerState::SetTeamID(ETeamID TeamID)
{
	this->TeamData.ID = TeamID;
}

ETeamID ASVRPlayerState::GetTeamID() const
{
	return TeamData.ID;
}

FPlayerDataStruct ASVRPlayerState::GetPlayerData()
{
	return TeamData;
} 

void ASVRPlayerState::SetFlagLost()
{
	HasFlag = false;
}

void ASVRPlayerState::SetFlagPossessed()
{
	HasFlag = true;
}

bool ASVRPlayerState::IsFragPossessed() const
{
	return HasFlag;
}

void ASVRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASVRPlayerState, FragsKilled);
	DOREPLIFETIME(ASVRPlayerState, IsReady);
	DOREPLIFETIME(ASVRPlayerState, HasFlag);
	DOREPLIFETIME(ASVRPlayerState, TeamData);	
}
