// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/ResourceTile.h"
#include "Enums.h"
#include "Pawn/Commander.h"
#include "Unit/BaseUnit.h"
#include "Building/BaseBuilding.h"
#include "Controller/STSPlayerController.h"
#include "Components/WidgetComponent.h"
#include "MapManager.h"

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

void AResourceTile::SetResources(TMap<uint8, float> NewResources)
{
	Resources = NewResources;
	TotalResourcesNum = 0.0f;
	for (auto Elem : Resources)
	{
		TotalResourcesNum += Elem.Value;
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
