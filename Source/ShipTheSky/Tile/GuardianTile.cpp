// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/GuardianTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "Unit/BaseUnit.h"

AGuardianTile::AGuardianTile()
{
	IslandType = EIslandTileType::Guardian;
}

void AGuardianTile::OnTileSelectedAsView(class ASTSPlayerController* PlayerController)
{
	Super::OnTileSelectedAsView(PlayerController);
	if (PlayerController->GetIsPathSelectionMode())
	{

	}
	else
	{
		if (PlayerController->GetCommander()->GetTargetIslandTile() == nullptr) return;
	}
}

void AGuardianTile::AddUnitOnThisIsland(ABaseUnit* Unit)
{
	if (UnitsOnThisIsland.Contains(Unit)) return;
	else
	{
		if (Unit == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Wrong Unit"));
		}
		else
		{
			UnitsOnThisIsland.Add(Unit);
			Unit->SetCurIslandTile(this);
		}
	}
}

void AGuardianTile::RemoveUnitFromThisIsland(ABaseUnit* Unit)
{
	if (UnitsOnThisIsland.Contains(Unit))
	{
		UnitsOnThisIsland.Remove(Unit);
		Unit->SetCurIslandTile(nullptr);
	}
}

void AGuardianTile::BeginPlay()
{
	Super::BeginPlay();

	int32 Index = FMath::RandRange(0, Guardians.Num() - 1);

	AActor* Guardian = GetWorld()->SpawnActor<AActor>(Guardians[Index]);
	Guardian->SetOwner(this);
	Guardian->SetActorLocation(GetActorLocation());
	Guardian->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
	FVector Direction(0.0f, 90.0f, 0.0f);
	Guardian->SetActorRotation(Direction.Rotation());
}
