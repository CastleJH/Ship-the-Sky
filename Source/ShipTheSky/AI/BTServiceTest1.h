// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceTest1.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UBTServiceTest1 : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTServiceTest1();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
