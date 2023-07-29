// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_UpgradeUnit.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Unit/BaseUnit.h"
#include "Tile/IslandTile.h"
#include "Tile/GuardianTile.h"
#include "Ship/Ship.h"

UBTTask_UpgradeUnit::UBTTask_UpgradeUnit()
{
}

EBTNodeResult::Type UBTTask_UpgradeUnit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;


	ABaseUnit* Unit = Cast<ABaseUnit>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("UnitToUpgrade")));
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("UnitToUpgrade"), nullptr);
	if (!Unit) return EBTNodeResult::Failed;

	if (!Unit->GetCurIslandTile())
	{
		if (!Unit->GetCurShip())
		{
			UE_LOG(LogTemp, Error, TEXT("No CurIslandTile And No CurShip..., %s"), *Unit->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No CurIslandTile But CurShip... Maybe in Ship %s Now. : %s"), *(Unit->GetCurShip()->GetName()), *Unit->GetName());
		}
		return  EBTNodeResult::Failed;
	}
	
	EUnitStat StatToUpgrade = StaticCast<EUnitStat>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("UnitStatToUpgrade")));

	float TargetResourceAmount = 200.0f;
	switch (StatToUpgrade)
	{
	case EUnitStat::HP:
		//TargetResourceAmount = Commander->GetResource(EResourceType::StoneCloud) / 2.0f;
		while (true)
		{
			if (!Unit->UpgradeHP()) break;
			if (Commander->GetResource(EResourceType::StoneCloud) < TargetResourceAmount) break;
		}
		return EBTNodeResult::Succeeded;
	case EUnitStat::Attack:
		//TargetResourceAmount = Commander->GetResource(EResourceType::StoneStorm) / 2.0f;
		while (true)
		{
			if (!Unit->UpgradeAttack()) break;
			if (Commander->GetResource(EResourceType::StoneStorm) < TargetResourceAmount) break;
		}
		return EBTNodeResult::Succeeded;
	case EUnitStat::FoodConsume:
		//TargetResourceAmount = Commander->GetResource(EResourceType::StoneSun) / 2.0f;
		while (true)
		{
			if (!Unit->UpgradeFoodConsume()) break;
			if (Commander->GetResource(EResourceType::StoneSun) < TargetResourceAmount) break;
		}
		return EBTNodeResult::Succeeded;
	case EUnitStat::Efficiency:
		//TargetResourceAmount = Commander->GetResource(EResourceType::StoneLightning) / 2.0f;
		while (true)
		{
			if (!Unit->UpgradeEfficiency()) break;
			if (Commander->GetResource(EResourceType::StoneLightning) < TargetResourceAmount) break;
		}
		return EBTNodeResult::Succeeded;
	case EUnitStat::All:
		//TargetResourceAmount = Commander->GetResource(EResourceType::StoneMeteor) / 2.0f;
		while (true)
		{
			if (!Unit->UpgradeAll()) break;
			if (Commander->GetResource(EResourceType::StoneMeteor) < TargetResourceAmount) break;
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
