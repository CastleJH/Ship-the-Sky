// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/IslandTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "Unit/BaseUnit.h"

AIslandTile::AIslandTile()
{
	TileType = ETileType::Island;
}

void AIslandTile::GiveTileEffect()
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

void AIslandTile::SetResources(float Power)
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

void AIslandTile::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AIslandTile::OnTileReleased(AActor* Target, FKey ButtonPressed)
{
	ASTSPlayerController* PlayerController = Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->GetCommander()->SetTargetIslandTile(this);
	PlayerController->SetIslandTileUIVisibility(true);
}