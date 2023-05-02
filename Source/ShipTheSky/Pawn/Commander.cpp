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

}

// Called when the game starts or when spawned
void ACommander::BeginPlay()
{
	Super::BeginPlay();
	
	GetGameInstance()->GetSubsystem<UMapManager>()->GenerateMap(75);
}

void ACommander::FillIslandWithUnit(int32 IslandID, ABaseUnit* Unit)
{
	AIslandTile* EmptyIslandTile = nullptr;
	TArray<AIslandTile*> IslandTiles = GetGameInstance()->GetSubsystem<UMapManager>()->GetSameIslandTiles(IslandID);
	for (auto Tile : IslandTiles)
	{
		if (Tile->GetUnit() == nullptr)
		{
			EmptyIslandTile = Tile;
			break;
		}
	}

	if (EmptyIslandTile && Unit)
	{
		ABaseTile* MainTile = GetGameInstance()->GetSubsystem<UMapManager>()->GetSameIslandTiles(IslandID)[0];
		FVector Direction = MainTile->GetActorLocation() - EmptyIslandTile->GetActorLocation();

		Unit->SetActorLocation(EmptyIslandTile->GetActorLocation());
		Unit->SetActorRotation(Direction.Rotation());
		Unit->SetActorHiddenInGame(false);
		
		EmptyIslandTile->SetUnit(Unit);
		Unit->SetCurTile(EmptyIslandTile);
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
