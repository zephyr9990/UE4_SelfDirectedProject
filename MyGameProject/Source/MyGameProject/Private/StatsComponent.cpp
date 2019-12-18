// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsComponent.h"
#include "PlayerCharacter.h"
#include "StatsDataAsset.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}



// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (!StatsDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("No data asset for %s"), *GetOwner()->GetName());
	}
	else 
	{
		Stats.Name = StatsDataAsset->Name;
		Stats.MaxHP = StatsDataAsset->MaxHP;
		Stats.CurrentHP = Stats.MaxHP;
		Stats.MaxMP = StatsDataAsset->MaxMP;
		Stats.CurrentMP = Stats.MaxMP;
		Stats.Strength = StatsDataAsset->Strength;
		Stats.Defense = StatsDataAsset->Defense;
		Stats.Intelligence = StatsDataAsset->Intelligence;
		Stats.MagicDefense = StatsDataAsset->MagicDefense;
	}
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FStatInfo UStatsComponent::GetStats()
{
	return Stats;
}

float UStatsComponent::GetPercentHP()
{
	return Stats.CurrentHP / Stats.MaxHP;
}

float UStatsComponent::GetPercentMP()
{
	return Stats.CurrentMP / Stats.MaxMP;
}

