// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServiceTest2.h"

UBTServiceTest2::UBTServiceTest2()
{
	NodeName = TEXT("Test2");

	Interval = 0.5f;
}

void UBTServiceTest2::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UE_LOG(LogTemp, Warning, TEXT("Test2 Tick"));
}
