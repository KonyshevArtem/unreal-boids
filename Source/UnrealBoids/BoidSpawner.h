// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "BoidSpawner.generated.h"

USTRUCT()
struct UNREALBOIDS_API FSpawnerBounds
{
	GENERATED_BODY()

public:
	FSpawnerBounds() = default;
	FSpawnerBounds(UStaticMeshComponent* leftBound, UStaticMeshComponent* rightBound, UStaticMeshComponent* backBound,
		UStaticMeshComponent* frontBound, UStaticMeshComponent* topBound, UStaticMeshComponent* bottomBound)
	{
		LeftBound = leftBound;
		RightBound = rightBound;
		BackBound = backBound;
		FrontBound = frontBound;
		TopBound = topBound;
		BottomBound = bottomBound;
	}
	virtual ~FSpawnerBounds() = default;

	UPROPERTY()
		UStaticMeshComponent* LeftBound;
	UPROPERTY()
		UStaticMeshComponent* RightBound;
	UPROPERTY()
		UStaticMeshComponent* BackBound;
	UPROPERTY()
		UStaticMeshComponent* FrontBound;
	UPROPERTY()
		UStaticMeshComponent* TopBound;
	UPROPERTY()
		UStaticMeshComponent* BottomBound;
};




UCLASS()
class UNREALBOIDS_API ABoidSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoidSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties")
		TSubclassOf<class ABoidPawn> BoidBlueprintType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties")
		int TotalBoids;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties", meta = (ClipMin = "100"))
		float BoidAreaWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties", meta = (ClipMin = "100"))
		float BoidAreaHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties", meta = (ClipMin = "100"))
		float BoidAreaDepth;

private:
	void SpawnBoid() const;
	FSpawnerBounds GetSpawnerBounds() const;
	void UpdateBounds(FSpawnerBounds bounds) const;
	void UpdateBound(UStaticMeshComponent* bound, FVector location, FVector scale) const;
};
