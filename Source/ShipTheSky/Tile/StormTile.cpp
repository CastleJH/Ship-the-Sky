// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/StormTile.h"
#include "Ship/Ship.h"

AStormTile::AStormTile()
{
	TileType = ETileType::Storm;
}

void AStormTile::TimePass(int32 GameDate)
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
		ShipOnThisTile->SetCurDurability(ShipOnThisTile->GetCurDurability() - GetResistanceAdjustedTilePower(ShipOnThisTile->GetStormResistance()));
	}
}
