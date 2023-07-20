// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_LocateUnitOnTile.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Tile/IslandTile.h"
#include "Unit/BaseUnit.h"

UBTTask_LocateUnitOnTile::UBTTask_LocateUnitOnTile()
{
}

EBTNodeResult::Type UBTTask_LocateUnitOnTile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;

	AIslandTile* Tile = Cast<AIslandTile>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("IslandTileToLocateUnit")));
	ABaseUnit* Unit = Cast<ABaseUnit>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("UnitToLocate")));

	Commander->TryRelocateUnitOnTile(Unit, Tile);

	return EBTNodeResult::Type();
}
