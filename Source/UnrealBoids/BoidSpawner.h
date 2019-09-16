// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "BoidSpawner.generated.h"

/**
 * Contains all bounds of spawner.
 */
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

	/**
	 * Add all bounds to array and return it.
	 *
	 * @return array with all bounds.
	 */
	TArray<UStaticMeshComponent*> ToArray() const
	{
		TArray<UStaticMeshComponent*> meshes = TArray<UStaticMeshComponent*>();
		if (LeftBound)
			meshes.Add(LeftBound);
		if (RightBound)
			meshes.Add(RightBound);
		if (BackBound)
			meshes.Add(BackBound);
		if (FrontBound)
			meshes.Add(FrontBound);
		if (TopBound)
			meshes.Add(TopBound);
		if (BottomBound)
			meshes.Add(BottomBound);
		return meshes;
	}

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
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties")
		TSubclassOf<class ABoidPawn> BoidBlueprintType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties")
		AActor* BoidTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties")
		int TotalBoids;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties", meta = (ClipMin = "100"))
		float BoidAreaWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties", meta = (ClipMin = "100"))
		float BoidAreaHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties", meta = (ClipMin = "100"))
		float BoidAreaDepth;

private:
	UPROPERTY()
		FSpawnerBounds bounds;

	/// Create boid inside spawner area and add target to it.
	void SpawnBoid() const;

	/**
	 * Get all bounds of spawner and put them in struct.
	 *
	 * @return struct containing all bounds of spawner
	 */
	FSpawnerBounds GetSpawnerBounds() const;

	/**
	 * Update location and scale of all bounds based on exposed properties.
	 *
	 * @param bounds Struct containing all bounds of spawner.
	 */
	void UpdateBoundTransforms(FSpawnerBounds bounds) const;

	/**
	 * Update bound location and scale.
	 *
	 * @param bound Reference to bound that will be updated.
	 * @param location New relative location of bound.
	 * @param scale New relative scale of bound.
	 */
	static void SetBoundTransform(UStaticMeshComponent* bound, FVector location, FVector scale);

	/**
	 * Find main camera and set visibility of all spawner bounds relative to that camera.
	 *
	 * Only inside bounds considered visible.
	 */
	void UpdateBoundVisibility();

	/**
	 * Send linecast from bound to camera and check overlap.
	 *
	 * @param bound Bound from which location linecast is sent.
	 * @param camera Main camera actor to which location linecast is sent.
	 * @return true if linecast hit other bound.
	 */
	bool IsBoundVisible(UStaticMeshComponent* bound, AActor* camera);
};
