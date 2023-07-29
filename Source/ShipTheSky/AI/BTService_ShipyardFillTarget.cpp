// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_ShipyardFillTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Building/BaseBuilding.h"
#include "Building/Shipyard.h"
#include "Tile/BaseTile.h"
#include "Tile/ResourceTile.h"
#include "Ship/Ship.h"
#include "Game/MapManager.h"

UBTService_ShipyardFillTarget::UBTService_ShipyardFillTarget()
{
	NodeName = TEXT("Shipyard Fill Target");

	Interval = 10.0f;
}

void UBTService_ShipyardFillTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return;

	AShip* ShipToMoveForUpgrade = nullptr;
	AShipyard* ShipyardToFill = nullptr;
	int32 TileDistance = 0;
	int32 TmpTileDistance = 0;

	for (auto Ship : Commander->Ships)
	{
		if (Ship->GetShipStatus() == EShipStatus::None && Ship->GetCurTile())
		{
			AResourceTile* ResTile = Cast<AResourceTile>(Ship->GetCurTile());
			if (ResTile && ResTile->GetBuilding() && ResTile->GetBuilding()->GetBuildingType() == EBuildingType::Shipyard) continue;
			ShipToMoveForUpgrade = Ship;
			break;
		}
	}

	if (ShipToMoveForUpgrade)
	{
		for (auto Building : Commander->Buildings)
		{
			if (Building->GetBuildingType() == EBuildingType::Shipyard && !Building->GetCurTile()->GetShip())
			{
				if (!ShipyardToFill)
				{
					ShipyardToFill = Cast<AShipyard>(Building);
					TileDistance = UMapManager::GetDistanceOfTwoTile(ShipyardToFill->GetCurTile(), ShipToMoveForUpgrade->GetCurTile());
				}
				else 
				{
					TmpTileDistance = UMapManager::GetDistanceOfTwoTile(Building->GetCurTile(), ShipToMoveForUpgrade->GetCurTile());
					if (TmpTileDistance < TileDistance)
					{
						TileDistance = TmpTileDistance;
						ShipyardToFill = Cast<AShipyard>(Building);
					}
				}
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipToMoveForUpgrade"), ShipToMoveForUpgrade);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipyardToFill"), ShipyardToFill);
}
