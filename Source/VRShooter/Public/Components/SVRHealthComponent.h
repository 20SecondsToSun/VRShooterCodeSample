// The Family

#pragma once

#include "CoreMinimal.h"
#include "SVRPlayerBaseComponent.h"
#include "SVRHealthComponent.generated.h"

// OnHealthChanged event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, USVRHealthComponent*, OwningHealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRSHOOTER_API USVRHealthComponent : public USVRPlayerBaseComponent
{
	GENERATED_BODY()

	bool IsDead;

public:	
	USVRHealthComponent();

protected:
	virtual void BeginPlay() override;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
		float DefaultHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
		float MaxHealth;	

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "HealthComponent")
		float Health;

	UFUNCTION()
		void OnRep_Health(float OldHealth);

	UFUNCTION()
		void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		void AddHealth(float Value);

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		float GetHealth() const;	

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		bool IsAlive() const;

	void Heal();

	bool IsHealthFull() const;

private:
	UPROPERTY(EditDefaultsOnly)
		bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly)
		float HealUpdateTime = 0.3f;

	UPROPERTY(EditDefaultsOnly)
		float HealWaitTime = 5.0f;

	UPROPERTY(EditDefaultsOnly)
		float HealModifier = 5.0f;

	FTimerHandle HealTimerHandler;
	
	void HealingUpdate();

public:
	UFUNCTION(Reliable, Server, WithValidation, Category = "Health | Debug")
		void MakeDead();

	virtual void OwnerAlive() override;
	virtual void OwnerDead() override;
};
