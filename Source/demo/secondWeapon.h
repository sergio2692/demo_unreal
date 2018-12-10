// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainWeapon.h"
#include "secondWeapon.generated.h"

class UStaticMesh;
class AProjectile;

/**
 * 
 */
UCLASS()
class DEMO_API AsecondWeapon : public AMainWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AActor> ProjectileClass;
public:
	virtual void Fire(UWorld* MyWorld) override;
	
	
};
