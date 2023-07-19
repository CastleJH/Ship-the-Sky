// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_ShipUpgradeTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Ship.h"
#include "Building/BaseBuilding.h"
#include "Building/Shipyard.h"
#include "Tile/BaseTile.h"
#include "Tile/ResourceTile.h"

UBTService_ShipUpgradeTarget::UBTService_ShipUpgradeTarget()
{
	NodeName = TEXT("Ship Upgrade Target");

	Interval = 2.0f;
}

void UBTService_ShipUpgradeTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return;

	AShip* ShipToUpgrade = nullptr;
	EShipStat ShipStatToUpgrade = EShipStat::None;
	int32 MinConsumption = 99999999;

	for (auto Building : Commander->Buildings)
	{
		AShipyard* Shipyard = Cast<AShipyard>(Building);

		if (Shipyard && Shipyard->GetCurTile() && Shipyard->GetCurTile()->GetShip())
		{
			TPair<EShipStat, int32> Recommendation = Shipyard->GetCurTile()->GetShip()->GetStatUpgradeRecommendation();
			if (Recommendation.Key != EShipStat::None && Recommendation.Value < MinConsumption)
			{
				ShipToUpgrade = Shipyard->GetCurTile()->GetShip();
				ShipStatToUpgrade = Recommendation.Key;
				MinConsumption = Recommendation.Value;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipToUpgrade"), ShipToUpgrade);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("ShipStatToUpgrade"), (uint8)ShipStatToUpgrade);

}
