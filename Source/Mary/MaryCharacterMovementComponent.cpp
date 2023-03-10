

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
		// float MoveSpeed = ASC->GetNumericAttribute(UPlayerAttributes::GetSpeedAttribute());
		// MaxWalkSpeed = MoveSpeed;
	}
}

float UMaryCharacterMovementComponent::GetMaxSpeed() const
{
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	return Super::GetMaxSpeed() * (ASC ? ASC->GetNumericAttribute(UPlayerAttributes::GetSpeedAttribute()) : 1.0f);
}

float UMaryCharacterMovementComponent::GetMinAnalogSpeed() const
{
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	return Super::GetMinAnalogSpeed() * (ASC ? ASC->GetNumericAttribute(UPlayerAttributes::GetSpeedAttribute()) : 1.0f);
}
