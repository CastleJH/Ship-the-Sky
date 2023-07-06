// Fill out your copyright notice in the Description page of Project Settings.


#include "Guardian/Guardian.h"
#include "Battle/BattleComponent.h"
#include "Tile/BaseTile.h"
#include "Tile/GuardianTile.h"
#include "Pawn/Commander.h"
#include "MapManager.h"
#include "Ship.h"
#include "Kismet/KismetMathLibrary.h" 

// Sets default values
AGuardian::AGuardian()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent0"));
	SkeletalMeshComponent->SetupAttachment(Root);
	SkeletalMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	BattleComponent = CreateDefaultSubobject<UBattleComponent>(TEXT("BattleComponent"));
}

// Called when the game starts or when spawned
void AGuardian::BeginPlay()
{
	Super::BeginPlay();
	
	BattleComponent->SetDamage(FMath::RandRange(10, 30));
	BattleComponent->SetMaxHP(FMath::RandRange(50, 100), true);
}

bool AGuardian::CheckEnemyShipAdjacentAndAttack()
{
	EnemyShips.Empty();
	if (!CurTile) return false;
	ACommander* OwningCommander = CurTile->GetIslandOwner();
	for (auto AdjTile : AdjTiles)
	{
		if (AdjTile->GetShip() && Cast<ACommander>(AdjTile->GetShip()->GetOwner()) != OwningCommander)
		{
			EnemyShips.Add(AdjTile->GetShip());
			UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
		}
	}
	if (EnemyShips.Num() == 0) RecoveryCount++;
	else RecoveryCount = 0;
	return EnemyShips.Num() != 0;
}

void AGuardian::AttackEnemyShip()
{
}

void AGuardian::SetGuardianTile(AGuardianTile* NewTile)
{
	GetGameInstance()->GetSubsystem<UMapManager>()->GetAdjacentTiles(NewTile, AdjTiles);
	CurTile = NewTile;
}
