// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Shipyard.h"
#include "Ship.h"
#include "Tile/IslandTile.h"
#include "Pawn/Commander.h"

AShipyard::AShipyard()
{
	BuildingType = EBuildingType::Shipyard;

	TimeNeed = 10;
	bIsCreatingShip = false;
}

bool AShipyard::AddShipCreationToArray(int32 WoodCloud, int32 WoodStorm, int32 WoodSun, int32 WoodLightning, int32 WoodMeteor)
{
	if (WaitingShipArray.Num() < MaxWaitingShip)
	{
		TArray<int32> Resources = { WoodCloud, WoodStorm, WoodSun, WoodLightning, WoodMeteor };
		WaitingShipArray.Add(Resources);
		StartShipCreation();
		return true;
	}
	return false;
}

int32 AShipyard::GetWaitingShipResourceByIndex(int32 Index, EResourceType ResourceType) const
{
	if (WaitingShipArray.IsValidIndex(Index))
	{
		if (ResourceType < EResourceType::WoodCloud || EResourceType::WoodMeteor < ResourceType) return -1;
		return WaitingShipArray[Index][(int32)ResourceType - (int32)EResourceType::WoodCloud];
	}
	else
	{
		return -1;
	}
}

void AShipyard::CancelWaitingShipByIndex(int32 Index)
{
	if (WaitingShipArray.IsValidIndex(Index))
	{
		if (Index == 0) ResetProgress();
		WaitingShipArray.RemoveAt(Index);
		if (WaitingShipArray.IsEmpty()) bIsCreatingShip = false;
	}
}

float AShipyard::GetProgressRate() const
{
	return (float)Progress / (float)TimeNeed;
}

void AShipyard::IncreaseProgress()
{
	if (!bIsCreatingShip) return;

	Super::IncreaseProgress();

	if (Progress == TimeNeed + 1)
	{
		FinishShipCreation();
		StartShipCreation();
	}
}

void AShipyard::StartShipCreation()
{
	if (bIsCreatingShip) return;
	if (!WaitingShipArray.IsEmpty())
	{
		ResetProgress();
		bIsCreatingShip = true;
	}
}

void AShipyard::FinishShipCreation()
{
	if (!WaitingShipArray.IsEmpty())
	{
		ACommander* OwnerCommander = Cast<ACommander>(GetOwner());
		CreateUnit(WaitingShipArray[0]);
		WaitingShipArray.RemoveAt(0);
		bIsCreatingShip = false;
		ResetProgress();
	}
}

void AShipyard::CreateUnit(TArray<int32>& Resources)
{
	ACommander* Commander = nullptr;
	if (GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Owner of Barracks"));
		return;
	}
	else
	{
		Commander = Cast<ACommander>(GetOwner());
		if (Commander == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("No Owner of Barracks"));
			return;
		}
	}

	AShip* Ship = GetWorld()->SpawnActor<AShip>(Commander->ShipClass);
	Ship->SetOwner(this);
	Ship->SetActorHiddenInGame(true);

	if (CurTile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong Location"));
		return;
	}

	Ship->LocateOnTile(CurTile);
}
