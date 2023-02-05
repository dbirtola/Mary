

#include "MaryCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "PlayerAttributes.h"

UMaryCharacterMovementComponent::UMaryCharacterMovementComponent()
{

}

void UMaryCharacterMovementComponent::NotifyBumpedPawn(APawn* BumpedPawn)
{
	Super::NotifyBumpedPawn(BumpedPawn);

	OnPawnBumped.Broadcast(BumpedPawn);
}

void UMaryCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		float MoveSpeed = ASC->GetNumericAttribute(UPlayerAttributes::GetSpeedAttribute());
		MaxWalkSpeed = MoveSpeed;
	}
}
