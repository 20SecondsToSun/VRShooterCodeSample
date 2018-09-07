// The Family

#include "SVRPickup.h"


// Sets default values
ASVRPickup::ASVRPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	PickupHolder = CreateDefaultSubobject<UChildActorComponent>(TEXT("PickupHolder"));
	PickupHolder->SetupAttachment(SceneComponent);
}

void ASVRPickup::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		ChildActorRef = PickupHolder->GetChildActor();	
		if (ChildActorRef)
		{
			ChildActorRef->SetOwner(this);
			ChildActorClass = PickupHolder->GetChildActorClass();
		}		
	}	
}

void ASVRPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

