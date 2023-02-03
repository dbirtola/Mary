// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStats.h"
#include "GameplayTagContainer.h"
#include "MaryPlayerState.generated.h"


UCLASS(config=Game)
class AMaryPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMaryPlayerState();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(Replicated)
	TArray<FPlayerStat> Stats;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddPlayerStat(FGameplayTag StatTag, float Value);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPlayerStat(FGameplayTag StatTag, float Value);

	UFUNCTION(BlueprintPure)
	TArray<FPlayerStat> GetStats() const;

	UFUNCTION(BlueprintPure)
	float GetStat(FGameplayTag StatTag) const;
};

