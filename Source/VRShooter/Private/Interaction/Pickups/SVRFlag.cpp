// The Family

#include "SVRFlag.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "./Player/Character/SVRBaseCharacter.h"
#include "../Components/SVRPickupComponent.h"
#include "../Online/SVRGameMode.h"

ASVRFlag::ASVRFlag()
{
	PhysCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PhysCapsule"));
	PhysCapsule->SetCapsuleHalfHeight(75.0f);
	PhysCapsule->SetCapsuleRadius(7.0f);
	PhysCapsule->SetCollisionObjectType(ECC_WorldDynamic);
	RootComponent = PhysCapsule;

	InteractCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCapsule"));
	InteractCapsule->SetCapsuleHalfHeight(75.0f);
	InteractCapsule->SetCapsuleRadius(20.0f);
	InteractCapsule->SetupAttachment(PhysCapsule);
	InteractCapsule->SetCollisionObjectType(ECC_GameTraceChannel8);	
	InteractCapsule->SetCollisionProfileName("Interactable");
	InteractCapsule->ShapeColor = FColor(0.0, 0.0, 255.0);

	SetReplicates(true);
	bNetUseOwnerRelevancy = false;
	bReplicateMovement = true;
}

void ASVRFlag::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		EnablePhysics(true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Inventory
void ASVRFlag::OnEquip(USkeletalMeshComponent* Mesh, const FName& AttachPoint)
{
	if (Role == ROLE_Authority)
	{
		EnablePhysics(false);
		AttachMeshToPawn(Mesh, AttachPoint);
		bIsEquipped = true;

		ASVRGameMode* GM = Cast<ASVRGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->FlagPossessed(GetOwner());
		}
	}
}

void ASVRFlag::OnUnEquip()
{
	if (Role == ROLE_Authority)
	{
		ASVRGameMode* GM = Cast<ASVRGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->FlagLost(GetOwner());
		}

		EnablePhysics(true);
		DetachMeshFromPawn();
		bIsEquipped = false;		
	}
}

void ASVRFlag::EnablePhysics(bool Enabled)
{
	if (Role == ROLE_Authority)
	{
		PhysicsEnabled = Enabled;	
		ChangePhycisBehaviour(PhysicsEnabled);
	}
}

void ASVRFlag::ChangePhycisBehaviour_Implementation(bool Enabled)
{
	if (!Enabled)
	{
		InteractCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PhysCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PhysCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
		PhysCapsule->SetSimulatePhysics(false);
	}
	else
	{
		InteractCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PhysCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PhysCapsule->SetCollisionResponseToAllChannels(ECR_Block);
		PhysCapsule->SetSimulatePhysics(true);
	}
}

void ASVRFlag::AttachMeshToPawn(USkeletalMeshComponent* Mesh, const FName& AttachPoint)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);

	AttachToComponent(Mesh, AttachmentRules, AttachPoint);
}

void ASVRFlag::DetachMeshFromPawn()
{
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	//SetVisibility(true);//TDO: bad fix
}

bool ASVRFlag::IsEquipped() const
{
	return bIsEquipped;
}

//////////////////////////////////////////////////////////////////////////
// Interactable interface
void ASVRFlag::StartInteract_Implementation(AActor* IInstigator)
{
	if (IInstigator)
	{
		TArray<USVRPickupComponent*> PickupComps;
		IInstigator->GetComponents<USVRPickupComponent>(PickupComps);
		if (PickupComps.Num() > 0)
		{
			PickupComps[0]->EquipFlag(this);
		}
	}
}

void ASVRFlag::StopInteract_Implementation(AActor* IInstigator)
{
	
}

//////////////////////////////////////////////////////////////////////////
// Replication
void ASVRFlag::OnRep_PhysicsEnabled(bool bPhysicsEnabled)
{
	ChangePhycisBehaviour(PhysicsEnabled);
}

void ASVRFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASVRFlag, PhysicsEnabled);
}
