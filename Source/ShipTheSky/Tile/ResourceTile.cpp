// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/ResourceTile.h"
#include "Enums.h"
#include "Pawn/Commander.h"
#include "Unit/BaseUnit.h"
#include "Building/BaseBuilding.h"
#include "Controller/STSPlayerController.h"
#include "Components/WidgetComponent.h"

AResourceTile::AResourceTile()
{
	ResourcesWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Tile Resources UI"));
	ResourcesWidgetComp->SetupAttachment(RootComponent);
	ResourcesWidgetComp->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	ResourcesWidgetComp->SetDrawSize(FVector2D(75.0f, 40.0f));
	ResourcesWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	ResourcesWidgetComp->SetTickMode(ETickMode::Automatic);
	ResourcesWidgetComp->SetWidgetClass(LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/TileUI/WBP_TileResourcesUI.WBP_TileResourcesUI_C")));
}

void AResourceTile::SetResources(float ResourcePower)
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
	UpdateTileResourcesUI();
}

void AResourceTile::TimePass(int32 GameDate)
{
	Super::TimePass(GameDate);

	//나중에 1은 더 크게!! GameDate % 30 == 29로.
	if (GameDate % 1 == 0) GiveResourceToUnit();
	GiveProgressToBuilding();
	if (UnitOnThisTile)
	{
		UnitOnThisTile->GetOwnerCommander()->SetResource(UnitOnThisTile->GetOwnerCommander()->GetResource(EResourceType::Food) - UnitOnThisTile->GetFoodConsume(), EResourceType::Food);
	}
}

void AResourceTile::UpdateTileResourcesUI()
{
	OnUpdateTileResourcesUI();
	ResourcesWidgetComp->SetWorldLocation(GetActorLocation() + FVector(-180.0f, 0.0f, 0.0f));
}

void AResourceTile::BeginPlay()
{
	Super::BeginPlay();
}

void AResourceTile::GiveResourceToUnit()
{
	if (UnitOnThisTile != nullptr)
	{
		if ((uint8)UnitOnThisTile->GetUnitType() != (uint8)GetIslandType()) return;
		ACommander* OwnerCommander = Cast<ACommander>(UnitOnThisTile->GetOwnerCommander());
		if (OwnerCommander != nullptr)
		{
			for (auto Resource : Resources)
			{
				if (Resource.Key >= (uint8)EResourceType::None) continue;
				OwnerCommander->SetResource(
					OwnerCommander->GetResource(StaticCast<EResourceType>(Resource.Key)) + Resource.Value * (1.0f + UnitOnThisTile->GetEfficiency() * 0.01f),
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
