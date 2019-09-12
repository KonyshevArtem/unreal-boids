// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidPawn.h"

// Sets default values
ABoidPawn::ABoidPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoidPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoidPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector currentLocation = GetActorLocation();
	SetActorLocation(currentLocation + GetActorForwardVector() * TopSpeed);
}
