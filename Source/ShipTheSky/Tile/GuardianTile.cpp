// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/GuardianTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"

void AGuardianTile::OnTileReleased(AActor* Target, FKey ButtonPressed)
{
	Super::OnTileReleased(Target, ButtonPressed);
	ASTSPlayerController* PlayerController = Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->GetCommander()->SetTargetIslandTile(this);
	PlayerController->OpenOwningIslandPanel();
	PlayerController->CloseOwningIslandBuildingPanel();
}