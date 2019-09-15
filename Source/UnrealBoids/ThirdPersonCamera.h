// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "ThirdPersonCamera.generated.h"

UCLASS()
class UNREALBOIDS_API AThirdPersonCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	AThirdPersonCamera();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Third person camera properties")
		AActor* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Third person camera properties")
		float Distance = 8000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Third person camera properties")
		float ZoomAmount = 1;

private:
	UPROPERTY()
		FRotator currentRotation = GetActorRotation();

	void SetHorizontal(float axisValue);
	void SetVertical(float axisValue);
	void ZoomIn();
	void ZoomOut();
	
	/**
	 * Get new camera location based on currentRotation around orbit location
	 *
	 * @param orbitLocation Location around which camera is rotating
	 * @return new location of camera
	 */
	FVector GetNewCameraLocation(FVector orbitLocation) const;
};
