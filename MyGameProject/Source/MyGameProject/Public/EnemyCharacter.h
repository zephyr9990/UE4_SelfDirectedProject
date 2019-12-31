// @Copyright Rodel Aragon 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

// Forward Declarations
class UWidgetComponent;

UCLASS()
class MYGAMEPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Sets the visibility of the lock on arrow
	* @param bValue: true = show, false = don't show
	*/
	void SetLockOnArrowVisibility(bool bValue);

private:
	UWidgetComponent* LockOnArrow = nullptr;
};
