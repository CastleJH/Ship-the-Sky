// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/MeteorTile.h"
#include "Ship/Ship.h"

AMeteorTile::AMeteorTile()
{
	TileType = ETileType::Meteor;
}

void AMeteorTile::TimePass(int32 GameDate)
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
		if (FMath::RandRange(0.0f, 1.0f) < GetResistanceAdjustedTilePower(ShipOnThisTile->GetMeteorResistance()) * 0.01f)
		{
			ShipOnThisTile->GetAttacked(1987654321.0f);
			if (ShipOnThisTile) ShipOnThisTile->GetAttacked(1.0f);
		}
	}
}
