// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SelectTileToBuild.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UBTService_SelectTileToBuild : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_SelectTileToBuild();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
