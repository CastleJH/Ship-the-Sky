// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Barracks.h"
#include "Unit/BaseUnit.h"
#include "Tile/IslandTile.h"
#include "Pawn/Commander.h"
#include "STSGameState.h"

ABarracks::ABarracks()
{
	BuildingType = EBuildingType::Barracks;
	TimeNeed = 10;
	bIsCreatingUnit = false;
}

void ABarracks::AddUnitCreationToQueue(EUnitType UnitType)
{
	WaitingUnitCreation.Enqueue(UnitType);
	StartUnitCreation();
}

float ABarracks::GetProgressRate() const
{
	return (float)Progress / (float)TimeNeed;
}

void ABarracks::IncreaseProgress()
{
	if (!bIsCreatingUnit) return;

	Super::IncreaseProgress();

	if (Progress == TimeNeed)
	{
		FinishUnitCreation();
		StartUnitCreation();
	}
}

void ABarracks::StartUnitCreation()
{
	if (bIsCreatingUnit) return;
	if (!WaitingUnitCreation.IsEmpty())
	{
		ResetProgress();
		bIsCreatingUnit = true;
	}
}

void ABarracks::FinishUnitCreation()
{
	if (!WaitingUnitCreation.IsEmpty())
	{
		ACommander* OwnerCommander = Cast<ACommander>(GetOwner());
		EUnitType NewUnitType;
		WaitingUnitCreation.Dequeue(NewUnitType);
		OwnerCommander->CreateUnit(CurTile->GetIslandID(), NewUnitType);
		bIsCreatingUnit = false;
	}
}
