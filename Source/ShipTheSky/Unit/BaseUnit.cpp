// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/BaseUnit.h"
#include "Tile/BaseTile.h"
#include "Tile/IslandTile.h"
#include "MapManager.h"
#include "Pawn/Commander.h"

// Sets default values
ABaseUnit::ABaseUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Unit Mesh"));
	RootComponent = SkeletalMeshComp;
	SkeletalMeshComp->CastShadow = false;
}

void ABaseUnit::LocateToIslandTile(AIslandTile* IslandTile)
{
	if (IslandTile == nullptr) 
	{
		UE_LOG(LogTemp, Error, TEXT("Null Island"));
		return;
	}

	ABaseTile* MainTile = GetGameInstance()->GetSubsystem<UMapManager>()->GetMainIslandTile(IslandTile->GetIslandID());

	if (MainTile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null Island"));
		return;
	}

	FVector Direction = MainTile->GetActorLocation() - IslandTile->GetActorLocation();

	SetActorLocation(IslandTile->GetActorLocation());
	SetActorRotation(Direction.Rotation());
	SetActorHiddenInGame(false);

	IslandTile->SetUnit(this);
	SetCurTile(IslandTile);
}

