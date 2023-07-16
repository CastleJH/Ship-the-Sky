// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_GuardianUpgradeTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Enums.h"
#include "Tile/GuardianTile.h"
#include "Guardian/Guardian.h"

UBTService_GuardianUpgradeTarget::UBTService_GuardianUpgradeTarget()
{
	NodeName = TEXT("Guardian Upgrade Target");

	Interval = 2.0f;
}

void UBTService_GuardianUpgradeTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return;

	AGuardian* Guardian = nullptr;
	EGuardianStat Stat = EGuardianStat::None;
	int32 MinConsumption = 99999999;

	for (auto GuardianTile : Commander->OwningIslands)
	{
		if (GuardianTile->IsBuildingTypeBuilt(EBuildingType::Sanctuary))
		{
			TPair<EGuardianStat, int32> Recommendation = GuardianTile->Guardian->GetStatUpgradeRecommendation();
			if (Recommendation.Key != EGuardianStat::None && Recommendation.Value < MinConsumption)
			{
				Stat = Recommendation.Key;
				MinConsumption = Recommendation.Value;
				Guardian = GuardianTile->Guardian;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("GuardianStatToUpgrade"), (uint8)Stat);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("GuardianToUpgrade"), Guardian);

}
