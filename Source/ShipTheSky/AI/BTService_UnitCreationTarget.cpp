// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_UnitCreationTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Tile/GuardianTile.h"
#include "Tile/ResourceTile.h"
#include "Building/BaseBuilding.h"
#include "Building/Barracks.h"
#include "Unit/BaseUnit.h"
#include "Enums.h"

UBTService_UnitCreationTarget::UBTService_UnitCreationTarget()
{
	NodeName = TEXT("Unit Creation Target");

	Interval = 1.0f;
}

void UBTService_UnitCreationTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UE_LOG(LogTemp, Warning, TEXT("Unit Creation Target"));

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return;

	ABarracks* TargetBarracks = nullptr;
	int32 MinWaiting = 999;
	for (auto GuardianTile : Commander->OwningIslands)
	{
		TArray<AResourceTile*> ResourceTiles = GuardianTile->GetAdjResourceTiles();
		int32 EmptyTiles[4] = { 0 };
		for (auto Tile : ResourceTiles)
		{
			if (Tile->GetBuilding() && Tile->GetBuilding()->GetBuildingType() == EBuildingType::Barracks)
			{
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
			if (!Tile->GetUnit() || (uint8)Tile->GetUnit()->GetUnitType() != (uint8)Tile->GetIslandType())
			{
				EmptyTiles[(uint8)Tile->GetIslandType()]++;
			}
		}
		if (EmptyTiles[(uint8)EUnitType::Farmer] > 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitTypeToCreate"), (uint8)EUnitType::Farmer);
		}
		else if (EmptyTiles[(uint8)EUnitType::Miner] > 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitTypeToCreate"), (uint8)EUnitType::Miner);
		}
		else if (EmptyTiles[(uint8)EUnitType::Woodcutter] > 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitTypeToCreate"), (uint8)EUnitType::Woodcutter);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("UnitTypeToCreate"), (uint8)EUnitType::None);
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("BuildingToCreateUnit"), TargetBarracks);
}
