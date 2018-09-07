// The Family

#include "SVRWeaponComponent.h"
#include "VRShooter.h"
#include "Net/UnrealNetwork.h"
#include "../Weapon/SVRBaseWeapon.h"

USVRWeaponComponent::USVRWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

	PlayerHandType = EPlayerHandType::Right;
}

void USVRWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		// fix for listener server
		GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &USVRWeaponComponent::SpawnDefaultInventory, 1.0f);
	}	
}

void USVRWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USVRWeaponComponent::SetShootFromEyes(bool Value)
{
	ShootFromEyes = Value;
}

void USVRWeaponComponent::SetPlayerHandType(EPlayerHandType HandType)
{
	PlayerHandType = HandType;
}

/////////////////////////////////////////////////////////////////////////
// Inventory

void USVRWeaponComponent::SpawnDefaultInventory()
{	
	int32 NumWeaponClasses = DefaultInventoryClasses.Num();
	for (int32 i = 0; i < NumWeaponClasses; i++)
	{
		if (DefaultInventoryClasses[i])
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ASVRBaseWeapon* NewWeapon = GetWorld()->SpawnActor<ASVRBaseWeapon>(DefaultInventoryClasses[i], SpawnInfo);
			AddWeapon(NewWeapon);
		}
	}

	// equip first weapon in inventory
	if (Inventory.Num() > 0)
	{
		EquipWeapon(Inventory[CurrentWeaponIndex]);
	}	
}

void USVRWeaponComponent::DestroyInventory()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		// remove all weapons from inventory and destroy them
		for (int32 i = Inventory.Num() - 1; i >= 0; i--)
		{
			ASVRBaseWeapon* Weapon = Inventory[i];
			if (Weapon)
			{
				RemoveWeapon(Weapon);
				Weapon->Destroy();
			}
		}
	}
}

void USVRWeaponComponent::AddWeapon(ASVRBaseWeapon* Weapon)
{
	if (Weapon && (GetOwnerRole() == ROLE_Authority))
	{
		Inventory.AddUnique(Weapon);
	}
}

void USVRWeaponComponent::RemoveWeapon(ASVRBaseWeapon* Weapon)
{
	if (Weapon && (GetOwnerRole() == ROLE_Authority))
	{
		Inventory.RemoveSingle(Weapon);
	}
}

ASVRBaseWeapon* USVRWeaponComponent::FindWeapon(TSubclassOf<ASVRBaseWeapon> WeaponClass)
{
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i] && Inventory[i]->IsA(WeaponClass))
		{
			return Inventory[i];
		}
	}

	return nullptr;
}

void USVRWeaponComponent::EquipWeapon(ASVRBaseWeapon* Weapon)
{
	if (Weapon)
	{
		if (GetOwnerRole() == ROLE_Authority)
		{
			SetCurrentWeapon(Weapon, CurrentWeapon);
		}
		else
		{
			ServerEquipWeapon(Weapon);
		}
	}
}

bool USVRWeaponComponent::ServerEquipWeapon_Validate(ASVRBaseWeapon* Weapon)
{
	return true;
}

void USVRWeaponComponent::ServerEquipWeapon_Implementation(ASVRBaseWeapon* Weapon)
{
	EquipWeapon(Weapon);
}

void USVRWeaponComponent::OnRep_CurrentWeapon(ASVRBaseWeapon* LastWeapon)
{
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
}

void USVRWeaponComponent::SetCurrentWeapon(ASVRBaseWeapon* NewWeapon, ASVRBaseWeapon* LastWeapon)
{
	ASVRBaseWeapon* LocalLastWeapon = nullptr;

	if (LastWeapon != nullptr)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	// unequip previous
	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
	}

	CurrentWeapon = NewWeapon;

	// equip new one
	if (NewWeapon)
	{
		auto Character = Cast<ACharacter>(GetOwner());
		FName AttachPoint = GetWeaponAttachSocketName();
		NewWeapon->SetOwner(GetOwner());
		NewWeapon->OnEquip(Character->GetMesh(), AttachPoint);
	}
}

FName USVRWeaponComponent::GetWeaponAttachSocketName() const
{
	return PlayerHandType == EPlayerHandType::Left ? LeftWeaponAttachSocketName : RightWeaponAttachSocketName;
}

void USVRWeaponComponent::StartFire()
{
	if (CurrentWeapon && CanFire())
	{
		CurrentWeapon->StartFire();
	}
}

void USVRWeaponComponent::StopFire()
{
	if (CurrentWeapon)
	{	
		CurrentWeapon->StopFire();
	}
}

bool USVRWeaponComponent::CanFire()
{
	return IsOwnerAlive;
}

void USVRWeaponComponent::ChangeWeapon()
{
	if (Inventory.Num() > 0)
	{
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % DefaultInventoryClasses.Num();
		EquipWeapon(Inventory[CurrentWeaponIndex]);
	}
}

void USVRWeaponComponent::OwnerDead()
{
	Super::OwnerDead();

	StopFire();

	if (CurrentWeapon)
	{
		CurrentWeapon->OnUnEquip();
		Inventory.RemoveSingle(CurrentWeapon);
		CurrentWeapon = nullptr;
	}
}

void USVRWeaponComponent::OwnerAlive()
{
	Super::OwnerAlive();

	if (CurrentWeapon == nullptr)
	{
		SpawnDefaultInventory();
	}
}

void USVRWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USVRWeaponComponent, Inventory);
	DOREPLIFETIME(USVRWeaponComponent, CurrentWeapon);
}
