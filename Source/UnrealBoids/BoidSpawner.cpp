// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidSpawner.h"
#include "Engine/World.h"
#include "BoidPawn.h"
#include "Engine/Engine.h"

// Sets default values
ABoidSpawner::ABoidSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABoidSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const FSpawnerBounds bounds = GetSpawnerBounds();
	UpdateBounds(bounds);
}

// Called when the game starts or when spawned
void ABoidSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!BoidBlueprintType) return;

	for (int i = 0; i < TotalBoids; ++i)
	{
		SpawnBoid();
	}
}

void ABoidSpawner::SpawnBoid() const
{
	FActorSpawnParameters parameters;
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FVector direction = FMath::VRand();
	GetWorld()->SpawnActor<ABoidPawn>(BoidBlueprintType.Get(), GetActorLocation(), direction.Rotation(), parameters);
}

/**
 * Get all bounds of spawner and put them in struct.
 *
 * @return struct containing all bounds of spawner
 */
FSpawnerBounds ABoidSpawner::GetSpawnerBounds() const
{
	TArray<UActorComponent*> components = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	TMap<FString, UStaticMeshComponent*> nameToMesh = TMap<FString, UStaticMeshComponent*>();
	for (UActorComponent* component : components)
	{
		UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(component);
		nameToMesh.Add(mesh->GetName(), mesh);
	}
	return FSpawnerBounds(nameToMesh.FindRef("LeftBound"), nameToMesh.FindRef("RightBound"), nameToMesh.FindRef("BackBound"),
		nameToMesh.FindRef("FrontBound"), nameToMesh.FindRef("TopBound"), nameToMesh.FindRef("BottomBound"));
}

/**
 * Update location and scale of all bounds based on exposed properties.
 *
 * @param bounds Struct containing all bounds of spawner.
 */
void ABoidSpawner::UpdateBounds(FSpawnerBounds bounds) const
{
	const FVector yLocation = FVector(0, -BoidAreaWidth / 2, 0);
	const FVector yScale = FVector(BoidAreaDepth / 100, 1, BoidAreaHeight / 100);
	UpdateBound(bounds.LeftBound, yLocation, yScale);
	UpdateBound(bounds.RightBound, -yLocation, yScale);

	const FVector xLocation = FVector(BoidAreaDepth / 2, 0, 0);
	const FVector xScale = FVector(1, BoidAreaWidth / 100, BoidAreaHeight / 100);
	UpdateBound(bounds.FrontBound, xLocation, xScale);
	UpdateBound(bounds.BackBound, -xLocation, xScale);

	const FVector zLocation = FVector(0, 0, BoidAreaHeight / 2);
	const FVector zScale = FVector(BoidAreaDepth / 100, BoidAreaWidth / 100, 1);
	UpdateBound(bounds.TopBound, zLocation, zScale);
	UpdateBound(bounds.BottomBound, -zLocation, zScale);
}

/**
 * Update bound location and scale.
 *
 * @param bound Reference to bound that will be updated.
 * @param location New relative location of bound.
 * @param scale New relative scale of bound.
 */
void ABoidSpawner::UpdateBound(UStaticMeshComponent* bound, FVector location, FVector scale) const
{
	if (!bound) return;
	bound->SetRelativeLocation(location);
	bound->SetRelativeScale3D(scale);
}

