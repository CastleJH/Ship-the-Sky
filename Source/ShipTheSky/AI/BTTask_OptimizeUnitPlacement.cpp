// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_OptimizeUnitPlacement.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Tile/GuardianTile.h"
#include "Unit/BaseUnit.h"
#include "Tile/ResourceTile.h"
#include "Battle/BattleComponent.h"

UBTTask_OptimizeUnitPlacement::UBTTask_OptimizeUnitPlacement()
{
}

EBTNodeResult::Type UBTTask_OptimizeUnitPlacement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;

	for (auto GuardianTile : Commander->OwningIslands)
	{
		GuardianTile->OptimizeUnitPlacementForBest();
	}
	/*
	AGuardianTile* GuardianTileToOptimizePlacement = Cast<AGuardianTile>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("GuardianTileToOptimizePlacement")));
	if (!GuardianTileToOptimizePlacement)
	{
		UE_LOG(LogTemp, Error, TEXT("No GuardianTileToOptimizePlacement"));
		return EBTNodeResult::Failed;
	}

	UE_LOG(LogTemp, Warning, TEXT("passed.."));
	if (GuardianTileToOptimizePlacement->bIsAttackedRecently) GuardianTileToOptimizePlacement->OptimizeUnitPlacementForDefense();
	else GuardianTileToOptimizePlacement->OptimizeUnitPlacementForRecovery();
	*/
	return EBTNodeResult::Succeeded;
}
