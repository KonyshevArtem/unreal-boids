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
		float MinSpeed = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid properties")
		float MaxSpeed = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid properties")
		int LinecastPointsAmount = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid properties")
		float CohesionWeight = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid properties")
		float SeparationWeight = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid properties")
		float AlignmentWeight = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid properties")
		float CollisionAvoidanceWeight = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid properties")
		float TargetWeight = 1;

	/**
	 * Return current velocity of boid.
	 *
	 * @return current velocity.
	 */
	FVector GetCurrentVelocity() const;

	/*
	 * Set target for boid to go to.
	 *
	 * @param target Target to go to.
	 */
	void SetTarget(AActor* target);

private:
	float sphereRadius;

	UPROPERTY()
		FVector currentVelocity;
	UPROPERTY()
		TSet<ABoidPawn*> nearbyBoids = TSet<ABoidPawn*>();
	UPROPERTY()
		TArray<FVector> linetracePoints = BoidUtils::GetPointsOnUnitSphere(LinecastPointsAmount);
	UPROPERTY()
		AActor* target;

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

	/**
	 * Get directions away from each nearby boid and average them.
	 *
	 * Return zero vector if no nearby boids.
	 *
	 * @return separation direction
	 */
	FVector GetSeparationDirection() const;

	/*
	 * Get direction to center of mass of all nearby boids.
	 *
	 * Return zero vector if no nearby boids.
	 *
	 * @return direction to center of mass.
	 */
	FVector GetCohesionDirection() const;

	/*
	 * Get average direction of all nearby boids.
	 *
	 * Return zero vector if no nearby boids.
	 *
	 * @return average direction.
	 */
	FVector GetAlignmentDirection() const;

	/*
	 * Get direction to target.
	 *
	 * Return zero vector if no target.
	 *
	 * @return direction to target.
	 */
	FVector GetTargetDirection() const;
	
	/**
	 * Convert direction to force.
	 *
	 * @param direction Heading direction to convert.
	 * @return force converted from direction.
	 */
	FVector GetForce(FVector direction) const;
};
