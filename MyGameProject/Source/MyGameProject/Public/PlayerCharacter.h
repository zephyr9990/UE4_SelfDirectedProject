// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

// Forward Declaration
class UStatsComponent;
class USphereComponent;
class UUserWidget;

UCLASS()
class MYGAMEPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Holds the player's stats
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	UStatsComponent* PlayerStatsComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	USphereComponent* LockOnRangeSphere = nullptr;
private:
	/** Moves the player forward or backward.
	* @param Amount: The amount to move.
	*/
	void MoveForward(float Amount);

	/** Moves the player forward or backward.
	* @param Amount: The amount to move.
	*/
	void MoveRight(float Amount);

	/** Rotates the player camera's yaw.
	* @param Amount: The amount to rotate.
	*/
	void RotateYaw(float Amount);

	/** Rotates the player camera's yaw.
	* @param Amount: The amount to rotate.
	*/
	void RotatePitch(float Amount);

	void LockOnToNearestTarget();

	UPROPERTY(EditAnywhere, Category = "Setup")
	float LookSensitivity = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<UUserWidget> wPlayerHUD = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float LockOnSphereRadius = 2000.0f;

	APlayerController* PlayerController = nullptr;

	UUserWidget* PlayerHUD = nullptr;
};
