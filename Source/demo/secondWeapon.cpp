// Fill out your copyright notice in the Description page of Project Settings.

#include "secondWeapon.h"
#include "engine/World.h"
#include "Projectile.h"


void AsecondWeapon::Fire(UWorld* MyWorld)
{
	AActor* MyOwner = GetOwner();
	if (ProjectileClass)
	{
		FVector MuzzleLocation = Weapon->GetSocketLocation(MuzzleSocketName);

		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		FVector ShotDirection = EyeRotation.Vector();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, ActorSpawnParams);
	}
	
}
