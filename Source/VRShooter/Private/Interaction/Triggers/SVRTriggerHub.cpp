// The Family

#include "SVRTriggerHub.h"
#include "Engine.h"
#include "SVRTriggerMaster.h"
#include "SVRTriggerSlave.h"
#include "Components/ChildActorComponent.h"
#include "../../Types/SVRPreprocessor.h"

// Sets default values
ASVRTriggerHub::ASVRTriggerHub()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComponent;


	TriggerMasterComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("TriggerMaster"));
	TriggerMasterComponent->SetupAttachment(SceneComponent);

	TriggerSlaveComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("TriggerSlave"));
	TriggerSlaveComponent->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void ASVRTriggerHub::BeginPlay()
{
	Super::BeginPlay();	

	if (Role == ROLE_Authority)
	{

		TriggerMaster = Cast<ASVRTriggerMaster>(TriggerMasterComponent->GetChildActor());
		TriggerSlave = Cast<ASVRTriggerSlave>(TriggerSlaveComponent->GetChildActor());

		if (TriggerMaster && TriggerSlave)
		{
			TriggerMaster->StateChanged.AddDynamic(this, &ASVRTriggerHub::OnMasterStateChanged);
			TriggerSlave->StateChanged.AddDynamic(this, &ASVRTriggerHub::OnSlaveStateChanged);
			TriggerMaster->ProgressChanged.AddDynamic(this, &ASVRTriggerHub::OnProgressChanged);
		}
	}
}

void ASVRTriggerHub::OnProgressChanged(float Percent)
{
	TriggerSlave->SetProgress(Percent);
}

void ASVRTriggerHub::OnMasterStateChanged(ETriggerState State)
{
	TriggerSlave->SetState(State);

	/*if (State == ETriggerState::Active)
	{
		InteractionPercent = 0.0;
		InteractionTimeStart = GetWorld()->TimeSeconds;
		GetWorldTimerManager().SetTimer(InteractionTimer, this, &ASVRTriggerHub::OnInteractionTimerUpdate, 0.1, true);
	}
	else if (State == ETriggerState::Deactive)
	{
		GetWorldTimerManager().ClearTimer(InteractionTimer);
	}*/
}

void ASVRTriggerHub::OnSlaveStateChanged(ETriggerState State)
{

}

void ASVRTriggerHub::OnInteractionTimerUpdate()
{
	InteractionPercent = (GetWorld()->TimeSeconds - InteractionTimeStart) / InteractionTime;
	InteractionPercentUpdate(InteractionPercent);

	if (InteractionPercent >= 1.0f)
	{
		TriggerMaster->SetState(ETriggerState::Deactive);
		TriggerSlave->SetState(ETriggerState::Deactive);
	}
}

void ASVRTriggerHub::InteractionPercentUpdate_Implementation(float Percent)
{

}

void ASVRTriggerHub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

