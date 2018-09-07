// The Family

#include "SVRFlagSpawner.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "SVRFlagSpawnPoint.h"
#include "./Interaction/Pickups/SVRFlag.h"

// Sets default values
ASVRFlagSpawner::ASVRFlagSpawner()
{
 
}

// Called when the game starts or when spawned
void ASVRFlagSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role == ROLE_Authority)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASVRFlagSpawnPoint::StaticClass(), SpawnPoints);
		Spawn();
	}
}

void ASVRFlagSpawner::Spawn()
{
	if (CurrentFlag)
	{
		CurrentFlag->Destroy();
	}

	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnInfo;

	if (SpawnPoints.Num())
	{
		auto Index = FMath::RandRange(0, SpawnPoints.Num() - 1);
		Location = SpawnPoints[Index]->GetActorLocation();
	}
	
	CurrentFlag = GetWorld()->SpawnActor<ASVRFlag>(FlagClass, Location, Rotation, SpawnInfo);
}
