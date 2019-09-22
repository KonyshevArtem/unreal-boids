// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BoidPawn.h"
#include "BoidUtils.h"
#include "BoidService.generated.h"

/**
 * 
 */
UCLASS()
class UNREALBOIDS_API UBoidService : public UBTService
{
	GENERATED_BODY()

public:
	UBoidService();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector BlueprintKey;
	
private:
	UPROPERTY()
		TArray<FVector> linetracePoints = BoidUtils::GetPointsOnUnitSphere(300);

	/**
	 * Return direction closest to current forward direction of ownerBoid and which has no obstacle.
	 *
	 * Linetrace from actor to each point in linetracePoints and return direction to first with no hit.
	 * Order of points in linetracePoints guaranties that found direction will be closest to current forward direction.
	 *
	 * @param ownerBoid Boid from location of which lines are cast.
	 * @return direction with no obstacle.
	 */
	FVector GetObstacleAvoidDirection(ABoidPawn* ownerBoid) const;

	/**
	 * Get negative directions from each nearby boid to ownerBoid and average them.
	 *
	 * Return zero vector if no nearby boids.
	 *
	 * @param ownerBoid Boid to get nearby boids from.
	 * @return separation direction
	 */
	static FVector GetSeparationDirection(ABoidPawn* ownerBoid);

	/**
	 * Get direction from ownerBoid to center of mass of all nearby boids of ownerBoid.
	 *
	 * Return zero vector if no nearby boids.
	 *
	 * @param ownerBoid Boid to get nearby boids from.
	 * @return direction to center of mass.
	 */
	static FVector GetCohesionDirection(ABoidPawn* ownerBoid);

	/**
	 * Get average velocity of all nearby boids of ownerBoid.
	 *
	 * Return zero vector if no nearby boids.
	 *
	 * @param ownerBoid Boid to get nearby boids from.
	 * @return average direction.
	 */
	static FVector GetAlignmentDirection(ABoidPawn* ownerBoid);

	/**
	 * Get direction from boid to target.
	 *
	 * Return zero vector if no target.
	 *
	 * @param ownerBoid Boid from which direction is calculated.
	 * @param target Actor to which direction is calculated.
	 * @return direction to target.
	 */
	static FVector GetTargetDirection(ABoidPawn* ownerBoid, AActor* target);

	/**
	 * Convert direction to force relative to boid.
	 *
	 * @param ownerBoid Boid relative to which direction is converted.
	 * @param direction Heading direction to convert.
	 * @return force converted from direction.
	 */
	static FVector GetForce(ABoidPawn* ownerBoid, FVector direction);
};
