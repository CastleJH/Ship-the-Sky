// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ASTSAIController::ASTSAIController()
{
}

void ASTSAIController::RunAI()
{
	//Blackboard는 AIController에서 기본 멤버변수로 제공한다.
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void ASTSAIController::StopAI()
{
	//BrainComponent는 AIController에서 기본 멤버변수로 제공한다.
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ASTSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
