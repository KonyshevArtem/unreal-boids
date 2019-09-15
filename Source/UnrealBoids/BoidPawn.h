// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoidSpawner.h"
#include "Components/SphereComponent.h"
#include "BoidUtils.h"
#include "BoidPawn.generated.h"

UCLASS()
class UNREALBOIDS_API ABoidPawn : public APawn
{
	GENERATED_BODY()

public:
	ABoidPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid properties")
		float TopSpeed = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid properties")
		int LinecastPointsAmount = 300;

	/**
	 * Return current direction of boid.
	 *
	 * @return current velocity.
	 */
	FVector GetCurrentDirection() const;

private:
	float sphereRadius;

	UPROPERTY()
		FVector currentDirection;
	UPROPERTY()
		TSet<ABoidPawn*> nearbyBoids = TSet<ABoidPawn*>();
	UPROPERTY()
		TArray<FVector> linetracePoints = BoidUtils::GetPointsOnUnitSphere(LinecastPointsAmount);

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	/**
	 * Return direction closest to current forward direction and which has no obstacle.
	 *
	 * Linetrace from actor to each point in linetracePoints and return direction to first with no hit.
	 * Order of points in linetracePoints guaranties that found direction will be closest to current forward direction.
	 *
	 * @return direction with no obstacle.
	 */
	FVector GetObstacleAvoidDirection() const;
};
