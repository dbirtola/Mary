// Fill out your copyright notice in the Description page of Project Settings.


#include "MaryCollectible.h"

#include "AbilitySystemComponent.h"
#include "UObject/ConstructorHelpers.h"

UStaticMesh* AMaryCollectible::DefaultGrownMesh = nullptr;

// Sets default values
AMaryCollectible::AMaryCollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if(DefaultGrownMesh == nullptr)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh>StaticAsset(TEXT("/Game/Meshes/SM_Flower_Temp.SM_Flower_Temp"));
		DefaultGrownMesh = StaticAsset.Object;
	}

	if(GrowingMesh == nullptr)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh>StaticAsset(TEXT("/Game/Meshes/SM_Sprout_Temp.SM_Sprout_Temp"));
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
	SetActorLocation(GetActorLocation() - FVector(0,0,100));
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

