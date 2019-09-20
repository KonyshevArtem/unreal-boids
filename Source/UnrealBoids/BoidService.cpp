// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidService.h"
#include "AIController.h"
#include "Engine/Engine.h"
#include "BehaviorTree/BlackboardComponent.h"

UBoidService::UBoidService()
{
	bNotifyBecomeRelevant = true;
}

void UBoidService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	ABoidPawn* ownerBoid = Cast<ABoidPawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!ownerBoid) return;

	const FVector targetForce = GetForce(ownerBoid, GetTargetDirection(ownerBoid)) * ownerBoid->TargetWeight;
	const FVector obstacleAvoidForce = GetForce(ownerBoid, GetObstacleAvoidDirection(ownerBoid)) * ownerBoid->CollisionAvoidanceWeight;
	const FVector separationForce = GetForce(ownerBoid, GetSeparationDirection(ownerBoid)) * ownerBoid->SeparationWeight;
	const FVector cohesionForce = GetForce(ownerBoid, GetCohesionDirection(ownerBoid)) * ownerBoid->CohesionWeight;
	const FVector alignmentForce = GetForce(ownerBoid, GetAlignmentDirection(ownerBoid)) * ownerBoid->AlignmentWeight;

	FVector currentVelocity = ownerBoid->GetCurrentVelocity();
	currentVelocity += (obstacleAvoidForce + separationForce + cohesionForce + alignmentForce + targetForce) * GetWorld()->GetDeltaSeconds();
	const float speed = FMath::Clamp(currentVelocity.Size(), ownerBoid->MinSpeed, ownerBoid->MaxSpeed);
	const FVector direction = currentVelocity.GetSafeNormal();
	currentVelocity = direction * speed;

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlueprintKey.SelectedKeyName, ownerBoid->GetActorLocation() + currentVelocity);
}

FVector UBoidService::GetObstacleAvoidDirection(ABoidPawn* ownerBoid) const
{
	for (FVector point : linetracePoints)
	{
		FVector worldPoint = ownerBoid->GetTransform().TransformPosition(point * ownerBoid->GetSphereRadius());
		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(ownerBoid);
		if (!GetWorld()->LineTraceSingleByChannel(hitResult, ownerBoid->GetActorLocation(), worldPoint, ECC_GameTraceChannel1, params))
		{
			return (worldPoint - ownerBoid->GetActorLocation()).GetSafeNormal();
		}
	}
	return ownerBoid->GetActorForwardVector();
}

FVector UBoidService::GetSeparationDirection(ABoidPawn* ownerBoid)
{
	const TSet<ABoidPawn*> nearbyBoids = ownerBoid->GetNearbyBoids();
	if (nearbyBoids.Num() == 0) return FVector::ZeroVector;

	FVector separationDirection = FVector::ZeroVector;
	for (ABoidPawn* boid : nearbyBoids)
	{
		separationDirection += ownerBoid->GetActorLocation() - boid->GetActorLocation();
	}
	return separationDirection / nearbyBoids.Num();
}

FVector UBoidService::GetCohesionDirection(ABoidPawn* ownerBoid)
{
	const TSet<ABoidPawn*> nearbyBoids = ownerBoid->GetNearbyBoids();
	if (nearbyBoids.Num() == 0) return FVector::ZeroVector;

	FVector centerOfMass = FVector::ZeroVector;
	for (ABoidPawn* boid : nearbyBoids)
	{
		centerOfMass += boid->GetActorLocation();
	}
	centerOfMass /= nearbyBoids.Num();
	return centerOfMass - ownerBoid->GetActorLocation();
}

FVector UBoidService::GetAlignmentDirection(ABoidPawn* ownerBoid)
{
	const TSet<ABoidPawn*> nearbyBoids = ownerBoid->GetNearbyBoids();
	if (nearbyBoids.Num() == 0) return FVector::ZeroVector;

	FVector alignmentDirection = FVector::ZeroVector;
	for (ABoidPawn* boid : nearbyBoids)
	{
		alignmentDirection += boid->GetCurrentVelocity();
	}
	return alignmentDirection / nearbyBoids.Num();
}

FVector UBoidService::GetTargetDirection(ABoidPawn* ownerBoid)
{
	AActor* target = ownerBoid->GetTarget();
	if (!target) return FVector::ZeroVector;
	return (target->GetActorLocation() - ownerBoid->GetActorLocation()).GetSafeNormal();
}

FVector UBoidService::GetForce(ABoidPawn* ownerBoid, FVector direction)
{
	const FVector force = direction.GetSafeNormal() * ownerBoid->MaxSpeed - ownerBoid->GetCurrentVelocity();
	return force;
}
