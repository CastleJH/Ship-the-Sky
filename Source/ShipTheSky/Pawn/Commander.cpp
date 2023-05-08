// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/Commander.h"
#include "Tile/IslandTile.h"
#include "Unit/BaseUnit.h"
#include "Building/BaseBuilding.h"
#include "MapManager.h"

// Sets default values
ACommander::ACommander()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Resources.Init(0, (int32)EResourceType::End);
}

// Called when the game starts or when spawned
void ACommander::BeginPlay()
{
	Super::BeginPlay();
}

void ACommander::FillIslandWithUnit(int32 IslandID, ABaseUnit* Unit)
{
	AIslandTile* EmptyIslandTile = nullptr;
	TArray<AIslandTile*> IslandTiles = GetGameInstance()->GetSubsystem<UMapManager>()->GetSameIslandTiles(IslandID);
	for (auto Tile : IslandTiles)
	{
		if (Tile->GetUnit() == nullptr)
		{
			if (Unit->GetUnitType() != EUnitType::Warrior) 
			{
				if (Tile->GetIslandType() == EIslandTileType::Guardian) continue;
				if (Tile->GetIslandType() == EIslandTileType::Mine && Unit->GetUnitType() != EUnitType::Miner) continue;
				if (Tile->GetIslandType() == EIslandTileType::Forest && Unit->GetUnitType() != EUnitType::Woodcutter) continue;
				if (Tile->GetIslandType() == EIslandTileType::Farm && Unit->GetUnitType() != EUnitType::Farmer) continue;
			}
			EmptyIslandTile = Tile;
			break;
		}
	}

	if (EmptyIslandTile && Unit)
	{
		Unit->LocateToIslandTile(EmptyIslandTile);
	}
}

// Called every frame
void ACommander::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACommander::CreateUnit(int32 IslandID, EUnitType Type)
{
	ABaseUnit* Unit = nullptr;
	switch (Type)
	{
	case EUnitType::Woodcutter:
		Unit = GetWorld()->SpawnActor<ABaseUnit>(WoodcutterClass);
		break;
	case EUnitType::Miner:
		Unit = GetWorld()->SpawnActor<ABaseUnit>(MinerClass);
		break;
	case EUnitType::Farmer:
		Unit = GetWorld()->SpawnActor<ABaseUnit>(FarmerClass);
		break;
	case EUnitType::Warrior:
		Unit = GetWorld()->SpawnActor<ABaseUnit>(WarriorClass);
		break;
	}
	Unit->SetOwner(this);
	Unit->SetActorHiddenInGame(true);
	FillIslandWithUnit(IslandID, Unit);
}

void ACommander::ConstructBuilding(AIslandTile* Tile, EBuildingType Type)
{
	if (Tile && !Tile->GetBuilding())
	{
		ABaseBuilding* Building = nullptr;
		ABaseTile* MainTile = GetGameInstance()->GetSubsystem<UMapManager>()->GetSameIslandTiles(Tile->GetIslandID())[0];
		FVector Direction = MainTile->GetActorLocation() - Tile->GetActorLocation();
		switch (Type)
		{
		case EBuildingType::Barracks:
			Building = GetWorld()->SpawnActor<ABaseBuilding>(BarracksClass, Tile->GetActorLocation(), Direction.Rotation());
			break;
		}
		if (Building)
		{
			Tile->SetBuilding(Building);
			Building->SetCurTile(Tile);
		}
	}
}
