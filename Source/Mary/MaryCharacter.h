// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "MaryCollectible.h"
#include "MaryCharacter.generated.h"

class UAbilitySystemComponent;

UENUM()
enum CharacterState
{
	Walking,
	Dashing,
	Stunned
};

UCLASS(config = Game)
class AMaryCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Use Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UseAction;

public:
	AMaryCharacter(const FObjectInitializer& ObjectInitializer);
	
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	UFUNCTION()
	void OnTagNewOrRemoved(const FGameplayTag Tag, int32 Stacks);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
	void ChangeState(CharacterState NewState);
	CharacterState GetCharacterState() const { return CurrentState; }

	UFUNCTION(BlueprintImplementableEvent)
	void AttachFlower(AMaryCollectible* Collectible);

protected:

	UFUNCTION(Server, Reliable)
	void ServerChangeState(CharacterState NewState);
	
	UFUNCTION()
	void OnRep_CurrentState();
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentState, BlueprintReadOnly, Category="State")
	TEnumAsByte<CharacterState> CurrentState;

	FVector ForwardDirection;
	FVector RightDirection;

	FVector2D MovementVector;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<AMaryCollectible*> HeldCollectibles;

	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<AMaryCollectible*> HoveredCollectibles;
	
	static TSubclassOf<UGameplayAbility> DropAbility;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Jump() override;

	void Startwalking();
	void TickWalking(float DeltaSeconds);

	void StartDashing();
	void TickDashing(float DeltaSeconds);

	void StartStunned();
	void TickStunned(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Use();
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();



public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


};

