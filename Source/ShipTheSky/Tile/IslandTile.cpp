// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/IslandTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "Components/WidgetComponent.h"
#include "Widget/IslandResourceUI.h"
#include "MapManager.h"

AIslandTile::AIslandTile()
{
	TileType = ETileType::Island;
}

void AIslandTile::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AIslandTile::OnTileReleased(AActor* Target, FKey ButtonPressed)
{
	Super::OnTileReleased(Target, ButtonPressed);
	ASTSPlayerController* PlayerController = Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PlayerController->GetIsPathSelectionMode())
	{

	}
	else
	{
		if (PlayerController->GetCommander()->GetTargetTile() == nullptr) return;

		PlayerController->CloseOwningIslandBuildingUI();
		PlayerController->OpenOwningIslandUI();
	}
	
}

AGuardianTile* AIslandTile::GetGuardianTile() const
{
	return GetGameInstance()->GetSubsystem<UMapManager>()->GetGuardianTile(IslandID);
}
