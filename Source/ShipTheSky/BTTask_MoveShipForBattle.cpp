// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveShipForBattle.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Tile/GuardianTile.h"
#include "MapManager.h"
#include "Tile/BaseTile.h"
#include "Ship.h"

UBTTask_MoveShipForBattle::UBTTask_MoveShipForBattle()
{
}

EBTNodeResult::Type UBTTask_MoveShipForBattle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;

	AGuardianTile* GuardianTileToAttack = Cast<AGuardianTile>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("GuardianTileToAttack")));
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("GuardianTileToAttack"), nullptr);

	AShip* Ship = Cast<AShip>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("ShipToGoBattle")));
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipToGoBattle"), nullptr);

	if (!GuardianTileToAttack) return EBTNodeResult::Failed;
	if (!Ship || Ship->GetShipStatus() != EShipStatus::None) return EBTNodeResult::Failed;

	if (!GetWorld()->GetGameInstance()->GetSubsystem<UMapManager>()->MakePathToTile(Ship, GuardianTileToAttack, true)) return EBTNodeResult::Failed;
	
	Ship->SetShipStatus(EShipStatus::MoveForBattle);
	Ship->FollowPath();

	return EBTNodeResult::Succeeded;
}
