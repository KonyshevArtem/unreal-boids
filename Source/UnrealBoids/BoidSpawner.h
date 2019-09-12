// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidSpawner.generated.h"

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties")
		TSubclassOf<class ABoidPawn> BoidBlueprintType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid spawner properties")
		int TotalBoids;

private:
	void SpawnBoid() const;
};
