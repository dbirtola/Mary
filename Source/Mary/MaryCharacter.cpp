// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaryCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "MaryPlayerState.h"
#include "MaryCharacterMovementComponent.h"
#include "PlayerAttributes.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"


TSubclassOf<UGameplayAbility> AMaryCharacter::DropAbility = NULL;

// AMaryCharacter

AMaryCharacter::AMaryCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMaryCharacterMovementComponent>(TEXT("CharMoveComp")))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	if(IsValid(DropAbility))
	{
		ConstructorHelpers::FObjectFinder<UGameplayAbility>StaticAsset(TEXT("/Game/Gameplay/Abilities/GA_DropFlower.GA_DropFlower"));
		DropAbility = StaticAsset.Object->GetClass();
	}
}

UAbilitySystemComponent* AMaryCharacter::GetAbilitySystemComponent() const
{
	if (AMaryPlayerState* MaryPS = GetPlayerState<AMaryPlayerState>())
	{
		return MaryPS->GetAbilitySystemComponent();
	}
	return nullptr;
}

void AMaryCharacter::OnTagNewOrRemoved(const FGameplayTag Tag, int32 Stacks)
{
	if (Tag == FGameplayTag::RequestGameplayTag("Effects.Daze"))
	{
		if (Stacks > 0)
		{
			GetCharacterMovement()->DisableMovement();
		}

		if (Stacks <= 0)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}
}

void AMaryCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMaryCharacter, CurrentState);
	DOREPLIFETIME(AMaryCharacter, HeldCollectibles);
	DOREPLIFETIME(AMaryCharacter, HoveredCollectibles);
}

void AMaryCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(CurrentState == Walking)
	{
		TickWalking(DeltaSeconds);
	}

	if(CurrentState == Dashing)
	{
		TickDashing(DeltaSeconds);
	}

	if(CurrentState == Stunned)
	{
		TickStunned(DeltaSeconds);
	}
}

void AMaryCharacter::ChangeState(CharacterState NewState)
{
	if(GetLocalRole() != ROLE_AutonomousProxy)
	{
		return;
	}
	
	if(CurrentState == NewState)
	{
		return;
	}
	CurrentState = NewState;
	
	
	ServerChangeState(NewState);
}

void AMaryCharacter::OnRep_CurrentState()
{
	
}

void AMaryCharacter::ServerChangeState_Implementation(CharacterState NewState)
{
	CurrentState = NewState;
}


void AMaryCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	bReplicates = true;

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMaryCharacter::Interact_Implementation()
{
	if(HeldCollectibles.Num() == 0) //No held Item
	{
		for(AMaryCollectible* HoveredCollectible : HoveredCollectibles) 
		{
			if(IsValid(HoveredCollectible) && HoveredCollectible->TryPickup())
			{
				//HoveredCollectible->AttachToActor(this,FAttachmentTransformRules::SnapToTargetNotIncludingScale, "HeadSocket");
				AttachFlower(HoveredCollectible);
				HeldCollectibles.Add(HoveredCollectible);
				break;
			}
		}
	}
	else
	{
		for(AMaryCollectible* HeldCollectible : HeldCollectibles)
		{
			if(IsValid(HeldCollectible))
			{
				HeldCollectible->TryDrop();
				//GetAbilitySystemComponent()->TryActivateAbilityByClass(DropAbility);
				HeldCollectibles.Remove(HeldCollectible);

				if (GetAbilitySystemComponent())
				{
					if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Effects.InFountain"))))
					{

						if (HasAuthority())
						{
							if (AMaryPlayerState* MaryPlayerState = GetPlayerState<AMaryPlayerState>())
							{
								MaryPlayerState->AddPlayerStat(FGameplayTag::RequestGameplayTag(FName("Stat.Points")), 1.0f);
							}
						}
					}
				}

				//bool bUsed = HeldCollectible->TryUse(GetAbilitySystemComponent());
				//if(!bUsed)
				//{
				//
				//}
				break;
			}
		}
	}
}

void AMaryCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMaryCharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMaryCharacter::Move);

		//Interacting
		//EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMaryCharacter::Interact);
	}

}

void AMaryCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator Rotation = GetActorRotation();
		bool attributeSuccess;
		const FRotator YawRotation(
			0,
			Rotation.Yaw + GetAbilitySystemComponent()->GetGameplayAttributeValue(UPlayerAttributes::GetRotationDeltaAttribute(), attributeSuccess),
			0
			);


		// get forward vector
		ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	}
}

void AMaryCharacter::Jump()
{
	//ChangeState(Stunned);
}

void AMaryCharacter::TickWalking(float DeltaSeconds)
{
	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);

	MovementVector = FVector2D::ZeroVector;
}

void AMaryCharacter::TickDashing(float DeltaSeconds)
{
}

void AMaryCharacter::TickStunned(float DeltaSeconds)
{
	
}
