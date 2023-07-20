// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_OptimizeUnitPlacement.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Tile/GuardianTile.h"
#include "Unit/BaseUnit.h"
#include "Tile/ResourceTile.h"
#include "Battle/BattleComponent.h"

UBTTask_OptimizeUnitPlacement::UBTTask_OptimizeUnitPlacement()
{
}

EBTNodeResult::Type UBTTask_OptimizeUnitPlacement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;
	return EBTNodeResult::Failed;

	AGuardianTile* GuardianTileToOptimizePlacement = Cast<AGuardianTile>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("GuardianTileToOptimizePlacement")));
	if (!GuardianTileToOptimizePlacement)
	{
		UE_LOG(LogTemp, Error, TEXT("No GuardianTileToOptimizePlacement"));
		return EBTNodeResult::Failed;
	}

	if (GuardianTileToOptimizePlacement->bIsAttackedRecently)
	{
		TArray<ABaseUnit*> Warriors;
		TArray<ABaseUnit*> NotWarriors;
		for (auto Unit : GuardianTileToOptimizePlacement->UnitsOnThisIsland)
		{
			if (Unit->GetUnitType() == EUnitType::Warrior) Warriors.Add(Unit);
			else NotWarriors.Add(Unit);
			Warriors.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
				return A.BattleComponent->GetCurHP() < B.BattleComponent->GetCurHP();
				});
			NotWarriors.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
				return A.BattleComponent->GetCurHP() < B.BattleComponent->GetCurHP();
				});
		}
		for (auto Tile : GuardianTileToOptimizePlacement->GetAdjResourceTiles())
		{
			if (!Warriors.IsEmpty())
			{
				Commander->TryRelocateUnitOnTile(Warriors.Last(), Tile);
				Warriors.Pop();
			}
			else if (!NotWarriors.IsEmpty())
			{
				Commander->TryRelocateUnitOnTile(NotWarriors.Last(), Tile);
				NotWarriors.Pop();
			}
			else break;
		}
	}
	else
	{
		TArray<AResourceTile*> TilesToMatch;
		TArray<ABaseUnit*> Miners;
		TArray<ABaseUnit*> Woodcutters;
		TArray<ABaseUnit*> Farmers;
		for (auto Unit : GuardianTileToOptimizePlacement->UnitsOnThisIsland)
		{
			switch (Unit->GetUnitType())
			{
			case EUnitType::Miner:
				Miners.Add(Unit);
				break;
			case EUnitType::Woodcutter:
				Woodcutters.Add(Unit);
				break;
			case EUnitType::Farmer:
				Farmers.Add(Unit);
				break;
			default:
				break;
			}
		}
		Miners.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
			return A.GetEfficiency() < B.GetEfficiency();
			});
		Woodcutters.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
			return A.GetEfficiency() < B.GetEfficiency();
			});
		Farmers.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
			return A.GetEfficiency() < B.GetEfficiency();
			});
		for (auto Tile : GuardianTileToOptimizePlacement->GetAdjResourceTiles())
		{
			switch (Tile->GetIslandType())
			{
			case EIslandTileType::Mine:
				if (!Miners.IsEmpty())
				{
					Commander->TryRelocateUnitOnTile(Miners.Last(), Tile);
					Miners.Pop();
				}
			case EIslandTileType::Forest:
				if (!Woodcutters.IsEmpty())
				{
					Commander->TryRelocateUnitOnTile(Woodcutters.Last(), Tile);
					Woodcutters.Pop();
				}
			case EIslandTileType::Farm:
				if (!Farmers.IsEmpty())
				{
					Commander->TryRelocateUnitOnTile(Farmers.Last(), Tile);
					Farmers.Pop();
				}
				break;
			default:
				break;
			}
		}
	}
}
