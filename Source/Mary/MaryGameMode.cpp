// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaryGameMode.h"
#include "MaryCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMaryGameMode::AMaryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
