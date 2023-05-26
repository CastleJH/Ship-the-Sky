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

bool AShipyard::AddShipCreationToArray()
{
	if (WaitingShipArray.Num() < MaxWaitingShip)
	{
		FString ShipName = FString::Printf(TEXT("Airship %d"), FMath::RandRange(100, 999));
		WaitingShipArray.Add(ShipName);
		StartShipCreation();
		return true;
	}
	return false;
}

FString AShipyard::GetWaitingShipNameByIndex(int32 Index) const
{
	if (WaitingShipArray.IsValidIndex(Index))
	{
		return WaitingShipArray[Index];
	}
	else
	{
		return FString(TEXT(" "));
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
		if (FinishShipCreation()) 
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

bool AShipyard::FinishShipCreation()
{
	if (!WaitingShipArray.IsEmpty())
	{
		ACommander* OwnerCommander = Cast<ACommander>(GetOwner());
		bool CreationSuccess = CreateShip(WaitingShipArray[0]);
		if (!CreationSuccess)
		{
			Progress = TimeNeed;
			return false;
		}

		WaitingShipArray.RemoveAt(0);
		bIsCreatingShip = false;
		ResetProgress();

		return true;
	}
	return false;
}

bool AShipyard::CreateShip(FString ShipName)
{
	if (CurTile->GetShip() != nullptr) return false;

	ACommander* Commander = nullptr;
	if (GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Owner of Barracks"));
		return false;
	}
	else
	{
		Commander = Cast<ACommander>(GetOwner());
		if (Commander == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("No Owner of Barracks"));
			return false;
		}
	}

	AShip* Ship = GetWorld()->SpawnActor<AShip>(Commander->ShipClass);
	Ship->SetOwner(GetOwner());
	Ship->SetActorHiddenInGame(true);

	if (CurTile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong Location"));
		return false;
	}

	Ship->TryLocateOnTile(CurTile);
	return true;
}
