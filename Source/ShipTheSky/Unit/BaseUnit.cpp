// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/BaseUnit.h"
#include "Tile/BaseTile.h"
#include "Tile/IslandTile.h"
#include "Tile/ResourceTile.h"
#include "Tile/GuardianTile.h"
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

void ABaseUnit::LocateToResourceTile(AResourceTile* ResourceTile)
{
	if (ResourceTile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null Island"));
		return;
	}

	ABaseTile* MainTile = ResourceTile->GetGuardianTile();

	if (MainTile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null Island"));
		return;
	}

	FVector Direction = MainTile->GetActorLocation() - ResourceTile->GetActorLocation();

	SetActorLocation(ResourceTile->GetActorLocation());
	SetActorRotation(Direction.Rotation());
	SetActorHiddenInGame(false);

	ResourceTile->SetUnit(this);
	SetCurTile(ResourceTile);
}
