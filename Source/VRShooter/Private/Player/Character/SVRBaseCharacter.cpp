// The Family

#include "SVRBaseCharacter.h"
#include "VRShooter.h"
#include "SVRPlayerState.h"
#include "SVRBasePlayerController.h"
#include "../Weapon/SVRBaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "../Components/SVRHealthComponent.h"
#include "../Components/SVRWeaponComponent.h"
#include "../Components/SVRPickupComponent.h"
#include "../Components/SVRInteractableComponent.h"
#include "../Components/SVRPlayerBaseComponent.h"
#include "../Components/SVRVFXComponent.h"

#include "./Online/SVRGameMode.h"
#include "./SVRGameInstance.h"

ASVRBaseCharacter::ASVRBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;	
	SetReplicates(true);
	SetReplicateMovement(true);

	HealthComp = CreateDefaultSubobject<USVRHealthComponent>(TEXT("HealthComp"));
	PlayerComponents.Add(HealthComp);

	WeaponComp = CreateDefaultSubobject<USVRWeaponComponent>(TEXT("WeaponComp"));
	PlayerComponents.Add(WeaponComp);

	PickupComp = CreateDefaultSubobject<USVRPickupComponent>(TEXT("PickupComp"));
	PlayerComponents.Add(PickupComp);

	InteractableComp = CreateDefaultSubobject<USVRInteractableComponent>(TEXT("InteractableComp"));
	PlayerComponents.Add(InteractableComp);	

	VFXComp = CreateDefaultSubobject<USVRVFXComponent>(TEXT("VFXComp"));
	PlayerComponents.Add(VFXComp);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction Arrow"));
	Arrow->SetupAttachment(RootComponent);
}

void ASVRBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for (int32 iMat = 0; iMat < GetMesh()->GetNumMaterials(); iMat++)
	{
		MeshMIDs.Add(GetMesh()->CreateAndSetMaterialInstanceDynamic(iMat));
	}	
}

void ASVRBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComp->OnHealthChanged.AddDynamic(this, &ASVRBaseCharacter::OnHealthChanged);
	WeaponComp->SetShootFromEyes(ShootFromEyes);
}

void ASVRBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTeamColorsAllMIDs();
}

void ASVRBaseCharacter::PossessedBy(class AController* InController)
{
	Super::PossessedBy(InController);
	ASVRBasePlayerController* BaseController = Cast<ASVRBasePlayerController>(InController);
	if (BaseController)
	{
		MeshColor = BaseController->GetTeamColor();
		UpdateTeamData();
	}
}

void ASVRBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ASVRBaseCharacter::Heal()
{
	HealthComp->Heal();
}


void ASVRBaseCharacter::MakeDead()
{
	HealthComp->MakeDead();
}

void ASVRBaseCharacter::OnHealthChanged(USVRHealthComponent* OwningHealthComp, 
	float Health, 
	float HealthDelta, 
	const class UDamageType* DamageType,
	class AController* InstigatedBy, 
	AActor* DamageCauser)
{
	OnHealthChangedEvent(Health);
}

void ASVRBaseCharacter::OnHealthChangedEvent_Implementation(float Health) 
{
	
}

// Called to bind functionality to input
void ASVRBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASVRBaseCharacter::ForceRespawnHandler_Implementation()
{
	ASVRGameMode* SVRGameMode = GetWorld()->GetAuthGameMode<ASVRGameMode>();
	if (SVRGameMode)
	{
		SVRGameMode->ForceRespawnHandler(this);
	}
}

bool ASVRBaseCharacter::ForceRespawnHandler_Validate()
{
	return true;
}

/////////////////////////////////////////////////////////////////////////
// Movement

FVector ASVRBaseCharacter::GetForwardVector() const
{
	return CameraComp->GetForwardVector();
}

FVector ASVRBaseCharacter::GetRightVector() const
{
	return GetActorRightVector();
}

void ASVRBaseCharacter::MoveForward(float Value)
{	
	AddMovementInput(GetActorForwardVector() * Value);
}

void ASVRBaseCharacter::MoveRight(float Value)
{	
	AddMovementInput(GetRightVector() * Value);
}

void ASVRBaseCharacter::StartFire()
{
	WeaponComp->StartFire();	
}

void ASVRBaseCharacter::StopFire_Implementation()
{
	WeaponComp->StopFire();	
}

void ASVRBaseCharacter::ChangeWeapon()
{
	WeaponComp->ChangeWeapon();
}

USkeletalMeshComponent* ASVRBaseCharacter::GetPawnMesh() const
{
	return GetMesh();
}

FVector ASVRBaseCharacter::GetPawnViewLocation() const
{
	return CameraComp ? CameraComp->GetComponentLocation() : Super::GetPawnViewLocation();
}

bool ASVRBaseCharacter::IsAlive() const
{
	return HealthComp->IsAlive();
}

/////////////////////////////////////////////////////////////////////////
// TeamData

void ASVRBaseCharacter::UpdateTeamData()
{
	UpdateTeamColorsAllMIDs();
}

void ASVRBaseCharacter::UpdateTeamColorsAllMIDs()
{
	for (int32 i = 0; i < MeshMIDs.Num(); ++i)
	{
		UpdateTeamColors(MeshMIDs[i]);
	}
}

void ASVRBaseCharacter::UpdateTeamColors(UMaterialInstanceDynamic* UseMID)
{
	if (UseMID)
	{
		UseMID->SetVectorParameterValue(MeshColorParam, MeshColor);
	}
}

void ASVRBaseCharacter::OnRep_MeshColor()
{	
	UpdateTeamData();
}

void ASVRBaseCharacter::ChangeSpectatorMode(bool SpecMode)
{
	SpectatorMode = SpecMode;

	if (GetMesh())
	{
		GetMesh()->SetVisibility(!SpecMode);
	}

	for (int32 i = 0; i < PlayerComponents.Num(); ++i)
	{
		if (SpecMode)
		{
			PlayerComponents[i]->OwnerDead();
		}
		else
		{
			PlayerComponents[i]->OwnerAlive();
		}		
	}

	OnSpectatorModeChanged.Broadcast(SpecMode);
}

void ASVRBaseCharacter::DropPickup_Implementation()
{
	PickupComp->DropPickup();
}

bool ASVRBaseCharacter::DropPickup_Validate()
{
	return true;
}

void ASVRBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASVRBaseCharacter, MeshColor);
	DOREPLIFETIME(ASVRBaseCharacter, SpectatorMode);	
}

