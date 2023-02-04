
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MaryCharacterMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnBumped, APawn*, OtherPawn);

UCLASS()
class UMaryCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UMaryCharacterMovementComponent();

	void NotifyBumpedPawn(APawn* BumpedPawn) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnPawnBumped OnPawnBumped;
};