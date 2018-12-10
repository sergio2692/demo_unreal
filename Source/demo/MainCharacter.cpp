// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainCharacter::moveForward(float value)
{
	if (value != 0.0f)
	AddMovementInput(GetActorForwardVector(), value);
}
void AMainCharacter::moveRight(float value)
{
	if (value != 0.0f)
	AddMovementInput(GetActorRightVector() , value);
}
// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainCharacter::InitCrouch()
{
	Crouch();
}
void AMainCharacter::StopCrouch()
{
	UnCrouch();
}


// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward",this,&AMainCharacter::moveForward);
	PlayerInputComponent->BindAxis("MoveRight",this, &AMainCharacter::moveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch",EInputEvent::IE_Pressed,this,&AMainCharacter::InitCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &AMainCharacter::StopCrouch);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);

} 

FVector AMainCharacter::GetPawnViewLocation() const
{
	if (CameraComponent) {
		return CameraComponent->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

