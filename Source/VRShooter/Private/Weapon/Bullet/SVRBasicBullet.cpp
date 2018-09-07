// The Family

#include "SVRBasicBullet.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

ASVRBasicBullet::ASVRBasicBullet()
{
 	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetCollisionProfileName("Projectile");	
	SphereCollision->OnComponentHit.AddDynamic(this, &ASVRBasicBullet::OnHit);		
	RootComponent = SphereCollision;

	BulletStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletStaticMesh"));
	BulletStaticMesh->SetupAttachment(SphereCollision);
}

void ASVRBasicBullet::SetIgnoreActors(const TArray<AActor*>& IgnoreAcors)
{
	
}

void ASVRBasicBullet::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		GetWorldTimerManager().SetTimer(BulletLife_TimerHandle, this, &ASVRBasicBullet::onBulletLifeEnd, BulletLife, false);
	}	
}
void ASVRBasicBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role == ROLE_Authority)
	{
		EPhysicalSurface SurfaceType = SurfaceType_Default;
		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		UGameplayStatics::ApplyPointDamage(OtherActor, ActualDamage, FVector::ZeroVector, Hit, nullptr, this, DamageType);

		PlayImpactEffects(SurfaceType, Hit.ImpactPoint);	
		onBulletLifeEnd();
	}
	else
	{
		EPhysicalSurface SurfaceType = SurfaceType_Default;
		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		PlayImpactEffects(SurfaceType, Hit.ImpactPoint);
	}
}

void ASVRBasicBullet::onBulletLifeEnd()
{
	Destroy();
}

void ASVRBasicBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASVRBasicBullet::PlayImpactEffects(EPhysicalSurface SurfaceType, const FVector& ImpactPoint)
{
	UParticleSystem* SelectedEffect = nullptr;

	switch (SurfaceType)
	{
	case SurfaceType2:
		SelectedEffect = FleshImpactEffect;
		break;

	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}

	if (SelectedEffect)
	{			
		FTransform Transformes;
		Transformes.SetLocation(ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Transformes, true);
	}
}

