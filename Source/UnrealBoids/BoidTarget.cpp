// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidTarget.h"
#include "Runtime\Engine\Classes\Components\InputComponent.h"

// Sets default values
ABoidTarget::ABoidTarget()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoidTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoidTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoidTarget::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Horizontal", this, &ABoidTarget::SetHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &ABoidTarget::SetVertical);
	PlayerInputComponent->BindAxis("Frontal", this, &ABoidTarget::SetFrontal);
}

void ABoidTarget::SetHorizontal(float axisValue)
{
	FVector currentLocation = GetActorLocation();
	currentLocation.X += axisValue * Speed;
	SetActorLocation(currentLocation);
}

void ABoidTarget::SetVertical(float axisValue)
{
	FVector currentLocation = GetActorLocation();
	currentLocation.Z += axisValue * Speed;
	SetActorLocation(currentLocation);
}

void ABoidTarget::SetFrontal(float axisValue)
{
	FVector currentLocation = GetActorLocation();
	currentLocation.Y += axisValue * Speed;
	SetActorLocation(currentLocation);
}

