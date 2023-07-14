// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STSAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API ASTSAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASTSAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBehaviorTree> BTAsset;
};
