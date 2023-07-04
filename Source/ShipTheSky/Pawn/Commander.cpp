// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/Commander.h"
#include "Tile/IslandTile.h"
#include "Tile/ResourceTile.h"
#include "Tile/GuardianTile.h"
#include "Unit/BaseUnit.h"
#include "Building/Barracks.h"
#include "Building/Shipyard.h"
#include "Ship.h"
#include "MapManager.h"

// Sets default values
ACommander::ACommander()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Resources.Init(0, (int32)EResourceType::None);
	TargetIslandTile = nullptr;
	TargetTile = nullptr;
}

void ACommander::ConstructBuilding(AResourceTile* Tile, EBuildingType Type)
{
	UE_LOG(LogTemp, Warning, TEXT("HERE"));
	if (Tile && !Tile->GetBuilding())
	{
		ABaseBuilding* Building = nullptr;
		ABaseTile* MainTile = Tile->GetGuardianTile();
		if (MainTile == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Nullptr Island"));
			return;
		}
		FVector Direction = MainTile->GetActorLocation() - Tile->GetActorLocation();
		switch (Type)
		{
		case EBuildingType::Barracks:
			Building = GetWorld()->SpawnActor<ABaseBuilding>(BarracksClass, Tile->GetActorLocation(), Direction.Rotation());
			break;
		case EBuildingType::Shipyard:
			Building = GetWorld()->SpawnActor<ABaseBuilding>(ShipyardClass, Tile->GetActorLocation(), Direction.Rotation());
			break;
		}
		if (Building)
		{
			Building->SetOwner(this);
			Tile->SetBuilding(Building);
			Building->SetCurTile(Tile);
		}
	}
}

bool ACommander::TryCreateUnit(ABarracks* Barracks, EUnitType Type)
{
	if (Barracks == nullptr) return false;
	Barracks->AddUnitCreationToArray(Type);
	return true;
}

bool ACommander::TryCreateShip(AShipyard* Shipyard)
{
	if (Shipyard == nullptr) return false;
	Shipyard->AddShipCreationToArray();
	return true;
}

bool ACommander::TryEmbarkUnit(AShip* Ship, ABaseUnit* Unit)
{
	if (Ship == nullptr) return false;
	if (Unit == nullptr) return false;
	return Unit->Embark(Ship);
}

bool ACommander::TryDisembarkUnit(ABaseUnit* Unit)
{
	if (Unit == nullptr) return false;
	return Unit->Disembark();
}

bool ACommander::TryDepartShip(AShip* Ship)
{
	if (Ship == nullptr) return false;
	Ship->FollowPath();
	return true;
}

bool ACommander::TryStopShip(AShip* Ship)
{
	Ship->EmptyPath();
	return true;
}

void ACommander::FillIslandWithUnit(int32 IslandID, ABaseUnit* Unit)
{
	AResourceTile* EmptyIslandTile = nullptr;
	TArray<AIslandTile*> IslandTiles;
	GetGameInstance()->GetSubsystem<UMapManager>()->GetSameIslandTiles(IslandID, IslandTiles);
	for (auto Tile : IslandTiles)
	{
		if (Tile->GetIslandType() != EIslandTileType::Guardian)
		{
			if (Cast<AResourceTile>(Tile)->GetUnit() == nullptr)
			{
				if (Unit->GetUnitType() != EUnitType::Warrior)
				{
					if (Tile->GetIslandType() == EIslandTileType::Guardian) continue;
					if (Tile->GetIslandType() == EIslandTileType::Mine && Unit->GetUnitType() != EUnitType::Miner) continue;
					if (Tile->GetIslandType() == EIslandTileType::Forest && Unit->GetUnitType() != EUnitType::Woodcutter) continue;
					if (Tile->GetIslandType() == EIslandTileType::Farm && Unit->GetUnitType() != EUnitType::Farmer) continue;
				}
				EmptyIslandTile = Cast<AResourceTile>(Tile);
				break;
			}
		}
	}

	if (EmptyIslandTile && Unit)
	{
		Unit->LocateToResourceTile(EmptyIslandTile);
	}
}

void ACommander::SetTargetTile(ABaseTile* NewTile)
{
	TargetTile = NewTile;
	TargetIslandTile = Cast<AIslandTile>(NewTile);
	TargetResourceTile = Cast<AResourceTile>(NewTile);
	TargetGuardianTile = Cast<AResourceTile>(NewTile);
}
