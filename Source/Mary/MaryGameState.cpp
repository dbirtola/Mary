// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaryGameState.h"
#include "MaryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"

AMaryGameState::AMaryGameState()
{
}

void AMaryGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMaryGameState, CurrentSegment);
	DOREPLIFETIME(AMaryGameState, ActiveTimer);
}

void AMaryGameState::StartSegment(EGameSegment Segment)
{
	CurrentSegment = Segment;
}

void AMaryGameState::EndSegment()
{

}

FGameTimer AMaryGameState::StartTimer(float Time, FSimpleDelegate& Delegate)
{
	ActiveTimer.ServerStartTime = GetServerWorldTimeSeconds();
	ActiveTimer.InitialDuration = Time;
	GetWorld()->GetTimerManager().SetTimer(ActiveTimer.TimerHandle, Delegate, Time, false);

	return ActiveTimer;
}

float AMaryGameState::GetRemainingSegmentTime() const
{
	const float ElapsedTime = GetServerWorldTimeSeconds() - ActiveTimer.ServerStartTime;
	const float RemainingTime = ActiveTimer.InitialDuration - ElapsedTime;
	return RemainingTime;
}
