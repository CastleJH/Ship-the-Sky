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

FString ABarracks::GetCreatingUnitName() const
{
	FString ReturnString = TEXT("기본");
	if (WaitingUnitCreation.IsEmpty()) return ReturnString;

	switch (*WaitingUnitCreation.Peek())
	{
	case EUnitType::Miner:
		ReturnString = TEXT("광부");
		break;
	case EUnitType::Woodcutter:
		ReturnString = TEXT("나무꾼");
		break;
	case EUnitType::Farmer:
		ReturnString = TEXT("농부");
		break;
	case EUnitType::Warrior:
		ReturnString = TEXT("전사");
		break;
	default:
		break;
	}
	return ReturnString;
}

void ABarracks::IncreaseProgress()
{
	if (!bIsCreatingUnit) return;

	Super::IncreaseProgress();

	if (Progress == TimeNeed + 1)
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
		ResetProgress();
	}
}
