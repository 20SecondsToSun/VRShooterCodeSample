// The Family

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SVRBasicBullet.generated.h"

class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;
class USoundCue;

UCLASS()
class VRSHOOTER_API ASVRBasicBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ASVRBasicBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet | Default")
		UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet | Default")
		UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet | Default")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet | Default")
		float ActualDamage = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet | Default")
		float BulletLife = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet | Default")
		USoundCue* BulletHitSound;

	void PlayImpactEffects(EPhysicalSurface SurfaceType, const FVector& ImpactPoint);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet | Default")
		USceneComponent* SceneComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet | Default")
		USphereComponent* SphereCollision;

	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet | Default")
		UStaticMeshComponent* BulletStaticMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetIgnoreActors(const TArray<AActor*>& IgnoreAcors);
	
private:
	FTimerHandle BulletLife_TimerHandle;
	
	UFUNCTION()
//		void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void onBulletLifeEnd();


	
	
};
