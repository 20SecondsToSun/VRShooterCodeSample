// The Family

#include "SVRTraceWeapon.h"
#include "Net/UnrealNetwork.h"
#include "../Types/SVRPreprocessor.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "./Player/Character/SVRBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

ASVRTraceWeapon::ASVRTraceWeapon()
{
	TracerTargetName = "Target";
}

void ASVRTraceWeapon::CalculateTraceData()
{
	TraceData.Valid = false;

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		ASVRBaseCharacter* BaseCharacter = Cast<ASVRBaseCharacter>(MyOwner);
		FVector ShotDirection = BaseCharacter->GetForwardVector();

		// Bullet Spread
		float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		FVector TraceStart = EyeLocation;

		if (!BaseCharacter->ShootFromEyes)
		{
			UArrowComponent* Arrow = GetForwardArrow();
			TraceStart = Arrow->GetComponentLocation();		
			ShotDirection = FMath::VRandCone(Arrow->GetForwardVector(), HalfRad, HalfRad);
		}

		FVector TraceEnd = TraceStart + (ShotDirection * TraceMaxDistance);

		TraceData.TraceEnd = TraceEnd;
		TraceData.TraceStart = TraceStart;
		TraceData.ShotDirection = ShotDirection;
		TraceData.Valid = true;
	}
}

void ASVRTraceWeapon::OnRep_HitScanTrace()
{
	PlayTracerEffect(HitScanTrace.TraceTo);
}

bool ASVRTraceWeapon::MakeSingleTrace_Validate(const FHitScanTraceData& SingleTraceData)
{
	return true;
}

void ASVRTraceWeapon::MakeSingleTrace_Implementation(const FHitScanTraceData& SingleTraceData)
{
	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		FVector TracerEndPoint = SingleTraceData.TraceEnd;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FHitResult Hit;

		if (GetWorld()->LineTraceSingleByChannel(Hit, SingleTraceData.TraceStart, SingleTraceData.TraceEnd, COLLISION_WEAPON, QueryParams))
		{
			MakeDamage(Hit, TraceData.ShotDirection);
			TracerEndPoint = Hit.ImpactPoint;
		}

		PlayTracerEffectMulti(TracerEndPoint);
	}
}

void ASVRTraceWeapon::PlayTracerEffectMulti_Implementation(const FVector& TracerEndPoint)
{
	PlayTracerEffect(TracerEndPoint);
}

void ASVRTraceWeapon::PlayTracerEffect(const FVector& ImpactPoint)
{
	if (TracerEffect)
	{
		USkeletalMeshComponent* WeaponMesh = GetWeaponMesh();

		FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
		FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation, MuzzleRotation, true);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, ImpactPoint);
		}
	}
}

void ASVRTraceWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASVRTraceWeapon, HitScanTrace);
}
