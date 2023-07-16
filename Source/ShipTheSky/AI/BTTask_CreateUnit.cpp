// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_CreateUnit.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Building/Barracks.h"

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

	if (Commander->TryCreateUnit(Barracks, Type))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitTypeToCreate"), (uint8)EUnitType::None);
		return EBTNodeResult::Succeeded;
	}
	else return EBTNodeResult::Failed;
}
