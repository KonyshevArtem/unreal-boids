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
	 * Return direction closest to current forward direction and which has no obstacle.
	 *
	 * Linetrace from actor to each point in linetracePoints and return direction to first with no hit.
	 * Order of points in linetracePoints guaranties that found direction will be closest to current forward direction.
	 *
	 * @return direction with no obstacle.
	 */
	FVector GetObstacleAvoidDirection(ABoidPawn* ownerBoid) const;

	/**
	 * Get directions away from each nearby boid and average them.
	 *
	 * Return zero vector if no nearby boids.
	 *
	 * @return separation direction
	 */
	static FVector GetSeparationDirection(ABoidPawn* ownerBoid);

	/*
	 * Get direction to center of mass of all nearby boids.
	 *
	 * Return zero vector if no nearby boids.
	 *
	 * @return direction to center of mass.
	 */
	static FVector GetCohesionDirection(ABoidPawn* ownerBoid);

	/*
	 * Get average direction of all nearby boids.
	 *
	 * Return zero vector if no nearby boids.
	 *
	 * @return average direction.
	 */
	static FVector GetAlignmentDirection(ABoidPawn* ownerBoid);

	/*
	 * Get direction to target.
	 *
	 * Return zero vector if no target.
	 *
	 * @return direction to target.
	 */
	static FVector GetTargetDirection(ABoidPawn* ownerBoid);

	/**
	 * Convert direction to force.
	 *
	 * @param direction Heading direction to convert.
	 * @return force converted from direction.
	 */
	static FVector GetForce(ABoidPawn* ownerBoid, FVector direction);
};
