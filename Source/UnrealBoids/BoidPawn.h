// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoidPawn.generated.h"

UCLASS()
class UNREALBOIDS_API ABoidPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoidPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boid properties")
		float TopSpeed;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
