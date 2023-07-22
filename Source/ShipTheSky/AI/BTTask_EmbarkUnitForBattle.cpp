// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_EmbarkUnitForBattle.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Ship.h"

UBTTask_EmbarkUnitForBattle::UBTTask_EmbarkUnitForBattle()
{
}


EBTNodeResult::Type UBTTask_EmbarkUnitForBattle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;

	AShip* Ship = Cast<AShip>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("ShipToGoBattle")));
	if (!Ship) return EBTNodeResult::Failed;

	if (!Ship->EmbarkCandiateBattleUnits())
	{
		Ship->DisembarkAllUnits();
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
