// The Family

#include "SVRPickupComponent.h"
#include "../Pickups/SVRFlag.h"
#include "GameFramework/Character.h"

USVRPickupComponent::USVRPickupComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USVRPickupComponent::EquipFlag(ASVRFlag* Flag)
{
	if (Flag && IsOwnerAlive)
	{
		if (GetOwnerRole() == ROLE_Authority)
		{
			SetCurrentFlag(Flag);
		}
		else
		{
			ServerEquipFlag(Flag);
		}
	}
}

void USVRPickupComponent::SetCurrentFlag(ASVRFlag* Flag)
{
	CurrentFlag = Flag;

	// equip new one
	if (CurrentFlag)
	{
		auto Character = Cast<ACharacter>(GetOwner());
		CurrentFlag->SetOwner(GetOwner());
		CurrentFlag->OnEquip(Character->GetMesh(), FlagAttachSocketName);
	}
}

void USVRPickupComponent::DropPickup()
{
	if (CurrentFlag)
	{
		CurrentFlag->OnUnEquip();
	}
}

bool USVRPickupComponent::ServerEquipFlag_Validate(ASVRFlag* Flag)
{
	return true;
}

void USVRPickupComponent::ServerEquipFlag_Implementation(ASVRFlag* Flag)
{
	EquipFlag(Flag);
}

void USVRPickupComponent::OwnerDead()
{
	Super::OwnerDead();
	DropPickup();
}

void USVRPickupComponent::OwnerAlive()
{
	Super::OwnerAlive();
}

