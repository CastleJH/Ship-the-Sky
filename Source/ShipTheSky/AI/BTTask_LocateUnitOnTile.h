// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LocateUnitOnTile.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UBTTask_LocateUnitOnTile : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_LocateUnitOnTile();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
