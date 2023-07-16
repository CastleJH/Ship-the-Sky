// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CreateShip.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Building/Shipyard.h"

UBTTask_CreateShip::UBTTask_CreateShip()
{
}

EBTNodeResult::Type UBTTask_CreateShip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;

	AShipyard* Shipyard = Cast<AShipyard>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("ShipyardToCreateShip")));

	if (Commander->TryCreateShip(Shipyard))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipyardToCreateShip"), nullptr);
		return EBTNodeResult::Succeeded;
	}
	else return EBTNodeResult::Failed;
}
