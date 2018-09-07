// The Family

#include "SVRGameState.h"
#include "../Types/SVRPreprocessor.h"
#include "Engine.h"
#include "SVRPlayerState.h"
#include "./SVRGameInstance.h"
#include "Net/UnrealNetwork.h"

int32 ASVRGameState::GetReadyCount() const 
{
	int32 ReadyCount = 0;

	UE_LOG(VRLog, Warning, TEXT("---------------------------"));
	for (int32 i = 0; i < PlayerArray.Num(); i++) 
	{
		const ASVRPlayerState* PS = Cast<ASVRPlayerState>(PlayerArray[i]);	

		if (PS != nullptr && PS->IsPlayerReady())
		{
			ReadyCount++;
		}
	}

	return ReadyCount;
}

int32  ASVRGameState::GetIndexByID(ETeamID ID)
{
	int32 Index = 0;	
	for (int32 i = 0; i < TeamStat.Num(); i++)
	{
		if (TeamStat[i].ID == ID)
		{
			Index = i;
			break;
		}
	}

	return Index;
}

bool ASVRGameState::HasTeamID(ETeamID ID) const
{
	return TeamStat.FindByPredicate([&](const FTeamStat& elem) { return (elem.ID == ID);}) ? true : false;
}

FTeamStat ASVRGameState::GetTeamStatByID(ETeamID ID) const
{	
	return *TeamStat.FindByPredicate([&](const FTeamStat& elem) { return (elem.ID == ID);});
}

void ASVRGameState::ClearTeams()
{
	TeamStat.Empty();	
}

void ASVRGameState::AddTeamByID(ETeamID ID)
{
	auto SVRGameInstance = Cast<USVRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(SVRGameInstance);
	FTeamData TeamData = SVRGameInstance->GetTeamDataByID(ID);
	FTeamStat Stat(ID, TeamData.Name, TeamData.Color);	
	TeamStat.Add(Stat);
	OnTeamStatChanged.Broadcast();
}

void ASVRGameState::AddTeamFragByID(ETeamID ID, int32 FragCost)
{
	int32 Index = GetIndexByID(ID);
	TeamStat[Index].Frags += FragCost;
	UE_LOG(VRLog, Warning, TEXT(" TeamStat[ID - 1].Frags %i"), TeamStat[Index].Frags);
	OnTeamStatChanged.Broadcast();
}

void ASVRGameState::AddTeamDeathByID(ETeamID ID)
{
	int32 Index = GetIndexByID(ID);
	TeamStat[Index].Deaths++;
	OnTeamStatChanged.Broadcast();
}

void ASVRGameState::SetFlagPossessedTeamID(ETeamID ID)
{
	int32 Index = GetIndexByID(ID);
	TeamStat[Index].HasFlag = true;
	OnTeamStatChanged.Broadcast();
}

void ASVRGameState::SetFlagLostTeamID(ETeamID ID)
{
	int32 Index = GetIndexByID(ID);
	TeamStat[Index].HasFlag = false;
	OnTeamStatChanged.Broadcast();
}

void ASVRGameState::SetWinTeam(ETeamID ID)
{
	int32 Index = GetIndexByID(ID);
	TeamStat[Index].Wins++;
	OnTeamStatChanged.Broadcast();
}

void ASVRGameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();
	UE_LOG(VRLog, Warning, TEXT(" MatchState %s"), *MatchState.ToString());
}

void ASVRGameState::OnRep_TeamStat(TArray<FTeamStat> _TeamStat)
{
	UE_LOG(VRLog, Warning, TEXT(" OnRep_TeamStat"));
	OnTeamStatChanged.Broadcast();
}

void ASVRGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASVRGameState, MaxPlayers, COND_InitialOnly);
	DOREPLIFETIME(ASVRGameState, NumPlayers);
	DOREPLIFETIME(ASVRGameState, TimeForGameRoundEnd);
	DOREPLIFETIME(ASVRGameState, TeamStat);
	DOREPLIFETIME(ASVRGameState, CurrentRound);
}