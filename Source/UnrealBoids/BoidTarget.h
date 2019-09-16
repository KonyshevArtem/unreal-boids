// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoidTarget.generated.h"

UCLASS()
class UNREALBOIDS_API ABoidTarget : public APawn
{
	GENERATED_BODY()

public:
	ABoidTarget();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid target properties")
		float Speed = 30;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
		void SetHorizontal(float axisValue);
	UFUNCTION()
		void SetVertical(float axisValue);
	UFUNCTION()
		void SetFrontal(float axisValue);

};
