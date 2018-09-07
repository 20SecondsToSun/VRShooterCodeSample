// The Family

#include "SVRTriggerMaster.h"
#include "Components/MeshComponent.h"

ASVRTriggerMaster::ASVRTriggerMaster()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASVRTriggerMaster::BeginPlay()
{
	Super::BeginPlay();	
}

void ASVRTriggerMaster::SetState(ETriggerState Value)
{
	Super::SetState(Value);

	if (State == ETriggerState::Deactive)
	{

	}
}

void ASVRTriggerMaster::StartInteract_Implementation(AActor* IInstigator)
{
	//if (State != ETriggerState::Active)
	{
		SetState(ETriggerState::Active);
	}	
}

void ASVRTriggerMaster::StopInteract_Implementation(AActor* IInstigator)
{
	if (State != ETriggerState::Deactive)
	{
		SetState(ETriggerState::Deactive);
	}
}

void ASVRTriggerMaster::OnHighlightInteractableChanged_Implementation(bool Value)
{
	HighlightStateChanged.Broadcast(Value);
}
