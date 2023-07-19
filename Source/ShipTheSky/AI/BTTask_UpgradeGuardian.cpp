// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_UpgradeGuardian.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Guardian/Guardian.h"
UBTTask_UpgradeGuardian::UBTTask_UpgradeGuardian()
{
}

EBTNodeResult::Type UBTTask_UpgradeGuardian::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return EBTNodeResult::Failed;

	AGuardian* Guardian = Cast<AGuardian>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("GuardianToUpgrade")));
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("GuardianToUpgrade"), nullptr);
	if (!Guardian) return EBTNodeResult::Failed;

	EGuardianStat StatToUpgrade = StaticCast<EGuardianStat>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("GuardianStatToUpgrade")));

	float TargetResourceAmount = 200.0f;
	switch (StatToUpgrade)
	{
	case EGuardianStat::HPCloud:
		//TargetResourceAmount = Commander->GetResource(EResourceType::StoneCloud) / 2.0f;
		while (true)
		{
			if (!Guardian->UpgradeHPWithCloud()) break;
			if (Commander->GetResource(EResourceType::StoneCloud) < TargetResourceAmount) break;
		}
		return EBTNodeResult::Succeeded;
	case EGuardianStat::AttackStorm:
		//TargetResourceAmount = Commander->GetResource(EResourceType::StoneStorm) / 2.0f;
		while (true)
		{
			if (!Guardian->UpgradeAttackWithStorm()) break;
			if (Commander->GetResource(EResourceType::StoneStorm) < TargetResourceAmount) break;
		}
		return EBTNodeResult::Succeeded;
	case EGuardianStat::HPSun:
		//TargetResourceAmount = Commander->GetResource(EResourceType::StoneSun) / 2.0f;
		while (true)
		{
			if (!Guardian->UpgradeHPWithSun()) break;
			if (Commander->GetResource(EResourceType::StoneSun) < TargetResourceAmount) break;
		}
		return EBTNodeResult::Succeeded;
	case EGuardianStat::AttackLightning:
		//TargetResourceAmount = Commander->GetResource(EResourceType::StoneLightning) / 2.0f;
		while (true)
		{
			if (!Guardian->UpgradeAttackWithLightning()) break;
			if (Commander->GetResource(EResourceType::StoneLightning) < TargetResourceAmount) break;
		}
		return EBTNodeResult::Succeeded;
	case EGuardianStat::Score:
		//TargetResourceAmount = Commander->GetResource(EResourceType::StoneMeteor) / 2.0f;
		while (true)
		{
			if (!Guardian->UpgradeScorePower()) break;
			if (Commander->GetResource(EResourceType::StoneMeteor) < TargetResourceAmount) break;
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
