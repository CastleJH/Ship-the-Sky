// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/BaseUnit.h"
#include "Tile/BaseTile.h"
#include "Tile/IslandTile.h"
#include "Tile/ResourceTile.h"
#include "Tile/GuardianTile.h"
#include "MapManager.h"
#include "Pawn/Commander.h"
#include "Ship.h"

// Sets default values
ABaseUnit::ABaseUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Unit Mesh"));
	RootComponent = SkeletalMeshComp;
	SkeletalMeshComp->CastShadow = false;
}

void ABaseUnit::LocateToResourceTile(AResourceTile* ResourceTile)
{
	if (ResourceTile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null Island"));
		return;
	}

	ABaseTile* MainTile = ResourceTile->GetGuardianTile();

	if (MainTile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null Island"));
		return;
	}

	if (ResourceTile->GetIslandID() != CurIslandTile->GetIslandID())
	{
		UE_LOG(LogTemp, Error, TEXT("Different Island"));
		return;
	}

	FVector Direction = MainTile->GetActorLocation() - ResourceTile->GetActorLocation();

	SetActorLocation(ResourceTile->GetActorLocation());
	SetActorRotation(Direction.Rotation());
	SetActorHiddenInGame(false);

	ResourceTile->SetUnit(this);
	CurIslandTile = ResourceTile;
}

bool ABaseUnit::Embark(AShip* Ship)
{
	if (Ship != nullptr && CurIslandTile != nullptr && Ship->AddUnit(this))
	{
		SetActorHiddenInGame(true);

		if (CurIslandTile != nullptr)
		{
			AResourceTile* Tile = Cast<AResourceTile>(CurIslandTile);
			if (Tile != nullptr) Tile->SetUnit(nullptr);
			GetGameInstance()->GetSubsystem<UMapManager>()->GetGuardianTile(CurIslandTile->GetIslandID())->RemoveUnitFromThisIsland(this);
			SetCurIslandTile(nullptr);
		}

		CurShip = Ship;
		return true;
	}
	if (Ship == nullptr) UE_LOG(LogTemp, Warning, TEXT("Ship nullptr fail"));
	if (CurIslandTile == nullptr) UE_LOG(LogTemp, Warning, TEXT("CurTile nullptr fail"));
	if (!Ship->AddUnit(this)) UE_LOG(LogTemp, Warning, TEXT("Add Unit fail"));
	return false;
}

bool ABaseUnit::Disembark()
{
	if (CurShip != nullptr && CurShip->RemoveUnit(this))
	{
		AIslandTile* IslandTile = Cast<AIslandTile>(CurShip->GetCurTile());
		if (IslandTile == nullptr) return false;
		int32 IslandID = IslandTile->GetIslandID();
		AGuardianTile* GuardianTile = GetGameInstance()->GetSubsystem<UMapManager>()->GetGuardianTile(IslandID);
		if (GuardianTile != nullptr)
		{
			CurShip = nullptr;
			GuardianTile->AddUnitOnThisIsland(this);
			Cast<ACommander>(GetOwner())->FillIslandWithUnit(IslandID, this);
			return true;
		}
	}
	return false;
}
