// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidSpawner.h"
#include "Engine/World.h"
#include "BoidPawn.h"

// Sets default values
ABoidSpawner::ABoidSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

