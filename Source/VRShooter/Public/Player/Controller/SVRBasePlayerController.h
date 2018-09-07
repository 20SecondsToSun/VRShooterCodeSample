// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../../Types/SVROnlineTypes.h"
#include "../../Types/SVRVCharacterTypes.h"
#include "SVRBasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VRSHOOTER_API ASVRBasePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, Replicated)
		EClientLaunchType ClientLaunchType = EClientLaunchType::Desktop;

	UPROPERTY(Transient, BlueprintReadOnly, Replicated)
		FLinearColor TeamColor;

	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void LookUpHandler(float Value);
	virtual void TurnHandler(float Value);	
	virtual void DefineBindControls();	

public:
	ASVRBasePlayerController();

	virtual void ChangeLevelHandler();
	virtual void PlayerStartFire();
	virtual void PlayerStopFire();
	virtual void ChangeWeapon();
	virtual void Jump();

	virtual void FlagDropHandler();
	virtual void ForceRespawnHandler();
	void MakeDeadHandler();

	/** sets up input */
	virtual void SetupInputComponent() override;		

	UFUNCTION(Reliable, Client, Category = "Config")
		void LoadLocalData(ETeamID ID);

	UFUNCTION(Reliable, Server, WithValidation, Category = "Config")
		void SetPlayerData(const FTeamData& PlayerData, const FString& PName, EClientLaunchType LType);

	UFUNCTION(BlueprintCallable, Category = "State")
		FLinearColor GetTeamColor() const;

	UFUNCTION(BlueprintCallable, Category = "State")
		EClientLaunchType GetClientLaunchType() const;

	UFUNCTION(BlueprintCallable, Category = "State")
		bool IsOculusClientType() const;
};
