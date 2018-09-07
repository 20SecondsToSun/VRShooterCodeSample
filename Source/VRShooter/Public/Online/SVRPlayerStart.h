// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "./SVRGameInstance.h"
#include "SVRPlayerStart.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLitModeChanged, bool, LitMode);

class ACharacter;

UCLASS()
class VRSHOOTER_API ASVRPlayerStart : public APlayerStart
{
	GENERATED_BODY()

	bool mIsBusy = false;
	ACharacter* WaitForThisPlayer;

public:	
	bool IsBusy() const;	

	void SetBusy(bool Value);	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
		ETeamID ID;		

	UFUNCTION(BlueprintCallable, Category = "Respawn")
		void TryRespawn(AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable)
		void LitItAndWait(ACharacter* Actor);

	UFUNCTION(NetMulticast, Reliable)
		void LitItOff();

	UPROPERTY(BlueprintAssignable)
		FOnLitModeChanged OnLitModeChanged;
};
