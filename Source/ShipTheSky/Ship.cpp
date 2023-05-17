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

void AShip::LocateOnTile(ABaseTile* Tile)
{
	if (Tile == nullptr) return;
	/*if (Tile->GetTileType() == ETileType::Island)
	{
		TArray<ABaseTile*> AdjacentTiles;
		GetGameInstance()->GetSubsystem<UMapManager>()->GetAdjacentTiles(Tile, AdjacentTiles);

		while (true)
		{
			UE_LOG(LogTemp, Warning, TEXT("%d"), AdjacentTiles.Num());
			int32 Index = FMath::RandRange(0, AdjacentTiles.Num() - 1);
			if (AdjacentTiles[Index] != nullptr && AdjacentTiles[Index]->GetTileType() != ETileType::Island)
			{
				FVector NewLocation = AdjacentTiles[Index]->GetActorLocation();
				FRotator NewRotation = (AdjacentTiles[Index]->GetActorLocation() - Tile->GetActorLocation()).Rotation();
				SetActorLocation(NewLocation + FVector(0.0f, 0.0f, 30.0f));
				SetActorRotation(NewRotation);
				CurTile = AdjacentTiles[Index];
				break;
			}
		}
	}
	else
	{
		SetActorLocation(Tile->GetActorLocation() + FVector(0.0f, 0.0f, 30.0f));
		CurTile = Tile;
	}*/
	SetActorLocation(Tile->GetActorLocation() + FVector(0.0f, 0.0f, 250.0f));
	CurTile = Tile;
	SetActorHiddenInGame(false);
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


