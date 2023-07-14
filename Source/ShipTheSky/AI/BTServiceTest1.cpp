// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServiceTest1.h"

UBTServiceTest1::UBTServiceTest1()
{
	NodeName = TEXT("Test1");

	Interval = 1.0f;
}

void UBTServiceTest1::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UE_LOG(LogTemp, Warning, TEXT("Test1 Tick"));
}
