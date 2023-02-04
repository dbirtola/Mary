// Fill out your copyright notice in the Description page of Project Settings.


#include "MaryCollectible.h"
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
	case FMaryCollectibleState::Fallen:
		return true;
	default:
		return false;
	}
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

