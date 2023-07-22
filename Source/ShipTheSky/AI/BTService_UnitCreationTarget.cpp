// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_UnitCreationTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Tile/GuardianTile.h"
#include "Tile/ResourceTile.h"
#include "Building/BaseBuilding.h"
#include "Building/Barracks.h"
#include "Unit/BaseUnit.h"

UBTService_UnitCreationTarget::UBTService_UnitCreationTarget()
{
	NodeName = TEXT("Unit Creation Target");

	Interval = 1.0f;
}

void UBTService_UnitCreationTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return;

	ABarracks* TargetBarracks = nullptr;
	int32 MinWaiting = 999;
	bool bBarracksExist = false;

	for (auto GuardianTile : Commander->OwningIslands)
	{
		bBarracksExist = false;
		MinWaiting = 999;
		TArray<AResourceTile*> ResourceTiles = GuardianTile->GetAdjResourceTiles();
		int32 EmptyTiles[4] = { 0 };
		EmptyTiles[(uint8)EUnitType::Warrior] = ResourceTiles.Num();

		for (auto Unit : GuardianTile->UnitsOnThisIsland)
		{
			EmptyTiles[(uint8)Unit->GetUnitType()]--;
		}

		for (auto Tile : ResourceTiles)
		{
			if (Tile->GetBuilding() && Tile->GetBuilding()->GetBuildingType() == EBuildingType::Barracks)
			{
				bBarracksExist = true;
				ABarracks* Barracks = Cast<ABarracks>(Tile->GetBuilding());
				for (auto Type : Barracks->WaitingUnitArray)
				{
					EmptyTiles[(uint8)Type]--;
				}
				if (Barracks->WaitingUnitArray.Num() < MinWaiting)
				{
					TargetBarracks = Barracks;
					MinWaiting = Barracks->WaitingUnitArray.Num();
				}
			}
			EmptyTiles[(uint8)Tile->GetIslandType()]++;
		}
		if (!bBarracksExist) continue;
		
		if (EmptyTiles[(uint8)EUnitType::Farmer] > 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitTypeToCreate"), (uint8)EUnitType::Farmer);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("BarracksToCreateUnit"), TargetBarracks);
			return;
		}
		else if (EmptyTiles[(uint8)EUnitType::Miner] > 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitTypeToCreate"), (uint8)EUnitType::Miner);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("BarracksToCreateUnit"), TargetBarracks);
			return;
		}
		else if (EmptyTiles[(uint8)EUnitType::Woodcutter] > 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitTypeToCreate"), (uint8)EUnitType::Woodcutter);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("BarracksToCreateUnit"), TargetBarracks);
			return;
		}
		else if (EmptyTiles[(uint8)EUnitType::Warrior] > 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitTypeToCreate"), (uint8)EUnitType::Warrior);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("BarracksToCreateUnit"), TargetBarracks);
			return;
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitTypeToCreate"), (uint8)EUnitType::None);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("BarracksToCreateUnit"), nullptr);
}
