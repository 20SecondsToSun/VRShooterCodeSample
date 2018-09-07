// The Family

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/SVRBaseCharacter.h"
#include "SVRMixRealityCharacter.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UMotionControllerComponent;

UCLASS()
class VRSHOOTER_API ASVRMixRealityCharacter : public ASVRBaseCharacter
{
	GENERATED_BODY()

public:
	ASVRMixRealityCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMotionControllerComponent* MotionController_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMotionControllerComponent* MotionController_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* MotionControllerMesh_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* MotionControllerMesh_R;

	virtual USkeletalMeshComponent* GetSpecifcPawnMesh(bool WantFirstPerson) const;

protected:
	virtual void BeginPlay() override;
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;	

private:
};
