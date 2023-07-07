// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Barracks.h"
#include "Unit/BaseUnit.h"
#include "Tile/IslandTile.h"
#include "Tile/GuardianTile.h"
#include "Pawn/Commander.h"
#include "STSGameState.h"
#include "MapManager.h"

ABarracks::ABarracks()
{
	BuildingType = EBuildingType::Barracks;
	TimeNeed = 3;
	bIsCreatingUnit = false;
}

bool ABarracks::AddUnitCreationToArray(EUnitType UnitType)
{
	if (WaitingUnitArray.Num() < MaxWaitingUnit)
	{
		WaitingUnitArray.Add(UnitType);
		StartUnitCreation();
		return true;
	}
	return false;
}

EUnitType ABarracks::GetWaitingUnitByIndex(int32 Index) const
{
	if (WaitingUnitArray.IsValidIndex(Index))
	{
		return WaitingUnitArray[Index];
	}
	else
	{
		return EUnitType::None;
	}
}

void ABarracks::CancelWaitingUnitByIndex(int32 Index)
{
	if (WaitingUnitArray.IsValidIndex(Index))
	{
		if (Index == 0) ResetProgress();
		WaitingUnitArray.RemoveAt(Index);
		if (WaitingUnitArray.IsEmpty()) bIsCreatingUnit = false;
	}
}

float ABarracks::GetProgressRate() const
{
	return (float)Progress / (float)TimeNeed;
}

FString ABarracks::GetCreatingUnitName() const
{
	FString ReturnString = TEXT("기본");
	if (WaitingUnitArray.IsEmpty()) return ReturnString;

	switch (WaitingUnitArray[0])
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
	if (!WaitingUnitArray.IsEmpty())
	{
		ResetProgress();
		bIsCreatingUnit = true;
	}
}

void ABarracks::FinishUnitCreation()
{
	if (!WaitingUnitArray.IsEmpty())
	{
		EUnitType NewUnitType = WaitingUnitArray[0];
		WaitingUnitArray.RemoveAt(0);
		ABaseUnit* CreatedUnit = CreateUnit(NewUnitType);
		bIsCreatingUnit = false;
		ResetProgress();
	}
}

ABaseUnit* ABarracks::CreateUnit(EUnitType Type)
{
	if (OwnerCommander == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Owner of Barracks"));
		return nullptr;
	}

	ABaseUnit* Unit = nullptr;
	switch (Type)
	{
	case EUnitType::Woodcutter:
		Unit = GetWorld()->SpawnActor<ABaseUnit>(OwnerCommander->WoodcutterClass);
		break;
	case EUnitType::Miner:
		Unit = GetWorld()->SpawnActor<ABaseUnit>(OwnerCommander->MinerClass);
		break;
	case EUnitType::Farmer:
		Unit = GetWorld()->SpawnActor<ABaseUnit>(OwnerCommander->FarmerClass);
		break;
	case EUnitType::Warrior:
		Unit = GetWorld()->SpawnActor<ABaseUnit>(OwnerCommander->WarriorClass);
		break;
	}
	Unit->SetOwnerCommander(OwnerCommander);
	Unit->SetActorHiddenInGame(true);

	if (CurTile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong Location"));
		return nullptr;
	}
	CurTile->GetGuardianTile()->AddUnitOnThisIsland(Unit);
	OwnerCommander->FillIslandWithUnit(CurTile->GetIslandID(), Unit);
	return Unit;
}
