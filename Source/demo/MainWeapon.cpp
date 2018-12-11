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
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "TimerManager.h"

// Sets default values
AMainWeapon::AMainWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("weapon"));
	RootComponent = Weapon;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(200.0f);
	CollisionComp->SetupAttachment(Weapon);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMainWeapon::OnComponentBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AMainWeapon::OnComponentEndOverlap);
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

	BulletSpread = 2.0f;
	RateOfFire = 600;
}
void AMainWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 60 / RateOfFire;
}

void AMainWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
	AActor* MyOwner = GetOwner();

	if (Character && !MyOwner) {
		Character->weaponOverlap = this;
	}
}

void AMainWeapon::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
	if (Character) {
		Character->weaponOverlap = nullptr;
	}
}

void AMainWeapon::Fire()
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


void AMainWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &AMainWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}


void AMainWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}
// Called every frame
void AMainWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



