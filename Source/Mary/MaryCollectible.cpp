// Fill out your copyright notice in the Description page of Project Settings.


#include "MaryCollectible.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

UStaticMesh* AMaryCollectible::DefaultGrownMesh = nullptr;

// Sets default values
AMaryCollectible::AMaryCollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if(DefaultGrownMesh == nullptr)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh>StaticAsset(TEXT("/Game/Environment/Basic_Flower/prop_flower_sacs.prop_flower_sacs"));
		DefaultGrownMesh = StaticAsset.Object;
	}

	if(GrowingMesh == nullptr)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh>StaticAsset(TEXT("/Game/Environment/Sprout/prop_sprout.prop_sprout"));
		GrowingMesh = StaticAsset.Object;
	}
	
	CollectibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mushroom"));

	CollectibleMesh->SetStaticMesh(GrowingMesh);
}

// Called when the game starts or when spawned
void AMaryCollectible::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMaryCollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMaryCollectible::CanPickup() const
{
	switch (CollectibleState)
	{
	case FMaryCollectibleState::Grown:
	case FMaryCollectibleState::Dropped:
		return true;
	default:
		return false;
	}
}

bool AMaryCollectible::TryPickup()
{
	if(CanPickup())
	{
		CollectibleState = FMaryCollectibleState::Held;
		return true;
	}
	return false;
}

bool AMaryCollectible::CanUse() const
{
	if (CollectibleState == FMaryCollectibleState::Held && RemainingUses > 0)
	{
		return true;
	}
	return false;
}

bool AMaryCollectible::TryUse(UAbilitySystemComponent* AbilitySystemComponent)
{
	if(RemainingUses > 0)
	{
		for(TSubclassOf<UGameplayAbility> const FlowerAbility : FlowerAbilities)
		{
			AbilitySystemComponent->TryActivateAbilityByClass(FlowerAbility, true);
		}
		--RemainingUses;
		return true;
	}
	return false;
}

bool AMaryCollectible::TryDrop()
{
	if(CollectibleState == FMaryCollectibleState::Held)
	{
		CollectibleState = FMaryCollectibleState::Dropped;
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		OnDrop();
		return true;
	}
	return false;
}

void AMaryCollectible::OnDrop()
{
	UWorld* World = GetWorld();

	if(IsValid(World) == false)
	{
		return;
	}

	FVector DropLocation{};
	
	FHitResult OutHit;
	FVector Start = GetActorLocation();
	
	
	if(World->LineTraceSingleByChannel(OutHit, Start, Start - FVector(0, 0, 1000), ECC_WorldStatic))
	{
		DropLocation = OutHit.Location;
	}
	else
	{
		DropLocation = GetActorLocation() - FVector(0,0,225);
	}

	SetActorLocation(DropLocation);
	SetActorRotation(FRotator::ZeroRotator);
}

bool AMaryCollectible::BloomFlower()
{
	switch (CollectibleState)
	{
	case FMaryCollectibleState::Growing:
		if(GrownMesh != nullptr)
		{
			CollectibleMesh->SetStaticMesh(GrownMesh);
		}
		else
		{
			CollectibleMesh->SetStaticMesh(DefaultGrownMesh);
		}
		CollectibleState = FMaryCollectibleState::Grown;
		return true;
	default:
		return false;
	}
	return true;
}

void AMaryCollectible::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMaryCollectible, CollectibleState);
	DOREPLIFETIME(AMaryCollectible, PointValue);
	DOREPLIFETIME(AMaryCollectible, RemainingUses);
}

