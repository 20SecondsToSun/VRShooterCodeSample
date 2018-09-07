// The Family

#include "SVRTriggerBase.h"
#include "Net/UnrealNetwork.h"

ASVRTriggerBase::ASVRTriggerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
}

void ASVRTriggerBase::SetState_Implementation(ETriggerState Value)
{
	State = Value;
	StateChanged.Broadcast(State);
}

void ASVRTriggerBase::SetProgress(float Value)
{
	if (State == ETriggerState::Active)
	{
		Percent = FMath::Clamp(Value, 0.0f, 1.0f);
		ProgressChanged.Broadcast(Percent);
	}	
}

bool ASVRTriggerBase::IsActive() const
{
	return State == ETriggerState::Active;
}

float ASVRTriggerBase::GetProgress() const
{
	return Percent;
}


void ASVRTriggerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASVRTriggerBase, State);
}

