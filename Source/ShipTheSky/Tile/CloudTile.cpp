// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/CloudTile.h"
#include "Ship.h"

ACloudTile::ACloudTile()
{
	TileType = ETileType::Cloud;
}

void ACloudTile::TimePass(int32 GameDate)
{
	Super::TimePass(GameDate);

	if (ShipOnThisTile)
	{
		if (ShipOnThisTile->GetDurability() == 0)
		{
			ShipOnThisTile->SetModifiedFlightPower(ShipOnThisTile->GetOriginalFlightPower() * 2 + GetResistanceAdjustedTilePower(ShipOnThisTile->GetCloudResistance()));
		}
		else
		{
			ShipOnThisTile->SetModifiedFlightPower(ShipOnThisTile->GetOriginalFlightPower() + GetResistanceAdjustedTilePower(ShipOnThisTile->GetCloudResistance()));
		}
	}
}
