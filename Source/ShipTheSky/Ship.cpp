// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "Tile/BaseTile.h"
#include "Tile/IslandTile.h"
#include "MapManager.h"
#include "Kismet/KismetMathLibrary.h" 

// Sets default values
AShip::AShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->CastShadow = false;

	Durability = 100;
	FlightPower = 3;
	UnitCapacity = 4;

	CloudResistance = FMath::RandRange(0, 10);
	StormResistance = FMath::RandRange(0, 10);
	SunResistance = FMath::RandRange(0, 10);
	LightningResistance = FMath::RandRange(0, 10);
	MeteorResistance = FMath::RandRange(0, 10);
}

void AShip::Tick(float DeltaTime)
{
	FVector LerpedPosition = FMath::Lerp(GetActorLocation(), CurTile->GetActorLocation() + FVector(0.0f, 0.0f, 250.0f), 0.05f);

	SetActorLocation(LerpedPosition);
}

bool AShip::TryLocateOnTile(ABaseTile* Tile, bool RightAfter)
{
	if (Tile == nullptr) return false;
	if (Tile->GetShip() != nullptr) return false;

	if (CurTile != nullptr) CurTile->SetShip(nullptr);

	if (RightAfter) SetActorLocation(Tile->GetActorLocation() + FVector(0.0f, 0.0f, 250.0f));
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

bool AShip::AddUnit(ABaseUnit* Unit)
{
	if (Units.Contains(Unit)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Already In"));
		return false;
	}
	if (Units.Num() == UnitCapacity)
	{
		UE_LOG(LogTemp, Warning, TEXT("Full"));
		return false;
	}
	Units.Add(Unit);
	return true;
}

bool AShip::RemoveUnit(class ABaseUnit* Unit)
{
	if (!Units.Contains(Unit))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Exists"));
		return false;
	}
	Units.Remove(Unit);
	return true;
}

bool AShip::TryAddTileToPath(ABaseTile* Tile, bool bIsFirstPath)
{
	if (bIsFirstPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckFirst"));
		if (Path.Num() == 0 && Tile != CurTile) return false;
		if (Path.Num() != 0 && Tile != Path.Last(0)) return false;
		return true;
	}
	else if (Path.Num() != 0 && Path.Last(0) == Tile)
	{
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Added %s"), *Tile->GetActorNameOrLabel());
		Path.Add(Tile);
		return true;
	}
}

void AShip::EmptyPath()
{
	Path.Empty();
}

void AShip::FollowPath()
{
	UE_LOG(LogTemp, Warning, TEXT("Next"));
	if (!Path.IsEmpty())
	{
		if (Path[0] == CurTile)
		{
			Path.RemoveAt(0);
			if (Path.IsEmpty()) return;
		}
		bool Success = TryLocateOnTile(Path[0], false);
		if (Success) Path.RemoveAt(0);

		if (!Path.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Next"));
			GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AShip::FollowPath, FlightPower, false, FlightPower);
		}
	}
}


