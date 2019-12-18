// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

// Forward Declaration
class UStatsDataAsset;

USTRUCT(BlueprintType)
struct FStatInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FString Name;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float MaxHP;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float CurrentHP;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float MaxMP;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float CurrentMP;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 Strength;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 Defense;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 Intelligence;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 MagicDefense;

	FStatInfo()
	{
		Name = "";
		MaxHP = 0.0f;
		CurrentHP = 0.0f;
		MaxMP = 0.0f;
		CurrentMP = 0.0f;
		Strength = 0;
		Defense = 0;
		Intelligence = 0;
		MagicDefense = 0;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAMEPROJECT_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/**
	* Gets the current stats of the player
	* @return A pointer to the current stats of the player
	*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
	FStatInfo GetStats();

	// Used to initialize actor at Lv1 Stats
	UPROPERTY(EditAnywhere, Category = "Stats")
	UStatsDataAsset* StatsDataAsset;

	UFUNCTION(BlueprintCallable, Category = "Stat Percentages")
	float GetPercentHP();

	UFUNCTION(BlueprintCallable, Category = "Stat Percentages")
	float GetPercentMP();

private:
	FStatInfo Stats;
};
