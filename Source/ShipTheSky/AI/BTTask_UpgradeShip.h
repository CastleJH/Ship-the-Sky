// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UpgradeShip.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UBTTask_UpgradeShip : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_UpgradeShip();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
