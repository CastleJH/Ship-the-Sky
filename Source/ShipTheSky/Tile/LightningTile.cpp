// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/LightningTile.h"
#include "Ship.h"

ALightningTile::ALightningTile()
{
	TileType = ETileType::Lightning;
}

void ALightningTile::TimePass(int32 GameDate)
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
		ShipOnThisTile->DecreaseHPForUnits(GetResistanceAdjustedTilePower(ShipOnThisTile->GetLightningResistance()) / 10.0f);
	}
}
