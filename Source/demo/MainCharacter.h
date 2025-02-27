// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class AMainWeapon;
UCLASS()
class DEMO_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* weaponOverlap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void moveForward(float value);
	void moveRight(float value);
	void InitCrouch();
	void StopCrouch();

	void StartFire();
	void StopFire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* currentWeapon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	virtual FVector GetPawnViewLocation() const override;
};
