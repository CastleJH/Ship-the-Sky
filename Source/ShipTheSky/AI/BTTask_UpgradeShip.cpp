// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_UpgradeShip.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Ship.h"

UBTTask_UpgradeShip::UBTTask_UpgradeShip()
{
}

EBTNodeResult::Type UBTTask_UpgradeShip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;

	AShip* ShipToUpgrade = Cast<AShip>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("ShipToUpgrade")));
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipToUpgrade"), nullptr);
	EShipStat ShipStatToUpgrade = StaticCast<EShipStat>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("ShipStatToUpgrade")));
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("ShipStatToUpgrade"), (uint8)EShipStat::None);

	if (!ShipToUpgrade || ShipStatToUpgrade == EShipStat::None) return EBTNodeResult::Failed;


	float TargetResourceAmount = Commander->GetShipCreationCost() + 200.0f;

	if ((uint8)ShipStatToUpgrade < (uint8)EShipStat::FlightPowerCloud)
	{
		//TargetResourceAmount = Commander->GetResource(StaticCast<EResourceType>((uint8)ShipStatToUpgrade + 5)) / 2.0f;
		while (true)
		{
			if (!ShipToUpgrade->UpgradeResistance(StaticCast<ETileType>((uint8)ShipStatToUpgrade + 1))) break;
			if (Commander->GetResource(StaticCast<EResourceType>((uint8)ShipStatToUpgrade + 5)) < TargetResourceAmount) break;
		}
		return EBTNodeResult::Succeeded;
	}
	else
	{
		switch (ShipStatToUpgrade)
		{
		case EShipStat::FlightPowerCloud:
			//TargetResourceAmount = Commander->GetResource(EResourceType::WoodCloud) / 2.0f;
			while (true)
			{
				if (!ShipToUpgrade->UpgradeFlightPowerWithCloud()) break;
				if (Commander->GetResource(EResourceType::WoodCloud) < TargetResourceAmount) break;
			}
			return EBTNodeResult::Succeeded;
		case EShipStat::DurabilityStorm:
			//TargetResourceAmount = Commander->GetResource(EResourceType::WoodStorm) / 2.0f;
			while (true)
			{
				if (!ShipToUpgrade->UpgradeDurabilityWithStorm()) break;
				if (Commander->GetResource(EResourceType::WoodStorm) < TargetResourceAmount) break;
			}
			return EBTNodeResult::Succeeded;
		case EShipStat::FlightPowerSun:
			//TargetResourceAmount = Commander->GetResource(EResourceType::WoodSun) / 2.0f;
			while (true)
			{
				if (!ShipToUpgrade->UpgradeFlightPowerWithSun()) break;
				if (Commander->GetResource(EResourceType::WoodSun) < TargetResourceAmount) break;
			}
			return EBTNodeResult::Succeeded;
		case EShipStat::DurabilityLightning:
			//TargetResourceAmount = Commander->GetResource(EResourceType::WoodLightning) / 2.0f;
			while (true)
			{
				if (!ShipToUpgrade->UpgradeDurabilityWithLightning()) break;
				if (Commander->GetResource(EResourceType::WoodLightning) < TargetResourceAmount) break;
			}
			return EBTNodeResult::Succeeded;
		case EShipStat::Capacity:
			//TargetResourceAmount = Commander->GetResource(EResourceType::WoodMeteor) / 2.0f;
			while (true)
			{
				if (!ShipToUpgrade->UpgradeCapacity()) break;
				if (Commander->GetResource(EResourceType::WoodMeteor) < TargetResourceAmount) break;
			}
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
