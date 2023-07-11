// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/SunTile.h"
#include "Ship.h"

ASunTile::ASunTile()
{
	TileType = ETileType::Sun;
}

void ASunTile::TimePass(int32 GameDate)
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
		ShipOnThisTile->ConsumeFoodForUnits(GetResistanceAdjustedTilePower(ShipOnThisTile->GetSunResistance()));
	}
}
