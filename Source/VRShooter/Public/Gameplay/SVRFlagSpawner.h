// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SVRFlagSpawner.generated.h"

class ASVRFlag;

UCLASS()
class VRSHOOTER_API ASVRFlagSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ASVRFlagSpawner();

	UPROPERTY(EditAnywhere, Category = Inventory)
		TSubclassOf<ASVRFlag> FlagClass;

	void Spawn();

protected:
	virtual void BeginPlay() override;	

private:
	TArray<AActor*> SpawnPoints;	
	ASVRFlag* CurrentFlag;
};
