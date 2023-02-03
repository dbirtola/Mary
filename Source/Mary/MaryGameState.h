// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MaryGameState.generated.h"

UENUM(BlueprintType)
enum EGameSegment
{
	PreGame, // Pre game is when we reset the map and move players into their new starting position. 
	Play,	// Play is the actual game
	PostGame, // Post game is when we display results and let people run around a bit
	Restarting // While we are doing full level loads to restart, this lets clients know when we are starting a server travel
};

USTRUCT(BlueprintType)
struct FGameTimer
{
	GENERATED_BODY()

public:

	UPROPERTY(NotReplicated)
	FTimerHandle TimerHandle;

	UPROPERTY(BlueprintReadOnly)
	float ServerStartTime = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float InitialDuration = 0.0f;
};

UCLASS(minimalapi)
class AMaryGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMaryGameState();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TEnumAsByte<EGameSegment> CurrentSegment;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FGameTimer ActiveTimer;

	void StartSegment(EGameSegment Segment);

	void EndSegment();

	FGameTimer StartTimer(float Time, FSimpleDelegate& Delegate);

	UFUNCTION(BlueprintCallable)
	float GetRemainingSegmentTime() const;
};



