// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MaryGameMode.generated.h"

class AMaryGameState;

UCLASS(minimalapi)
class AMaryGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AMaryGameState* GetMaryGameState() const;

public:
	AMaryGameMode();
	void StartPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float PreGameDuration;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float GameDuration;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float PostGameDuration;

	void StartPreGame();
	void StartGame();
	void StartPostGame();

	UFUNCTION()
	void OnPreGameFinished();

	UFUNCTION()
	void OnGameFinished();

	UFUNCTION()
	void OnPostGameFinished();

	UFUNCTION(BlueprintCallable)
	void SetPlayerName(APlayerController* Controller, const FString& Name) const;
};



