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

	currentDirection = GetActorForwardVector();
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

	currentDirection = GetObstacleAvoidDirection();
	SetActorLocation(GetActorLocation() + currentDirection * TopSpeed);
	SetActorRotation(currentDirection.Rotation());
}

FVector ABoidPawn::GetCurrentDirection() const
{
	return currentDirection;
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
	for (FVector point: linetracePoints)
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
