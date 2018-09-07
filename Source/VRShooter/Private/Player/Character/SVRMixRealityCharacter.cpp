// The Family

#include "SVRMixRealityCharacter.h"
#include "VRShooter.h"
#include "../Components/SVRHealthComponent.h"
#include "../Components/SVRWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MotionControllerComponent.h"
#include "./SVRGameInstance.h"

ASVRMixRealityCharacter::ASVRMixRealityCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);

	MotionController_L = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController_L"));
	MotionController_L->SetupAttachment(RootComponent);
	MotionControllerMesh_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MotionControllerMesh_L"));
	MotionControllerMesh_L->SetupAttachment(MotionController_L);

	MotionController_R = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController_R"));
	MotionController_R->SetupAttachment(RootComponent);
	MotionControllerMesh_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MotionControllerMesh_R"));
	MotionControllerMesh_R->SetupAttachment(MotionController_R);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void ASVRMixRealityCharacter::BeginPlay()
{
	Super::BeginPlay();
}

USkeletalMeshComponent* ASVRMixRealityCharacter::GetSpecifcPawnMesh(bool WantFirstPerson) const
{
	return GetMesh();
}

void ASVRMixRealityCharacter::MoveForward(float Value)
{
	AddMovementInput(CameraComp->GetForwardVector() * Value);
}

void ASVRMixRealityCharacter::MoveRight(float Value)
{
	AddMovementInput(CameraComp->GetRightVector() * Value);
}



