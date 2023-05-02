// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/BaseUnit.h"
#include "Tile/BaseTile.h"
#include "Tile/IslandTile.h"
#include "MapManager.h"

// Sets default values
ABaseUnit::ABaseUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Unit Mesh"));
	RootComponent = SkeletalMeshComp;
	SkeletalMeshComp->CastShadow = false;
}

// Called when the game starts or when spawned
void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseUnit::LocateToIslandTile(AIslandTile* IslandTile)
{
	if (IslandTile == nullptr) return;
	ABaseTile* MainTile = GetGameInstance()->GetSubsystem<UMapManager>()->GetSameIslandTiles(IslandTile->GetIslandID())[0];
	FVector Direction = MainTile->GetActorLocation() - IslandTile->GetActorLocation();

	SetActorLocation(IslandTile->GetActorLocation());
	SetActorRotation(Direction.Rotation());
	SetActorHiddenInGame(false);

	IslandTile->SetUnit(this);
	SetCurTile(IslandTile);
}

