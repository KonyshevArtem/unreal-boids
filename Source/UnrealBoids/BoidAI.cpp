// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidAI.h"

void ABoidAI::BeginPlay()
{
	Super::BeginPlay();

	if (BoidBehaviourTree)
	{
		RunBehaviorTree(BoidBehaviourTree);
	}
}
