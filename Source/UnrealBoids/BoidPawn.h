// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoidSpawner.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "BoidPawn.generated.h"

UCLASS()
class UNREALBOIDS_API ABoidPawn : public APawn
{
	GENERATED_BODY()

public:
	ABoidPawn();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid properties")
		float MinSpeed = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid properties")
		float MaxSpeed = 10;

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

	/*
	 * Set target for boid to go to.
	 *
	 * @param target Target to go to.
	 */
	void SetTarget(AActor* target);

	/**
	 * Return boid's current target.
	 *
	 * @return reference to target actor.
	 */
	AActor* GetTarget() const;

	/*
	 * Return radius of sphere that is used for detecting other boids.
	 *
	 * @return radius of sphere.
	 */
	float GetSphereRadius() const;

	/*
	 * Return set of boids that currently overlap with detection sphere of this boid.
	 *
	 * @return set of nearby boids.
	 */
	TSet<ABoidPawn*> GetNearbyBoids() const;

private:
	float sphereRadius;

	UPROPERTY()
		TSet<ABoidPawn*> nearbyBoids = TSet<ABoidPawn*>();
	UPROPERTY()
		AActor* target;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);
};
