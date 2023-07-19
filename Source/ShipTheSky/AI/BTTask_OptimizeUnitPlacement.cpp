// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_OptimizeUnitPlacement.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"

UBTTask_OptimizeUnitPlacement::UBTTask_OptimizeUnitPlacement()
{
}

EBTNodeResult::Type UBTTask_OptimizeUnitPlacement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;
	return EBTNodeResult::Failed;
}
