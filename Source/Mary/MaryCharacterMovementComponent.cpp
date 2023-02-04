

#include "MaryCharacterMovementComponent.h"

UMaryCharacterMovementComponent::UMaryCharacterMovementComponent()
{

}

void UMaryCharacterMovementComponent::NotifyBumpedPawn(APawn* BumpedPawn)
{
	Super::NotifyBumpedPawn(BumpedPawn);

	OnPawnBumped.Broadcast(BumpedPawn);
}
