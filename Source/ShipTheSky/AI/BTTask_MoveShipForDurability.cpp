// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveShipForDurability.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Ship/Ship.h"
#include "Tile/BaseTile.h"
#include "Game/MapManager.h"
#include "Tile/GuardianTile.h"
#include "Tile/ResourceTile.h"

UBTTask_MoveShipForDurability::UBTTask_MoveShipForDurability()
{
}

EBTNodeResult::Type UBTTask_MoveShipForDurability::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;

	int32 MinDist = 1987654321;
	AGuardianTile* TargetIsland = nullptr;
	for (auto Ship : Commander->Ships)
	{
		TargetIsland = nullptr;
		if (Ship->GetCurTile() && Ship->GetCurTile()->GetTileType() != ETileType::Island && Ship->GetShipStatus() == EShipStatus::None)
		{
			for (auto GTile : Commander->OwningIslands)
			{
				int32 Dist = GetWorld()->GetGameInstance()->GetSubsystem<UMapManager>()->GetDistanceOfTwoTile(Ship->GetCurTile(), GTile);
				if (Dist < MinDist)
				{
					MinDist = Dist;
					TargetIsland = GTile;
				}
			}
			if (TargetIsland)
			{
				for (auto Tile : TargetIsland->GetAdjResourceTiles())
				{
					if (!Tile->GetShip() && GetWorld()->GetGameInstance()->GetSubsystem<UMapManager>()->MakePathToTile(Ship, Tile, false))
					{
						Ship->SetShipStatus(EShipStatus::MoveForDurability);
						Ship->FollowPath();
						break;
					}
				}
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
