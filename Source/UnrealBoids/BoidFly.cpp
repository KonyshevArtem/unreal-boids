// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidFly.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBoidFly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* boid = OwnerComp.GetAIOwner()->GetPawn();
	const FVector targetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);
	MoveBoid(boid, targetLocation);
	return EBTNodeResult::Succeeded;
}

void UBoidFly::MoveBoid(APawn* boid, FVector targetLocation)
{
	const FRotator targetDirection = (targetLocation - boid->GetActorLocation()).Rotation();
	boid->SetActorLocation(targetLocation);
	boid->SetActorRotation(targetDirection);
}
