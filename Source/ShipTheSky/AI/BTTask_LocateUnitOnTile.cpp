// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_LocateUnitOnTile.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Tile/IslandTile.h"
#include "Unit/BaseUnit.h"
#include "Tile/ResourceTile.h"
#include "Ship.h"

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

	if (!Tile || !Unit)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("IslandTileToLocateUnit"), nullptr);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("UnitToLocate"), nullptr);

	if (Unit->GetCurIslandTile() && !Unit->GetCurShip())
	{
		Commander->TryRelocateUnitOnTile(Unit, Tile);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		if (Unit->GetCurIslandTile() && Unit->GetCurShip())
		{
			UE_LOG(LogTemp, Error, TEXT("Both on Island and Ship. %s"), *Unit->GetName());
		}
		else if (!Unit->GetCurIslandTile() && Unit->GetCurShip())
		{
			UE_LOG(LogTemp, Error, TEXT("Just on Ship. It's acceptable. %s"), *Unit->GetName());
		}
		else if (!Unit->GetCurIslandTile() && !Unit->GetCurShip())
		{
			UE_LOG(LogTemp, Error, TEXT("There must be no this case. %s"), *Unit->GetName());
		}
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Type();
}
