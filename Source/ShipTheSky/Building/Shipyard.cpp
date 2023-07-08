// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Shipyard.h"
#include "Ship.h"
#include "Tile/ResourceTile.h"
#include "Pawn/Commander.h"

AShipyard::AShipyard()
{
	BuildingType = EBuildingType::Shipyard;

	TimeNeed = 3;
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
		return FString(TEXT(""));
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
	return FMath::Min(1.0f, (float)Progress / (float)TimeNeed);
}

void AShipyard::IncreaseProgress()
{
	if (!bIsCreatingShip) return;

	Super::IncreaseProgress();
	
	if (Progress >= TimeNeed + 1)
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

	if (OwnerCommander == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Owner of Barracks"));
		return false;
	}

	AShip* Ship = CurTile->GetIslandOwner()->SpawnShipToGame();
	Ship->SetActorHiddenInGame(true);
	Ship->SetShipName(WaitingShipArray[0]);

	if (CurTile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong Location"));
		Ship->Destroy();
		return false;
	}
	if (Ship->TryLocateOnTile(CurTile, true)) return true;
	else
	{
		Ship->Destroy();
		return false;
	}
}
