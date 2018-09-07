// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SVRPickup.generated.h"

UCLASS()
class VRSHOOTER_API ASVRPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASVRPickup();
	
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Pickup")
		USceneComponent* SceneComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Pickup")
		UChildActorComponent* PickupHolder;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "PickupActor")
		TSubclassOf<AActor> ChildActorClass;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "PickupActor")
		AActor* ChildActorRef;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
