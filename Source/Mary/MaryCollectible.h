// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "MaryCollectible.generated.h"

UENUM(BlueprintType)
enum FMaryCollectibleState
{
	Growing = 0,
	Grown = 1,
	HeldReady = 2,
	HeldUsed = 3,
	Fallen = 4	
};

UCLASS()
class MARY_API AMaryCollectible : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaryCollectible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Collectible")
	TEnumAsByte<FMaryCollectibleState> CollectibleState = FMaryCollectibleState::Growing;
	
	UPROPERTY(EditAnywhere, Category = "Collectible")
	int PointValue;

	UPROPERTY(EditAnywhere, Category = "Collectible")
	int RemainingUses = 1;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CollectibleMesh;

	UPROPERTY(EditAnywhere, Category = "Collectible")
	UStaticMesh* GrowingMesh;

	UPROPERTY(EditAnywhere, Category = "Collectible")
	UStaticMesh* GrownMesh;

	static UStaticMesh* DefaultGrownMesh;
	
	UFUNCTION(BlueprintCallable)
	bool CanPickup() const;

	UFUNCTION(BlueprintCallable)
	bool BloomFlower();
};
