// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "MaryCollectible.generated.h"

UENUM(BlueprintType)
enum FMaryCollectibleState
{
	Growing = 0,
	Grown = 1,
	Held = 2,
	Dropped = 3,
	Scored // a player has already used this to get points for it
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

	UPROPERTY(VisibleAnywhere, Category = "Collectible", Replicated)
	TEnumAsByte<FMaryCollectibleState> CollectibleState = FMaryCollectibleState::Growing;
	
	UPROPERTY(EditAnywhere, Category = "Collectible", Replicated)
	int PointValue = 1;

	UPROPERTY(EditAnywhere, Category = "Collectible", Replicated)
	int RemainingUses = 0;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TArray<TSubclassOf<UGameplayAbility>> FlowerAbilities;
	
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
	bool TryPickup();

	UFUNCTION(BlueprintCallable)
	bool CanUse() const;
	
	UFUNCTION(BlueprintCallable)
	bool TryUse(UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable)
	bool TryDrop();
	
	void OnDrop();

	UFUNCTION(BlueprintCallable)
	bool BloomFlower();

	UFUNCTION(BlueprintCallable)
	void OnPointsGranted();

	float GetPointValue() const;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
