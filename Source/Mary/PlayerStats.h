// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PlayerStats.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStat
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	float Value;
};