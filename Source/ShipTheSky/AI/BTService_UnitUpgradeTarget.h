// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UnitUpgradeTarget.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UBTService_UnitUpgradeTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UnitUpgradeTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
