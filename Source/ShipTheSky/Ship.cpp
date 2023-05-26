// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "Tile/BaseTile.h"
#include "Tile/IslandTile.h"
#include "MapManager.h"

// Sets default values
AShip::AShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->CastShadow = false;
}

bool AShip::TryLocateOnTile(ABaseTile* Tile)
{
	if (Tile == nullptr) return false;
	if (Tile->GetShip() != nullptr) return false;

	if (CurTile != nullptr) CurTile->SetShip(nullptr);

	SetActorLocation(Tile->GetActorLocation() + FVector(0.0f, 0.0f, 250.0f));
	CurTile = Tile;
	Tile->SetShip(this);
	SetActorHiddenInGame(false);
	return true;
}

void AShip::InitializeStatWithResources(int32 WoodCloud, int32 WoodStorm, int32 WoodSun, int32 WoodLightning, int32 WoodMeteor)
{
	Durability = 100;

	CloudResistance = WoodCloud;
	StormResistance = WoodStorm;
	SunResistance = WoodSun;
	LightningResistance = WoodLightning;
	MeteorResistance = WoodMeteor;

	FlightPower = 5;

	UnitCapacity = 4;
}


