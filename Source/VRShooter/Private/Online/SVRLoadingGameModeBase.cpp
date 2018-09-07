// The Family

#include "SVRLoadingGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"

void ASVRLoadingGameModeBase::StartPlay()
{
	Super::StartPlay();	
	GetWorld()->GetTimerManager().SetTimer(LoadTimerHandle, this, &ASVRLoadingGameModeBase::LoadTrainLevel, 1.0f, false);
}

void ASVRLoadingGameModeBase::LoadTrainLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Trainlevel");
}