// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/ResourceTile.h"
#include "Pawn/Commander.h"
#include "Unit/BaseUnit.h"
#include "Building/BaseBuilding.h"

void AResourceTile::SetResources(float Power)
{
	bool Selected[11] = { false };
	int32 NumToSelect = 0;
	int32 NumSelected = 0;
	uint8 TypeSelect;
	switch (IslandType)
	{
	case EIslandTileType::Mine:
		NumToSelect = FMath::RandRange(1, 3);
		while (NumToSelect != NumSelected)
		{
			TypeSelect = FMath::RandRange((int32)EResourceType::StoneCloud, (int32)EResourceType::StoneMeteor);
			if (Selected[TypeSelect]) continue;
			Selected[TypeSelect] = true;
			NumSelected++;
			Resources.Add(TPair<uint8, int32>(TypeSelect, FMath::RandRange(1, 4)));
		}
		break;
	case EIslandTileType::Forest:
		NumToSelect = FMath::RandRange(1, 3);
		while (NumToSelect != NumSelected)
		{
			TypeSelect = FMath::RandRange((int32)EResourceType::WoodCloud, (int32)EResourceType::WoodMeteor);
			if (Selected[TypeSelect]) continue;
			Selected[TypeSelect] = true;
			NumSelected++;
			Resources.Add(TPair<uint8, int32>(TypeSelect, FMath::RandRange(1, 4)));
		}
		break;
	case EIslandTileType::Farm:
		Resources.Add(TPair<uint8, int32>((uint8)EResourceType::Food, FMath::RandRange(1, 4)));
		break;
	default:
		break;
	}
}

void AResourceTile::TimePass(int32 GameDate)
{
	//나중에 1은 더 크게!! GameDAte % 30 == 29로.
	if (GameDate % 1 == 0) GiveResourceToUnit();
	GiveProgressToBuilding();
}

void AResourceTile::OnTileReleased(AActor* Target, FKey ButtonPressed)
{
	Super::OnTileReleased(Target, ButtonPressed);
}

void AResourceTile::GiveResourceToUnit()
{
	if (UnitOnThisTile != nullptr)
	{
		ACommander* OwnerCommander = Cast<ACommander>(UnitOnThisTile->GetOwner());
		if (OwnerCommander != nullptr)
		{
			for (auto Resource : Resources)
			{
				OwnerCommander->SetResource(
					OwnerCommander->GetResource(StaticCast<EResourceType>(Resource.Key)) + Resource.Value,
					StaticCast<EResourceType>(Resource.Key));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Wrong Owner"));
		}
	}
}

void AResourceTile::GiveProgressToBuilding()
{
	if (BuildingOnThisTile != nullptr)
	{
		BuildingOnThisTile->IncreaseProgress();
	}
}
