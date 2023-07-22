// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/IslandTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "Components/WidgetComponent.h"
#include "Widget/IslandResourceUI.h"
#include "MapManager.h"
#include "STSGameState.h"
#include "Ship.h"
#include "Tile/GuardianTile.h"

AIslandTile::AIslandTile()
{
	TileType = ETileType::Island;
}

void AIslandTile::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AIslandTile::SetIslandID(int32 NewIslandID)
{
	IslandID = NewIslandID;
	GuardianTile = GetGameInstance()->GetSubsystem<UMapManager>()->GetGuardianTile(IslandID);
}

AGuardianTile* AIslandTile::GetGuardianTile() const
{
	if (GuardianTile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong Guardian Tile:.. %s"), *GetName());
		return nullptr;
	}
	return GuardianTile;
}

void AIslandTile::SetGuardianTile(AIslandTile* Tile)
{
	GuardianTile = Cast<AGuardianTile>(Tile);
}

ACommander* AIslandTile::GetIslandOwner() const
{
	return GetWorld()->GetGameState<ASTSGameState>()->GetIslandOwner(IslandID);
}

void AIslandTile::TimePass(int GameDate)
{
	Super::TimePass(GameDate);
	if (ShipOnThisTile)
	{
		if (ShipOnThisTile->GetCurDurability() == 0)
		{
			ShipOnThisTile->SetModifiedFlightPower(ShipOnThisTile->GetOriginalFlightPower() * 2);
		}
		else
		{
			ShipOnThisTile->SetModifiedFlightPower(ShipOnThisTile->GetOriginalFlightPower());
		}
	}
}
