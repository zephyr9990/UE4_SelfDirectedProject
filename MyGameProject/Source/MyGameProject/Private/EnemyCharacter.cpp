// @Copyright Rodel Aragon 2019


#include "EnemyCharacter.h"
#include "Components/WidgetComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	LockOnArrow = FindComponentByClass<UWidgetComponent>();
	if (LockOnArrow)
	{
		LockOnArrow->SetVisibility(false);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("No Widget Component."));
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::SetLockOnArrowVisibility(bool bValue)
{
	if (LockOnArrow)
	{
		LockOnArrow->SetVisibility(bValue);
	}
}

