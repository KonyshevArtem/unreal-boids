// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BoidAI.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALBOIDS_API ABoidAI : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBehaviorTree* BoidBehaviourTree;
};
