// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_SelectTileToBuild.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Tile/GuardianTile.h"
#include "Tile/ResourceTile.h"
#include "Building/BaseBuilding.h"
#include "Enums.h"

UBTService_SelectTileToBuild::UBTService_SelectTileToBuild()
{
	NodeName = TEXT("Select Resource Tile To Build");

	Interval = 10.0f;
}

void UBTService_SelectTileToBuild::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return;

	AResourceTile* TargetTile = nullptr;
	int32 MinBuilt = 999;
	for (auto GuardianTile : Commander->OwningIslands)
	{
		int32 CurBuilt = 0;
		AResourceTile* CurTargetTile = nullptr;
		bool CanBuild = false;
		TArray<AResourceTile*> ResourceTiles = GuardianTile->GetAdjResourceTiles();
		for (auto Tile : ResourceTiles)
		{
			if (Tile->GetBuilding())
			{
				CurBuilt++;
			}
			else
			{
				CanBuild = true;
				CurTargetTile = Tile;
			}
		}
		if (CanBuild)
		{
			if (MinBuilt > CurBuilt)
			{
				MinBuilt = CurBuilt;
				TargetTile = CurTargetTile;
			}
		}
	}

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

	if (BuildingCount[(int32)EBuildingType::Barracks] * 2 > BuildingCount[(int32)EBuildingType::Shipyard] * 3 && BuildingExists[(int32)EBuildingType::Shipyard] == 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BuildingTypeToBuild"), (uint8)EBuildingType::Shipyard);
	}
	else if (BuildingExists[(int32)EBuildingType::Barracks] < 2)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BuildingTypeToBuild"), (uint8)EBuildingType::Barracks);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BuildingTypeToBuild"), (uint8)EBuildingType::None);
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ResourceTileToBuild"), TargetTile);
}
