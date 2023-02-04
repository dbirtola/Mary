

#include "PlayerAttributes.h"
#include "Net/UnrealNetwork.h"

void UPlayerAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributes, Speed, COND_None, REPNOTIFY_Always);
}

void UPlayerAttributes::OnRep_Speed(const FGameplayAttributeData& OldSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributes, Speed, OldSpeed);
}