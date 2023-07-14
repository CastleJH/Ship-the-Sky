// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ConstructionTarget.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UBTService_ConstructionTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_ConstructionTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
