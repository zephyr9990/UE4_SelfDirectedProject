// @Copyright Rodel Aragon 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatsDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMEPROJECT_API UStatsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Initial Stats")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Initial Stats")
	float MaxHP;

	UPROPERTY(EditAnywhere, Category = "Initial Stats")
	float MaxMP;

	UPROPERTY(EditAnywhere, Category = "Initial Stats")
	float Strength;

	UPROPERTY(EditAnywhere, Category = "Initial Stats")
	float Defense;

	UPROPERTY(EditAnywhere, Category = "Initial Stats")
	float Intelligence;

	UPROPERTY(EditAnywhere, Category = "Initial Stats")
	float MagicDefense;
};
