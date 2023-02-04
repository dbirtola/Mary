// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStats.h"
#include "GameplayTagContainer.h"
#include "MaryPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatsUpdated, const TArray<FPlayerStat>&, Stats);

UCLASS(config=Game)
class AMaryPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AMaryPlayerState();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void OnRep_Stats();

	UPROPERTY(ReplicatedUsing=OnRep_Stats)
	TArray<FPlayerStat> Stats;

	UPROPERTY(BlueprintAssignable)
	FOnStatsUpdated OnStatsUpdated;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddPlayerStat(FGameplayTag StatTag, float Value);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPlayerStat(FGameplayTag StatTag, float Value);

	UFUNCTION(BlueprintPure)
	TArray<FPlayerStat> GetStats() const;

	UFUNCTION(BlueprintPure)
	float GetStat(FGameplayTag StatTag) const;
};

