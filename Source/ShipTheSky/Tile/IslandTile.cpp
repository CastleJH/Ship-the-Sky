// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/IslandTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"

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
	ASTSPlayerController* PlayerController = Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->GetCommander()->SetTargetIslandTile(this);
	PlayerController->SetIslandTileUIVisibility(true);
}