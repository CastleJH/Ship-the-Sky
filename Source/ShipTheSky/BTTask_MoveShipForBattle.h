// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveShipForBattle.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UBTTask_MoveShipForBattle : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveShipForBattle();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
