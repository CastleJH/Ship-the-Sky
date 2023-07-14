// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_UnitUpgradeTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Tile/GuardianTile.h"
#include "Tile/ResourceTile.h"
#include "Building/BaseBuilding.h"
#include "Unit/BaseUnit.h"

UBTService_UnitUpgradeTarget::UBTService_UnitUpgradeTarget()
{
	NodeName = TEXT("Unit Upgrade Target");

	Interval = 2.0f;
}

void UBTService_UnitUpgradeTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return;

	AIslandTile* IslandTileToLocateUnit = nullptr;
	ABaseUnit* UnitToLocate = nullptr;
	ABaseUnit* UnitToUpgrade = nullptr;
	EUnitStat UnitStatToUpgrade = EUnitStat::None;
	int32 MinConsumption = 99999999;

	for (auto GuardianTile : Commander->OwningIslands)
	{
		AIslandTile* TmpIslandTileToLocateUnit = nullptr;
		for (auto Tile : GuardianTile->GetAdjResourceTiles())
		{
			if (Tile->GetBuilding() && Tile->GetBuilding()->GetBuildingType() == EBuildingType::Barracks)
			{
				TmpIslandTileToLocateUnit = Tile;
				break;
			}
		}
		if (TmpIslandTileToLocateUnit)
		{
			for (auto Unit : GuardianTile->UnitsOnThisIsland)
			{
				TPair<EUnitStat, int32> Recommendation = Unit->GetStatUpgradeRecommendation();
				if (Recommendation.Key == EUnitStat::None) continue;
				if (Recommendation.Value < MinConsumption)
				{
					MinConsumption = Recommendation.Value;

					IslandTileToLocateUnit = TmpIslandTileToLocateUnit;
					UnitToLocate = Unit;
					UnitToUpgrade = Unit;
					UnitStatToUpgrade = Recommendation.Key;
				}
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("IslandTileToLocateUnit"), IslandTileToLocateUnit);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("UnitToLocate"), UnitToLocate);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("UnitToUpgrade"), UnitToUpgrade);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitStatToUpgrade"), (uint8)UnitStatToUpgrade);
}
