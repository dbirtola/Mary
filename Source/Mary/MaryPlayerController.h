// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MaryPlayerController.generated.h"


UCLASS(config=Game)
class AMaryPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMaryPlayerController();

	void AutoManageActiveCameraTarget(AActor* SuggestedTarget) override;
};

