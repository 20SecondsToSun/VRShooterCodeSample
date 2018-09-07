// The Family

#include "SVRInteractableComponent.h"
#include "Engine.h"
#include "../Types/SVRPreprocessor.h"
#include "./Interaction/InteractableInterface.h"

USVRInteractableComponent::USVRInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USVRInteractableComponent::SearchInteractableBySphereTrace_Implementation(const FVector& StartLocation, const FVector& EndLocation, float SphereRadius, bool DebugDraw)
{
	if (IsOwnerAlive == false || !GetOwner())
	{
		return;
	}

	TArray<FHitResult> HitResults;
	bool bHitSomething = ScanSpaceBySphere(HitResults, StartLocation, EndLocation, SphereRadius, DebugDraw);

	if (bHitSomething)
	{
		TArray<AActor*> InteractableActors;

		for (auto Hit = HitResults.CreateIterator(); Hit; Hit++)
		{
			auto ActorInstance = (*Hit).GetActor();
		
			if (ActorInstance->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
			{
				InteractableActors.Add(ActorInstance);		
			}
		}

		const bool wasInteractable = InteractableActors.Num() != 0;

		if (wasInteractable)
		{
			AActor* NearestActor = InteractableActors[0];
			auto Owner = GetOwner();
			float NearestDistance = FVector::Dist(NearestActor->GetActorLocation(), Owner->GetActorLocation());
			for (size_t i = 1; i < InteractableActors.Num(); i++)
			{
				float CandidateDistance = FVector::Dist(Owner->GetActorLocation(), InteractableActors[i]->GetActorLocation());
				if (NearestDistance > CandidateDistance)
				{
					NearestActor = InteractableActors[i];
					NearestDistance = CandidateDistance;
				}
			}

			if (LastInteractableActor != NearestActor)
			{
				SnuffOutLast();
				LastInteractableActor = NearestActor;
				HighLightMeshes(LastInteractableActor);	
			}		
		}
		else
		{
			SnuffOutLast();
		}
	}
	else
	{
		SnuffOutLast();
	}
}

void USVRInteractableComponent::SnuffOutLast()
{
	if (LastInteractableActor)
	{
		SnuffOutMeshes(LastInteractableActor);
		LastInteractableActor = nullptr;
	}
}

void USVRInteractableComponent::HighLightMeshes_Implementation(AActor* InteractableActor)
{
	if(InteractableActor)
	{
		IInteractableInterface::Execute_HighLightMeshes(InteractableActor);
	}
}

void USVRInteractableComponent::SnuffOutMeshes_Implementation(AActor* InteractableActor)
{
	if (InteractableActor)
	{
		IInteractableInterface::Execute_SnuffOutMeshes(InteractableActor);
	}
}

bool USVRInteractableComponent::SearchInteractableBySphereTrace_Validate(const FVector& StartLocation, const FVector& EndLocation, float SphereRadius, bool DebugDraw)
{
	return true;
}

void USVRInteractableComponent::TryInteractableBySphereTrace_Implementation(const FVector& StartLocation, const FVector& EndLocation, float SphereRadius, bool DebugDraw)
{
	if (IsOwnerAlive == false)
	{
		return;
	}

	TArray<FHitResult> HitResults;
	bool bHitSomething = ScanSpaceBySphere(HitResults, StartLocation, EndLocation, SphereRadius, DebugDraw);

	if (bHitSomething)
	{
		for (auto Hit = HitResults.CreateIterator(); Hit; Hit++)		
		{
			auto ActorInstance = (*Hit).GetActor();		
			if (ActorInstance->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
			{	
				IInteractableInterface::Execute_StartInteract(ActorInstance, GetOwner());
				break;
			}
		}
	}
}

bool USVRInteractableComponent::TryInteractableBySphereTrace_Validate(const FVector& StartLocation, const FVector& EndLocation, float SphereRadius, bool DebugDraw)
{
	return true;
}

bool USVRInteractableComponent::ScanSpaceBySphere(TArray<FHitResult>& HitResults, const FVector& StartLocation, const FVector& EndLocation, float SphereRadius, bool DebugDraw)
{
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(SphereRadius);

	FCollisionQueryParams CollisionQueryParams;
	if (DebugDraw)
	{
		const FName TraceTag("DebugTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionQueryParams.TraceTag = TraceTag;
	}

	return GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::FQuat(), COLLISION_INTERACTABLE, CollisionShape, CollisionQueryParams);
}

void USVRInteractableComponent::OwnerDead()
{
	Super::OwnerDead();
	if (LastInteractableActor)
	{
		SnuffOutMeshes(LastInteractableActor);
		LastInteractableActor = nullptr;
	}
}

void USVRInteractableComponent::OwnerAlive()
{
	Super::OwnerAlive();
}

