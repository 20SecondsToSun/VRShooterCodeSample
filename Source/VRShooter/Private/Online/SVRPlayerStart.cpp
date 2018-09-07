// The Family

#include "SVRPlayerStart.h"
#include "VRShooter.h"
#include "GameFramework/Character.h"
#include "./Online/SVRGameMode.h"
#include "./Online/SVRPlayerState.h"

bool ASVRPlayerStart::IsBusy() const
{
	return mIsBusy;
}

void ASVRPlayerStart::SetBusy(bool Value)
{
	mIsBusy = Value;
}

void ASVRPlayerStart::TryRespawn(AActor* OtherActor)
{
	ASVRBaseCharacter* BaseCharacter = Cast<ASVRBaseCharacter>(OtherActor);
	
	if (BaseCharacter && BaseCharacter->SpectatorMode && WaitForThisPlayer == BaseCharacter)
	{
		ASVRGameMode* SVRGameMode = GetWorld()->GetAuthGameMode<ASVRGameMode>();
		if (SVRGameMode)
		{
			SVRGameMode->CanBeRespawn(OtherActor, this);
			LitItOff();// todo
		}				
	}
}

void ASVRPlayerStart::LitItAndWait_Implementation(ACharacter* Actor)
{
	WaitForThisPlayer = Actor;	
	OnLitModeChanged.Broadcast(true);
}

void ASVRPlayerStart::LitItOff_Implementation()
{
	WaitForThisPlayer = nullptr;
	OnLitModeChanged.Broadcast(false);
}
