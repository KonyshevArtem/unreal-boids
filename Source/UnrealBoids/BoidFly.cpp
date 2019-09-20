// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidFly.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBoidFly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* pawn = OwnerComp.GetAIOwner()->GetPawn();
	const FVector targetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);
	const FRotator targetDirection = (targetLocation - pawn->GetActorLocation()).Rotation();
	pawn->SetActorLocation(targetLocation);
	pawn->SetActorRotation(targetDirection);
	
	return EBTNodeResult::Succeeded;
}
