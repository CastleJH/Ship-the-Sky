// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/GuardianTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "Unit/BaseUnit.h"
#include "Guardian/Guardian.h"

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

void AGuardianTile::TimePass(int32 GameDate)
{
	if (Guardian) Guardian->CheckEnemyShipAdjacentAndAttack();
}

void AGuardianTile::SpawnGuardian(int32 Index)
{
	FTransform Loc(GetActorLocation());
	Guardian = GetWorld()->SpawnActorDeferred<AGuardian>(Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController())->GuardianClass[Index], Loc);
	Guardian->SetGuardianTile(this);
	Guardian->SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	Guardian->FinishSpawning(Loc);
}

void AGuardianTile::BeginPlay()
{
	Super::BeginPlay();
}
