// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("No player controller on player."));
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Forward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Right"), this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis(FName("Yaw"), this, &APlayerCharacter::RotateYaw);
	PlayerInputComponent->BindAxis(FName("Pitch"), this, &APlayerCharacter::RotatePitch);
}

void APlayerCharacter::MoveForward(float Amount)
{
	if (Amount && PlayerController)
	{
		// Make movement relative to camera
		float ControllerRotationYaw = PlayerController->GetControlRotation().Yaw;
		FRotator NewRotation = FRotator(0.0f, ControllerRotationYaw, 0.0f);
		FVector Forward = NewRotation.Quaternion().GetForwardVector();
		AddMovementInput(Forward, Amount);
	}
}

void APlayerCharacter::MoveRight(float Amount)
{
	if (Amount && PlayerController)
	{
		// Make movement relative to camera
		float ControllerRotationYaw = PlayerController->GetControlRotation().Yaw;
		FRotator NewRotation = FRotator(0.0f, ControllerRotationYaw, 0.0f);
		FVector Right = NewRotation.Quaternion().GetRightVector();
		AddMovementInput(Right, Amount);
	}
}

void APlayerCharacter::RotateYaw(float Amount)
{
	AddControllerYawInput(LookSensitivity * Amount * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::RotatePitch(float Amount)
{
	AddControllerPitchInput(LookSensitivity * Amount * GetWorld()->GetDeltaSeconds());
}

