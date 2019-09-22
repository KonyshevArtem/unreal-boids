// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BoidFly.generated.h"

/**
 * 
 */
UCLASS()
class UNREALBOIDS_API UBoidFly : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	/**
	 * Set boid rotation to face targetLocation and move boid to targetLocation.
	 *
	 * @param boid Pawn to move and rotate.
	 * @param targetLocation Location to which rotate and move boid.
	 */
	static void MoveBoid(APawn* boid, FVector targetLocation);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector BlackboardKey;
};
