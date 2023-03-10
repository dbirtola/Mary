
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


	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual float GetMaxSpeed() const override;
	
	virtual float GetMinAnalogSpeed() const override;

};