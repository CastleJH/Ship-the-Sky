// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UpgradeGuardian.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UBTTask_UpgradeGuardian : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_UpgradeGuardian();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
