// The Family

#include "SVRDesktopCharacter.h"
#include "VRShooter.h"
#include "../Weapon/SVRBaseWeapon.h"
#include "../Pickups/SVRFlag.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "SVRPlayerState.h"
#include "SVRBasePlayerController.h"
#include "../Components/SVRHealthComponent.h"
#include "../Components/SVRWeaponComponent.h"

#include "./Online/SVRGameMode.h"
#include "./SVRGameInstance.h"

ASVRDesktopCharacter::ASVRDesktopCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderer"));
	TextRenderer->SetupAttachment(RootComponent);
	TextRenderer->SetIsReplicated(true);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeLocation(FVector(0.0, 0.0, 70.0));

	//GetMesh()->SetOwnerNoSee(true);

	/*FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(CameraComp);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->SetCastShadow(false);*/

	SetReplicates(true);
	SetReplicateMovement(true);
}
