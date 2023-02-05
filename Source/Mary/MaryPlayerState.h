// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStats.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "MaryPlayerState.generated.h"

class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatsUpdated, const TArray<FPlayerStat>&, Stats);

UCLASS(config=Game)
class AMaryPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()


public:

	AMaryPlayerState();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent;

	UFUNCTION()
	void OnRep_Stats();

	UFUNCTION()
	void PawnSet(APlayerState* Player, APawn* NewPawn, APawn* OldPawn);

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


	UFUNCTION()
	void OnTagNewOrRemoved(const FGameplayTag Tag, int32 Stacks);
protected:
	void BeginPlay() override;

};

