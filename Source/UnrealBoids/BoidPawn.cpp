// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidPawn.h"
#include "Engine/Engine.h"

ABoidPawn::ABoidPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABoidPawn::BeginPlay()
{
	Super::BeginPlay();

	currentVelocity = GetActorForwardVector();
	if (USphereComponent * sphereComponent = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass())))
	{
		sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABoidPawn::BeginOverlap);
		sphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABoidPawn::EndOverlap);
		sphereRadius = sphereComponent->GetScaledSphereRadius();
	}
}

void ABoidPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector targetForce = GetForce(GetTargetDirection()) * TargetWeight;
	const FVector obstacleAvoidForce = GetForce(GetObstacleAvoidDirection()) * CollisionAvoidanceWeight;
	const FVector separationForce = GetForce(GetSeparationDirection()) * SeparationWeight;
	const FVector cohesionForce = GetForce(GetCohesionDirection()) * CohesionWeight;
	const FVector alignmentForce = GetForce(GetAlignmentDirection()) * AlignmentWeight;

	currentVelocity += (obstacleAvoidForce + separationForce + cohesionForce + alignmentForce + targetForce) * DeltaTime;
	const float speed = FMath::Clamp(currentVelocity.Size(), MinSpeed, MaxSpeed);
	const FVector direction = currentVelocity.GetSafeNormal();
	currentVelocity = direction * speed;


	SetActorLocation(GetActorLocation() + currentVelocity);
	SetActorRotation(currentVelocity.Rotation());
}

FVector ABoidPawn::GetCurrentVelocity() const
{
	return currentVelocity;
}

void ABoidPawn::SetTarget(AActor* target)
{
	this->target = target;
}

void ABoidPawn::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	if (ABoidPawn * boid = Cast<ABoidPawn>(OtherActor))
	{
		if (nearbyBoids.Contains(boid)) return;
		nearbyBoids.Add(boid);
	}
}

void ABoidPawn::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor == this) return;

	if (ABoidPawn * boid = Cast<ABoidPawn>(OtherActor))
	{
		if (!nearbyBoids.Contains(boid)) return;
		nearbyBoids.Remove(boid);
	}
}

FVector ABoidPawn::GetObstacleAvoidDirection() const
{
	for (FVector point : linetracePoints)
	{
		FVector worldPoint = GetTransform().TransformPosition(point * sphereRadius);
		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		if (!GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), worldPoint, ECC_GameTraceChannel1, params))
		{
			return (worldPoint - GetActorLocation()).GetSafeNormal();
		}
	}
	return GetActorForwardVector();
}

FVector ABoidPawn::GetSeparationDirection() const
{
	if (nearbyBoids.Num() == 0) return FVector::ZeroVector;

	FVector separationDirection = FVector::ZeroVector;
	for (ABoidPawn* boid : nearbyBoids)
	{
		separationDirection += GetActorLocation() - boid->GetActorLocation();
	}
	return separationDirection / nearbyBoids.Num();
}

FVector ABoidPawn::GetCohesionDirection() const
{
	if (nearbyBoids.Num() == 0) return FVector::ZeroVector;

	FVector centerOfMass = FVector::ZeroVector;
	for (ABoidPawn* boid : nearbyBoids)
	{
		centerOfMass += boid->GetActorLocation();
	}
	centerOfMass /= nearbyBoids.Num();
	return centerOfMass - GetActorLocation();
}

FVector ABoidPawn::GetAlignmentDirection() const
{
	if (nearbyBoids.Num() == 0) return FVector::ZeroVector;

	FVector alignmentDirection = FVector::ZeroVector;
	for (ABoidPawn* boid : nearbyBoids)
	{
		alignmentDirection += boid->GetCurrentVelocity();
	}
	return alignmentDirection / nearbyBoids.Num();
}

FVector ABoidPawn::GetTargetDirection() const
{
	if (!target) return FVector::ZeroVector;
	return (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
}

FVector ABoidPawn::GetForce(FVector direction) const
{
	const FVector force = direction.GetSafeNormal() * MaxSpeed - currentVelocity;
	return force;
}
