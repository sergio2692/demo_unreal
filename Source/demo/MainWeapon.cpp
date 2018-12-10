// Fill out your copyright notice in the Description page of Project Settings.

#include "MainWeapon.h"
#include "Engine/SkeletalMesh.h"
#include "DrawDebugHelpers.h"
#include "math/Color.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
AMainWeapon::AMainWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("weapon"));
	RootComponent = Weapon;
	
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

}

// Called when the game starts or when spawned
void AMainWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainWeapon::Fire(UWorld* MyWorld)
{
	
	AActor* MyOwner = GetOwner();
	if(MyOwner){
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
		FVector ShotDirection = EyeRotation.Vector();
		FVector traceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		FVector TracerEndPoint = traceEnd;

		FHitResult Hit;
		if (MyOwner->GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, traceEnd, ECC_Visibility, QueryParams)) {
			
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);

			if (ImpactEffect)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
		//DrawDebugLine(MyOwner->GetWorld(), EyeLocation, traceEnd, FColor::Red, false, 1.0f, 1111.0f);
		if(MuzzleEffect){
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, Weapon, MuzzleSocketName);
		}
		if(TracerEffect){
			FVector Muzzlelocation = Weapon->GetSocketLocation(MuzzleSocketName);
			UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, Muzzlelocation);
			if (TracerComp)
				TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
		}
	}

}

// Called every frame
void AMainWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

