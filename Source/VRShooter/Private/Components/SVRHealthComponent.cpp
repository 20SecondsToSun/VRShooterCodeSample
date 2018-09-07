// The Family

#include "SVRHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "../Types/SVRPreprocessor.h"
#include "../Online/SVRGameMode.h"

USVRHealthComponent::USVRHealthComponent()
{
	DefaultHealth = 100;
	MaxHealth = 100;
	IsDead = false;
	SetIsReplicated(true);
}

// Called when the game starts
void USVRHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &USVRHealthComponent::HandleTakeAnyDamage);
		}
	}

	Health = DefaultHealth;
}

void USVRHealthComponent::Heal()
{
	Health = DefaultHealth;
	IsDead = false;
	OnHealthChanged.Broadcast(this, Health, 0, nullptr, nullptr, nullptr);
}

void USVRHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = Health - OldHealth;

	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

void USVRHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead)
	{
		return;
	}

	//// Update health clamped
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(VRLog, Warning, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	IsDead = Health <= 0.0f;
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);



	if (IsDead)
	{
		if (AutoHeal)
		{
			GetWorld()->GetTimerManager().ClearTimer(HealTimerHandler);
		}

		ASVRGameMode* GM = Cast<ASVRGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
			GM->ActorKilled(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
	else
	{
		if (AutoHeal)
		{
			GetWorld()->GetTimerManager().ClearTimer(HealTimerHandler);
			GetWorld()->GetTimerManager().SetTimer(HealTimerHandler, this, &USVRHealthComponent::HealingUpdate, HealUpdateTime, true, HealWaitTime);
		}
	}
}

void USVRHealthComponent::HealingUpdate()
{
	AddHealth(HealModifier);

	if (IsHealthFull())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandler);
	}
	OnHealthChanged.Broadcast(this, Health, 0.0f, nullptr, nullptr, nullptr);
}

float USVRHealthComponent::GetHealth() const
{
	return Health;
}

bool USVRHealthComponent::IsAlive() const
{
	return Health > 0.0f;
}

void USVRHealthComponent::AddHealth(float Value)
{
	Health += Value;

	if(Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

bool USVRHealthComponent::MakeDead_Validate()
{
	return true;
}

void USVRHealthComponent::MakeDead_Implementation()
{
	Health = 0.0f;
	IsDead = Health <= 0.0f;
	OnHealthChanged.Broadcast(this, Health, 0.0f, nullptr, nullptr, nullptr);

	ASVRGameMode* GM = Cast<ASVRGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->OnActorKilled.Broadcast(GetOwner(), nullptr, nullptr);
		GM->ActorKilled(GetOwner(), nullptr, nullptr);
	}
}

bool USVRHealthComponent::IsHealthFull() const
{
	return MaxHealth == Health;
}

void USVRHealthComponent::OwnerAlive()
{
	Super::OwnerAlive();
}

void USVRHealthComponent::OwnerDead()
{
	Super::OwnerDead();
}

void USVRHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USVRHealthComponent, Health);
}