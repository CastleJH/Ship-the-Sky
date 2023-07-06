// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/IslandTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "Components/WidgetComponent.h"
#include "Widget/IslandResourceUI.h"
#include "MapManager.h"
#include "STSGameState.h"

AIslandTile::AIslandTile()
{
	TileType = ETileType::Island;
}

void AIslandTile::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AIslandTile::OnTileSelectedAsView(ASTSPlayerController* PlayerController)
{
	Super::OnTileSelectedAsView(PlayerController);

	if (PlayerController->GetIsPathSelectionMode())
	{

	}
	else
	{

	}
	
}

AGuardianTile* AIslandTile::GetGuardianTile() const
{
	return GetGameInstance()->GetSubsystem<UMapManager>()->GetGuardianTile(IslandID);
}

ACommander* AIslandTile::GetIslandOwner() const
{
	return GetWorld()->GetGameState<ASTSGameState>()->GetIslandOwner(IslandID);
}
