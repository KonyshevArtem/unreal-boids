// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidPawn.h"
#include "Engine/Engine.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

ABoidPawn::ABoidPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABoidPawn::BeginPlay()
{
	Super::BeginPlay();

	if (USphereComponent * sphereComponent = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass())))
	{
		sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABoidPawn::BeginOverlap);
		sphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABoidPawn::EndOverlap);
		sphereRadius = sphereComponent->GetScaledSphereRadius();
	}
}

void ABoidPawn::SetTarget(AActor* target) const
{
	GetController<AAIController>()->GetBlackboardComponent()->SetValueAsObject("Target", target);
}

float ABoidPawn::GetSphereRadius() const
{
	return sphereRadius;
}

TSet<ABoidPawn*> ABoidPawn::GetNearbyBoids() const
{
	return nearbyBoids;
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
