// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveShipForUpgrade.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Ship.h"
#include "MapManager.h"
#include "Tile/BaseTile.h"
#include "Tile/ResourceTile.h"
#include "Building/Shipyard.h"

UBTTask_MoveShipForUpgrade::UBTTask_MoveShipForUpgrade()
{
}

EBTNodeResult::Type UBTTask_MoveShipForUpgrade::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;

	AShip* ShipToMoveForUpgrade = Cast<AShip>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("ShipToMoveForUpgrade")));
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipToMoveForUpgrade"), nullptr);
	AShipyard* ShipyardToFill = Cast<AShipyard>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("ShipyardToFill")));
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipyardToFill"), nullptr);

	if (!ShipToMoveForUpgrade || !ShipyardToFill) return EBTNodeResult::Failed;

	ShipToMoveForUpgrade->SetShipStatus(EShipStatus::MoveForUpgrade);

	if (!GetWorld()->GetGameInstance()->GetSubsystem<UMapManager>()->MakePathToTile(ShipToMoveForUpgrade, ShipyardToFill->GetCurTile(), false))
	{
		ShipToMoveForUpgrade->SetShipStatus(EShipStatus::None);
		return EBTNodeResult::Failed;
	}
	if (!Commander->TryDepartShip(ShipToMoveForUpgrade))
	{
		ShipToMoveForUpgrade->SetShipStatus(EShipStatus::None);
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
