// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_ConstructionTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Tile/GuardianTile.h"
#include "Tile/ResourceTile.h"
#include "Building/BaseBuilding.h"
#include "Enums.h"

UBTService_ConstructionTarget::UBTService_ConstructionTarget()
{
	NodeName = TEXT("Building Construction Target");

	Interval = 10.0f;
}

void UBTService_ConstructionTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return;

	AResourceTile* TargetTile = nullptr;
	int32 MaxTileEmpty = 0;
	int32 MaxTileTypeCnt = 0;
	bool TileTypeExist[3] = { false, false, false };
	for (auto GuardianTile : Commander->OwningIslands)
	{
		int32 CurTileEmpty = 0;
		int32 CurTileTypeCnt = 0;
		int32 CurBuilt = 0;
		TileTypeExist[0] = false;
		TileTypeExist[1] = false;
		TileTypeExist[2] = false;
		AResourceTile* CurTargetTile = nullptr;
		bool CanBuild = false;
		TArray<AResourceTile*> ResourceTiles = GuardianTile->GetAdjResourceTiles();
		for (auto Tile : ResourceTiles)
		{
			if (!Tile->GetBuilding())
			{
				CurTileEmpty++;
				CanBuild = true;
				CurTargetTile = Tile;
			}
			else
			{
				CurTileEmpty--;
				CurBuilt++;
			}
			if (!TileTypeExist[(uint8)Tile->GetIslandType()])
			{
				TileTypeExist[(uint8)Tile->GetIslandType()] = true;
				CurTileTypeCnt++;
			}
		}
		if (CanBuild && CurBuilt < 3)
		{
			if (CurTileTypeCnt > MaxTileTypeCnt)
			{
				MaxTileTypeCnt = CurTileTypeCnt;
				MaxTileEmpty = CurTileEmpty;
				TargetTile = CurTargetTile;
			}
			else if (CurTileTypeCnt == MaxTileTypeCnt && MaxTileEmpty < CurTileEmpty)
			{
				MaxTileEmpty = CurTileEmpty;
				TargetTile = CurTargetTile;
			}
		}
	}

	if (TargetTile)
	{
		int32 BuildingExists[4] = { 0 };
		TArray<AResourceTile*> SameIslandTiles = TargetTile->GetGuardianTile()->GetAdjResourceTiles();
		for (auto Tile : SameIslandTiles)
		{
			if (Tile->GetBuilding())
			{
				BuildingExists[(int32)Tile->GetBuilding()->GetBuildingType()]++;
			}
		}

		int32 BuildingCount[4] = { 0 };
		for (auto Building : Commander->Buildings)
		{
			BuildingCount[(int32)Building->GetBuildingType()]++;
		}

		if (BuildingExists[(int32)EBuildingType::Barracks] == 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BuildingTypeToBuild"), (uint8)EBuildingType::Barracks);
		}
		else if (BuildingExists[(int32)EBuildingType::Shipyard] == 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BuildingTypeToBuild"), (uint8)EBuildingType::Shipyard);
		}
		else if (BuildingExists[(int32)EBuildingType::Sanctuary] == 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BuildingTypeToBuild"), (uint8)EBuildingType::Sanctuary);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BuildingTypeToBuild"), (uint8)EBuildingType::None);
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ResourceTileToBuild"), TargetTile);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BuildingTypeToBuild"), (uint8)EBuildingType::None);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ResourceTileToBuild"), nullptr);
	}
}
