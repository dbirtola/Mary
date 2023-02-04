// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaryGameMode.h"
#include "MaryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MaryGameState.h"
#include "Engine/World.h"

AMaryGameMode::AMaryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	bUseSeamlessTravel = true;
}

AMaryGameState* AMaryGameMode::GetMaryGameState() const
{
	return GetGameState<AMaryGameState>();
}

void AMaryGameMode::StartPlay()
{
	Super::StartPlay();	
	StartPreGame();
}

void AMaryGameMode::StartPreGame()
{
	UE_LOG(LogMary, Log, TEXT("Starting pre game segment"));

	GetMaryGameState()->StartSegment(PreGame);
	FSimpleDelegate PreGameFinished;
	PreGameFinished.BindUObject(this, &AMaryGameMode::OnPreGameFinished);
	GetMaryGameState()->StartTimer(PreGameDuration, PreGameFinished);
}

void AMaryGameMode::OnPreGameFinished()
{
	StartGame();
}

void AMaryGameMode::StartGame()
{
	UE_LOG(LogMary, Log, TEXT("Starting main game segment"));

	GetMaryGameState()->StartSegment(Play);
	FSimpleDelegate PlayFinished;
	PlayFinished.BindUObject(this, &AMaryGameMode::OnGameFinished);
	GetMaryGameState()->StartTimer(GameDuration, PlayFinished);
}

void AMaryGameMode::OnGameFinished()
{
	StartPostGame();
}

void AMaryGameMode::StartPostGame()
{
	UE_LOG(LogMary, Log, TEXT("Starting post game segment"));

	GetMaryGameState()->StartSegment(PostGame);
	FSimpleDelegate PostGameFinished;
	PostGameFinished.BindUObject(this, &AMaryGameMode::OnPostGameFinished);
	GetMaryGameState()->StartTimer(PostGameDuration, PostGameFinished);
}


void AMaryGameMode::OnPostGameFinished()
{
	UE_LOG(LogMary, Log, TEXT("Post game finished. Resetting Level."));
	
	// TODO: Should be able to reset level without losing our game mode using ResetLevel() or some functionality from AGameModeBase. Just doing this now to keep things simple.
	// Also because we do want to make sure we can manually restart these levels in case bugs happen
	GetMaryGameState()->StartSegment(Restarting);
	GetWorld()->ServerTravel("?Restart");
}
