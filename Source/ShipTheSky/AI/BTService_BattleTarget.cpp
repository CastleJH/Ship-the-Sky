// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_BattleTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Ship.h"
#include "Tile/BaseTile.h"
#include "Tile/GuardianTile.h"
#include "Tile/IslandTile.h"
#include "Unit/BaseUnit.h"
#include "MapManager.h"

UBTService_BattleTarget::UBTService_BattleTarget()
{
	NodeName = TEXT("Battle Target");

	Interval = 30.0f;
}

void UBTService_BattleTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return;

	for (auto Ship : Commander->Ships)
	{
		if (Ship->GetShipStatus() == EShipStatus::None && Ship->GetCurTile() && Ship->GetCurTile()->GetTileType() == ETileType::Island)
		{
			AGuardianTile* GuardianTile = Cast<AIslandTile>(Ship->GetCurTile())->GetGuardianTile();
			int32 MaxValue = 0;
			ABaseUnit* MaxUnit = nullptr;
			int32 TotalBattlePower = 0;
			for (int32 Idx = 0; Idx < Ship->GetUnitCapacity(); Idx++)
			{
				MaxValue = 0;
				MaxUnit = nullptr;
				for (auto Unit : GuardianTile->UnitsOnThisIsland)
				{
					if (Unit->GetUnitType() == EUnitType::Warrior && Unit->GetBattlePower() > MaxValue && !Ship->CheckAlreadyCandidateBattleUnit(Unit))
					{
						MaxUnit = Unit;
						MaxValue = Unit->GetBattlePower();
					}
				}
				if (!MaxUnit) break;
				Ship->AddCandidateBattleUnit(MaxUnit);
				TotalBattlePower += MaxValue;
			}
			UE_LOG(LogTemp, Warning, TEXT("TotalBattlePower : %d"), TotalBattlePower);
			TArray<TPair<int32, AGuardianTile*>>& OtherIslands = GetWorld()->GetGameInstance()->GetSubsystem<UMapManager>()->NearestIslands[GuardianTile->GetIslandID()];
			int32 Count = 0;
			for (auto OtherGuardianTile : OtherIslands)
			{
				if (!OtherGuardianTile.Value)
				{
					UE_LOG(LogTemp, Error, TEXT("Wrong otherguardiantile."));
					return;
				}
				UE_LOG(LogTemp, Warning, TEXT("OtherGuardianTile.Value->GetBattlePower(): %d"), OtherGuardianTile.Value->GetBattlePower());
				if (OtherGuardianTile.Value->GetIslandOwner() != Commander && OtherGuardianTile.Value->GetBattlePower() < TotalBattlePower && GuardianTile->GetAdjResourceTiles().Num() != GuardianTile->GetAdjTiles().Num())
				{
					for (auto adjs : GuardianTile->GetAdjTiles())
					{
						if (adjs->GetTileType() != ETileType::Island && !adjs->GetShip())
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipToGoBattle"), Ship);
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("GuardianTileToAttack"), OtherGuardianTile.Value);
							return;
						}
					}
				}
				if (++Count == 10) break;
			}
		}
		Ship->EmptyCandidateBattleUnit();
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipToGoBattle"), nullptr);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("GuardianTileToAttack"), nullptr);
}
