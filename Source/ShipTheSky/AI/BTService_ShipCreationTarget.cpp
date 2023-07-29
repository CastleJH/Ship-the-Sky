// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_ShipCreationTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/Commander.h"
#include "Ship/Ship.h"
#include "Building/BaseBuilding.h"
#include "Building/Shipyard.h"
#include "Enums.h"

UBTService_ShipCreationTarget::UBTService_ShipCreationTarget()
{
	NodeName = TEXT("Ship Creation Target");

	Interval = 10.0f;
}

void UBTService_ShipCreationTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACommander* Commander = Cast<ACommander>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Commander) return;

	int32 WoodConsume = Commander->GetShipCreationCost();
	for (EResourceType Type = EResourceType::WoodCloud; Type != EResourceType::Food;)
	{
		if (Commander->GetResource(Type) < WoodConsume)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipyardToCreateShip"), nullptr);
			return;
		}
		Type = StaticCast<EResourceType>((uint8)Type + 1);
	}

	int32 ShipyardCount = 0;
	AShipyard* Shipyard = nullptr;
	for (auto Building : Commander->Buildings)
	{
		if (Building->GetBuildingType() == EBuildingType::Shipyard)
		{
			ShipyardCount++;
			if (!Shipyard || FMath::RandRange(0, Commander->OwningIslands.Num()) == 0) Shipyard = Cast<AShipyard>(Building);
		}
	}

	if (Commander->Ships.Num() != ShipyardCount)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipyardToCreateShip"), Shipyard);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ShipyardToCreateShip"), nullptr);
	}
}
