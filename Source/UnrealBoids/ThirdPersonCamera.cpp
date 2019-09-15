// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCamera.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

AThirdPersonCamera::AThirdPersonCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AThirdPersonCamera::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (playerController)
	{
		playerController->InputComponent->BindAxis("MouseX", this, &AThirdPersonCamera::SetHorizontal);
		playerController->InputComponent->BindAxis("MouseY", this, &AThirdPersonCamera::SetVertical);
		playerController->InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AThirdPersonCamera::ZoomIn);
		playerController->InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AThirdPersonCamera::ZoomOut);
	}
}

void AThirdPersonCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector orbitPosition = Target->GetActorLocation();
	const FVector cameraLocation = GetNewCameraLocation(orbitPosition);
	const FRotator cameraRotation = (Target->GetActorLocation() - cameraLocation).Rotation();

	SetActorRotation(cameraRotation);
	SetActorLocation(cameraLocation);
}

void AThirdPersonCamera::SetHorizontal(float axisValue)
{
	currentRotation.Yaw -= axisValue;
}

void AThirdPersonCamera::SetVertical(float axisValue)
{
	currentRotation.Pitch += axisValue;
}

void AThirdPersonCamera::ZoomIn()
{
	Distance -= ZoomAmount;
}

void AThirdPersonCamera::ZoomOut()
{
	Distance += ZoomAmount;
}

FVector AThirdPersonCamera::GetNewCameraLocation(FVector orbitLocation) const
{
	const FVector direction = currentRotation.Vector();
	const FVector cameraLocation = orbitLocation + direction * Distance;
	return cameraLocation;
}

