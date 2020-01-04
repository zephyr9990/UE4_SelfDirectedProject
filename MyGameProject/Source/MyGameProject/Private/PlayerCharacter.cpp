// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "StatsComponent.h"
#include "EnemyCharacter.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Math/UnrealMathVectorCommon.h"



// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LockOnRangeSphere = CreateDefaultSubobject<USphereComponent>(FName("LockOnRange"));
	LockOnRangeSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	LockOnRangeSphere->SetSphereRadius(LockOnSphereRadius);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	CameraBoom->TargetArmLength = DefaultBoomArmLength;

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	Camera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);
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

	PlayerStatsComponent = FindComponentByClass<UStatsComponent>();
	if (!PlayerStatsComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No stats component on player."));
	}

	if (!wPlayerHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("No HUD class on player."));
	}
	else
	{
		PlayerHUD = CreateWidget<UUserWidget>(PlayerController, wPlayerHUD);
		PlayerHUD->AddToViewport();
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> Actors;
	LockOnRangeSphere->GetOverlappingActors(Actors);
	// Make sure closest enemy is within lock-on range. If not, turn off their lock on icon
	if (ClosestEnemy && Actors.Find(ClosestEnemy) == INDEX_NONE)
	{
		ClosestEnemy->SetLockOnArrowVisibility(false);
		ClosestEnemy = nullptr;
	}

	if (!LockOnTarget) 
	{
		// Reset Camera
		if (bLockOnReleased)
		{
			float CurrentTime = DeltaTime;
			FVector CurrentCameraLocation = CameraBoom->GetRelativeLocation();
			FVector CameraBoomDefaultLocation = FMath::Lerp(
				CurrentCameraLocation,
				FVector(0, 0, 50.0f),
				DeltaTime * 5
			);
			CameraBoom->SetRelativeLocation(CameraBoomDefaultLocation);

			float CurrentArmLength = CameraBoom->TargetArmLength;
			float ArmLength = FMath::Lerp(CurrentArmLength, DefaultBoomArmLength, DeltaTime * 5);
			CameraBoom->TargetArmLength = ArmLength;
		}
		// Find the closest enemy for player to lock on to
		for (AActor* Actor : Actors)
		{
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor);
			if (Enemy)
			{
				// Check to see if this is the closest enemy to the player
				FVector DistanceToEnemy = Enemy->GetActorLocation() - GetActorLocation();
				bool bEnemyIsNearestToPlayer = IsNearestTarget(DistanceToEnemy);
				if (bEnemyIsNearestToPlayer)
				{
					// Turn off current closest enemy's lock on widget,
					// and turn on the new closest enemy's lock on widget.
					if (ClosestEnemy)
					{
						ClosestEnemy->SetLockOnArrowVisibility(false);
					}
					ClosestEnemy = Enemy;
					ClosestEnemy->SetLockOnArrowVisibility(true);
				}
			}
		}
	}
	else // maintain camera distance to locked on enemy
	{
		FVector DistanceToLockOnTarget = LockOnTarget->GetActorLocation() - GetActorLocation();
		FVector MidPointBetweenPlayerAndTarget = DistanceToLockOnTarget / 2;

		DistanceToLockOnTarget.Normalize();
		FRotator TowardsLockOnTarget = DistanceToLockOnTarget.Rotation();
		TowardsLockOnTarget.Normalize();
		SetActorRotation(TowardsLockOnTarget);

		FVector LockOnLocation = FMath::Lerp(
			CameraBoom->GetRelativeLocation(),
			TowardsLockOnTarget.Vector().ForwardVector * MidPointBetweenPlayerAndTarget.Size(),
			DeltaTime * 5
		);
		CameraBoom->SetRelativeLocation(LockOnLocation);

		float CurrentArmLength = CameraBoom->TargetArmLength;
		float ArmLength = FMath::Lerp(CurrentArmLength, CurrentBoomArmLength + MidPointBetweenPlayerAndTarget.Size(), DeltaTime * 5);
		CameraBoom->TargetArmLength = ArmLength;
		UE_LOG(LogTemp, Warning, TEXT("%f"), ArmLength);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{ 
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis(FName("Forward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Right"), this, &APlayerCharacter::MoveRight);

	// Camera Rotation
	PlayerInputComponent->BindAxis(FName("Yaw"), this, &APlayerCharacter::RotateYaw);
	PlayerInputComponent->BindAxis(FName("Pitch"), this, &APlayerCharacter::RotatePitch);

	// Actions
	PlayerInputComponent->BindAction(FName("LockOn"), EInputEvent::IE_Pressed, this, &APlayerCharacter::LockOnToNearestTarget);
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

bool APlayerCharacter::IsNearestTarget(FVector DistanceToEnemy)
{
	if (!ClosestEnemy)
	{
		return true;
	}

	FVector DistanceToClosestEnemy = ClosestEnemy->GetActorLocation() - GetActorLocation();
	return DistanceToEnemy.Size() < DistanceToClosestEnemy.Size();
}

void APlayerCharacter::LockOnToNearestTarget()
{
	// If player is already locked on, release the lock on from current target
	if (LockOnTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Releasing lock on from %s!"), *LockOnTarget->GetName());
		LockOnTarget = nullptr;
		bLockOnReleased = true;
		return;
	}

	if (ClosestEnemy)
	{
		LockOnTarget = ClosestEnemy;
		CurrentBoomArmLength = CameraBoom->TargetArmLength;
		UE_LOG(LogTemp, Warning, TEXT("Locking onto %s!"), *ClosestEnemy->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Reorienting camera behind player."))
		CameraBoom->SetRelativeLocation(FVector(0, 0, 50));
		
		/*CameraBoom->bInheritYaw = false;
		CameraBoom->bInheritRoll = false;
		CameraBoom->bInheritPitch = false;

		CameraBoom->SetRelativeRotation(GetActorForwardVector().Rotation());
	
		CameraBoom->bInheritYaw = true;
		CameraBoom->bInheritRoll = true;
		CameraBoom->bInheritPitch = true;*/
	}
}

