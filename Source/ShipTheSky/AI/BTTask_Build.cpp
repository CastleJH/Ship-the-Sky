// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Build.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Tile/ResourceTile.h"
#include "Enums.h"

UBTTask_Build::UBTTask_Build()
{
}

EBTNodeResult::Type UBTTask_Build::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;

	AResourceTile* TargetTile = Cast<AResourceTile>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("ResourceTileToBuild")));
	EBuildingType Type = StaticCast<EBuildingType>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("BuildingTypeToBuild")));
	if (TargetTile)
	{
		if (Commander->TryConstructBuilding(TargetTile, Type)) return EBTNodeResult::Succeeded;
		else return EBTNodeResult::Failed;
	}
	else return EBTNodeResult::Failed;
}
