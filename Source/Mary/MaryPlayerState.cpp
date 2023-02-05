// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaryPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "PlayerStats.h"
#include "MaryCharacter.h"

AMaryPlayerState::AMaryPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
}

void AMaryPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMaryPlayerState, Stats);
}

UAbilitySystemComponent* AMaryPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMaryPlayerState::OnRep_Stats()
{
	OnStatsUpdated.Broadcast(Stats);
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

	OnStatsUpdated.Broadcast(Stats);
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

	OnStatsUpdated.Broadcast(Stats);
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

void AMaryPlayerState::OnTagNewOrRemoved(const FGameplayTag Tag, int32 Stacks)
{
	if (AMaryCharacter* Character = Cast<AMaryCharacter>(GetPawn()))
	{
		Character->OnTagNewOrRemoved(Tag, Stacks);
	}
}

void AMaryPlayerState::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("Effects.Daze")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AMaryPlayerState::OnTagNewOrRemoved);
}
