// The Family

#include "SVRGameSession.h"

void ASVRGameSession::InitOptions(const FString& Options)
{
	Super::InitOptions(Options);
	MaxPlayers = MaxPlayersDefault;
}