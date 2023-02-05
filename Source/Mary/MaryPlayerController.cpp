// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaryPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"

AMaryPlayerController::AMaryPlayerController()
{

}

void AMaryPlayerController::AutoManageActiveCameraTarget(AActor* SuggestedTarget)
{
	const auto& OverheadCamera = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
	SetViewTarget(OverheadCamera);
}
