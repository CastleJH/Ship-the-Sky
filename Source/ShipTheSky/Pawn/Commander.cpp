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
#include "Building/BaseBuilding.h"
#include "Enums.h"

// Sets default values
ACommander::ACommander()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Resources.Init(0, (int32)EResourceType::None);
	TargetIslandTile = nullptr;
	TargetTile = nullptr;

	OutlineColorIndex = 1;
}

bool ACommander::TryConstructBuilding(AResourceTile* Tile, EBuildingType Type)
{
	if (Tile->GetIslandOwner() != this) return false;
	UE_LOG(LogTemp, Warning, TEXT("HERE"));
	if (Tile && !Tile->GetBuilding())
	{
		ABaseBuilding* Building = nullptr;
		ABaseTile* MainTile = Tile->GetGuardianTile();
		if (MainTile == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Nullptr Island"));
			return false;
		}
		FVector Direction = MainTile->GetActorLocation() - Tile->GetActorLocation();
		Building = SpawnBuildingToGame(Type, Tile->GetActorLocation(), Direction.Rotation());
		Tile->SetBuilding(Building);
		Building->SetCurTile(Tile);
	}
	return true;
}

bool ACommander::TryCreateUnit(ABarracks* Barracks, EUnitType Type)
{
	int32 FoodConsume = GetUnitCreationCost();
	if (GetResource(EResourceType::Food) < FoodConsume) return false;
	if (Barracks == nullptr) return false;
	if (Barracks->AddUnitCreationToArray(Type)) SetResource(GetResource(EResourceType::Food) - FoodConsume, EResourceType::Food);
	return true;
}

bool ACommander::TryCreateShip(AShipyard* Shipyard)
{
	int32 WoodConsume = GetShipCreationCost();
	for (int32 Type = (int32)EResourceType::WoodCloud; Type != (int32)EResourceType::WoodMeteor; Type++)
	{
		if (Resources[Type] < WoodConsume) return false;
	}
	if (Shipyard == nullptr) return false;
	if (Shipyard->AddShipCreationToArray())
	{
		for (int32 Type = (int32)EResourceType::WoodCloud; Type != (int32)EResourceType::WoodMeteor; Type++)
		{
			Resources[Type] -= WoodConsume;
		}
	}
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
		Unit->LocateOnIslandTile(EmptyIslandTile, true);
	}
	else
	{
		Unit->LocateOnIslandTile(IslandTiles[0], true);
	}
}

void ACommander::SetTargetTile(ABaseTile* NewTile)
{
	TargetTile = NewTile;
	TargetIslandTile = Cast<AIslandTile>(NewTile);
	TargetResourceTile = Cast<AResourceTile>(NewTile);
	TargetGuardianTile = Cast<AGuardianTile>(NewTile);
}

ABaseUnit* ACommander::SpawnUnitToGame(EUnitType Type)
{
	TSubclassOf<ABaseUnit> UnitClass;
	switch (Type)
	{
	case EUnitType::Miner:
		UnitClass = MinerClass;
		break;
	case EUnitType::Woodcutter:
		UnitClass = WoodcutterClass;
		break;
	case EUnitType::Farmer:
		UnitClass = FarmerClass;
		break;
	case EUnitType::Warrior:
		UnitClass = WarriorClass;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Wrong Type"));
		break;
	}
	ABaseUnit* Unit = GetWorld()->SpawnActor<ABaseUnit>(UnitClass);
	Unit->SetOwnerCommander(this);
	Units.Add(Unit);
	return Unit;
}

void ACommander::DestroyUnitFromGame(ABaseUnit* Unit)
{
	if (Unit->GetCurShip()) Unit->GetCurShip()->RemoveUnit(Unit);
	if (Unit->GetCurIslandTile())
	{
		Unit->GetCurIslandTile()->GetGuardianTile()->RemoveUnitFromThisIsland(Unit);
	}
	Units.Remove(Unit);
	Unit->Destroy();
}

ABaseBuilding* ACommander::SpawnBuildingToGame(EBuildingType Type, FVector Location, FRotator Rotation)
{
	TSubclassOf<ABaseBuilding> BuildingClass;
	switch (Type)
	{
	case EBuildingType::Barracks:
		BuildingClass = BarracksClass;
		break;
	case EBuildingType::Shipyard:
		BuildingClass = ShipyardClass;
		break;
	case EBuildingType::Portal:
		BuildingClass = PortalClass;
		break;
	case EBuildingType::Sanctuary:
		BuildingClass = SanctuaryClass;
		break;
	}
	ABaseBuilding* Building = GetWorld()->SpawnActor<ABaseBuilding>(BuildingClass, Location, Rotation);
	Building->SetOwnerCommander(this);
	Buildings.Add(Building);
	return Building;
}

void ACommander::DestroyBuildingFromGame(ABaseBuilding* Building)
{
	if (Building->GetCurTile())
	{
		Building->GetCurTile()->SetBuilding(nullptr);
	}
	Buildings.Remove(Building);
	Building->Destroy();
}

AShip* ACommander::SpawnShipToGame()
{
	AShip* Ship = GetWorld()->SpawnActor<AShip>(ShipClass);
	Ship->SetOwnerCommander(this);
	Ships.Add(Ship);
	return Ship;
}

void ACommander::DestroyShipFromGame(AShip* Ship)
{
	if (TargetShip == Ship) TargetShip = nullptr;
	Ship->GetCurTile()->SetShip(nullptr);
	Ships.Remove(Ship);
	Ship->Destroy();
}
