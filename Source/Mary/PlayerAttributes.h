

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributes.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UPlayerAttributes : public UAttributeSet
{
	GENERATED_BODY()
public:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Speed", ReplicatedUsing = OnRep_Speed)
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UPlayerAttributes, Speed)

	UFUNCTION()
	virtual void OnRep_Speed(const FGameplayAttributeData& OldSpeed);
};