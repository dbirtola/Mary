// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaryPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerStats.h"

AMaryPlayerState::AMaryPlayerState()
{
}

void AMaryPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMaryPlayerState, Stats);
}

void AMaryPlayerState::AddPlayerStat(FGameplayTag StatTag, float Value)
{
	FPlayerStat* Stat = Stats.FindByPredicate([StatTag](const FPlayerStat& ExistingStat) { return ExistingStat.Tag == StatTag; });
	if (Stat)
	{
		Stat->Value += Value;
	}
	else
	{
		Stats.Add(FPlayerStat({ StatTag, Value }));
	}
}

void AMaryPlayerState::SetPlayerStat(FGameplayTag StatTag, float Value)
{
	FPlayerStat* Stat = Stats.FindByPredicate([StatTag](const FPlayerStat& ExistingStat) { return ExistingStat.Tag == StatTag; });
	if (Stat)
	{
		Stat->Value = Value;
	}
	else
	{
		Stats.Add(FPlayerStat({ StatTag, Value }));
	}
}

TArray<FPlayerStat> AMaryPlayerState::GetStats() const
{
	return Stats;
}

float AMaryPlayerState::GetStat(FGameplayTag StatTag) const
{
	const FPlayerStat* Stat = Stats.FindByPredicate([StatTag](const FPlayerStat& ExistingStat) { return ExistingStat.Tag == StatTag; });
	return Stat ? Stat->Value : 0.0f;
}
