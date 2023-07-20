// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_CreateUnit.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Building/Barracks.h"
#include "Tile/ResourceTile.h"

UBTTask_CreateUnit::UBTTask_CreateUnit()
{
}

EBTNodeResult::Type UBTTask_CreateUnit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;

	ABarracks* Barracks = Cast<ABarracks>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("BarracksToCreateUnit")));
	EUnitType Type = StaticCast<EUnitType>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("UnitTypeToCreate")));
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("BarracksToCreateUnit"), nullptr);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitTypeToCreate"), (uint8)EUnitType::None);

	if (!Barracks) return EBTNodeResult::Failed;
	if (Barracks->GetOwnerCommander() != Barracks->GetCurTile()->GetIslandOwner()) return EBTNodeResult::Failed;

	if (Commander->TryCreateUnit(Barracks, Type)) return EBTNodeResult::Succeeded;
	else return EBTNodeResult::Failed;
}
